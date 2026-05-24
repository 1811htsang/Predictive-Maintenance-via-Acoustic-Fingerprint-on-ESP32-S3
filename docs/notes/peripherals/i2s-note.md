# Ghi chú về I2S

## Tổng quan

I2S xuất phát từ I2C nhưng có sự thiết kế chuyên biệt cho truyền tải dữ liệu âm thanh số.

I2S cung cấp 2 ngoại vi chính là I2S0 và I2S1.

Một bus I2S tiêu chuẩn đề ra 3 tín hiệu chính:

- Tín hiệu CLK
- Tín hiệu chọn kênh
- Tín hiệu dữ liệu tuần tự

Và 2 vai trò là:

- Master: tạo xung clock và tín hiệu chọn kênh
- Slave: nhận xung clock và tín hiệu chọn kênh từ Master

Từ vai trò trên, mỗi ESP32 đều sẽ có 2 đơn vị thực hiện ghi nhận truyền tải (Transmit - TX) và ghi nhận (Receive - RX) dữ liệu âm thanh số.

Các tín hiệu đường truyền clock và chọn kênh của cả RX & TX unit đều có thể được config trở thành master chế độ truyền tải hoặc slave chế độ ghi nhận dữ liệu. (*Nghĩa là trên một ESP32, cả RX và TX đều có thể trở thành cùng một lúc master hoặc slave*)

2 đơn vị TX và RX đều được thiết kế sử dụng bộ FIFO 32-bit với sizeof 64.

Ngoài ra, chỉ có I2S0 mới hỗ trợ DAC/ADC nội bộ trên chip cũng như hỗ trợ quá trình truyền nhận tín hiệu PDM (Pulse Density Modulation).

**Thông tin bổ sung:**
PDM hay Pulse Density Modulation là phương pháp chuyển đổi tín hiệu âm thanh analog thành các chuỗi pulse, trong đó mật độ của các pulse phản ánh cường độ âm thanh.

Kiểm chứng thông tin và bổ sung thông tin về tổng quan kiến trúc I2S bên trong ESP32 được cập chi tiết tại [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 313.

Các thông tin tín hiệu chi tiết phía đầu vào I/O của I2S được mô tả chi tiết trong [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 314.

Cần lưu ý, BCK chính là tín hiệu clock tuần tự, WS chính là tín hiệu chọn kênh (Word Select).

Chú ý, ngoại trừ tín hiệu CLK của I2S, tất cả các tín hiệu còn lại được mapping vào chip thông qua ma trận GPIO và IO_MUX. Tín hiệu CLK được mapping vào IO_MUX cố định tại các vị trí GPIO0, GPIO1 (UOTXD), GPIO3 (UORXD) thông qua select hàm GPIO CLK_OUT1, CLK_OUT2, CLK_OUT3.

Thông tin chi tiết hơn về mapping nằm tại [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 61, 62.

## Thiết kế Clock của I2S

Nguồn clock của I2S lấy từ 2 nguồn chính:

- `APLL_CLK` (Nguồn mặc định được sử dụng)
- `PLL_F160M_CLK`

Thông tin bổ sung về 2 nguồn clock này được thu thập trong tài liệu [clk-note](/docs/notes/rcc-note.md).

`PLL_F160M_CLK` là nguồn clock general-purpose, nghĩa là có thể đa dụng với các yêu cầu custom / đòi hỏi sự chính xác không cần quá cao.

Lưu ý thêm, khi sử dụng `PLL_F160M_CLK` làm nguồn clock thì tránh việc sử dụng các tần số chia không nguyên (fractional dividers).

`APLL_CLK` ngược lại là nguồn clock chuyên dụng do độ phân giải cao và độ ổn định tốt hơn.

Sự lựa chọn 2 nguồn clock được xác định bởi bit `I2S_CLKA_ENA` của thanh ghi `I2S_CLKM_CONF_REG`.

Sau khi đã lựa chọn nguồn clock, tín hiệu clock sẽ được đưa qua bộ chia tần số (divider) để tạo ra tần số clock phù hợp với yêu cầu truyền tải dữ liệu.

Bộ chia tần số này bao gồm 2 phần:

- Bộ chia thứ nhất theo công thức
  $$f_{\text{I2S\_CLK}} = \frac{f_\text{CLK\_SRC}}{N+\frac{a}{b}}$$
  Trong đó:
  - N là giá trị được lưu trữ trong `REG_CLKM_DIV_NUM[7:0]`
  - b là giá trị được lưu trữ trong `I2S_CLKM_DIV_B[5:0]`
  - a là giá trị được lưu trữ trong `I2S_CLKM_DIV_A[5:0]`
  
  Tất cả giá trị này đều lưu trong thanh ghi `I2S_CLKM_CONF_REG`.
- Bộ chia thứ hai theo công thức
  $$f_{\text{I2S\_O\_BCK}} = \frac{f_{\text{I2S}}}{M}$$
  Trong đó
  - Nếu ở chế độ truyền tải master, M là giá trị được lưu trữ trong `I2S_TX_BCK_DIV_NUM[7:0]`
  - Nếu ở chế độ ghi nhận master, M là giá trị được lưu trữ trong `I2S_RX_BCK_DIV_NUM[7:0]`

  Cả hai giá trị này đều lưu trong thanh ghi `I2S_SAMPLE_RATE_CONF_REG`.

Ngoài ra một lưu ý cần bổ sung rằng khi hoạt động ở chế độ slave thì thiết bị master phải sử dụng `I2S_CLK` làm master clock và thỏa mãn yêu cầu
$$f_{\text{I2S\_CLK}} \geq 8\times f_{\text{I2S\_O\_BCK}}$$

## Các chế độ hoạt động của I2S

I2S hỗ trợ nhiều chế độ hoạt động khác nhau, bao gồm:

- Tiêu chuẩn Philips
- Tiêu chuẩn MSB-Alignment
- Tiêu chuẩn PCM

Ngoài ra I2S module của ESP32 cũng hỗ trợ nén/giải nén A-law. Khi sử dụng A-law nên lưu ý xóa giá trị 2 bit `RX_PCM_BYPASS` và `TX_PCM_BYPASS` trong thanh ghi `I2S_CONF_REG` để kích hoạt chức năng này.

**Giải thích bổ sung**: A-law là một phương pháp nén tín hiệu âm thanh số nhằm giảm thiểu dung lượng dữ liệu trong quá trình truyền tải mà vẫn giữ được chất lượng âm thanh tương đối tốt.

### Thiết kế hoạt động chung của các tiêu chuẩn

- BCK chính là tín hiệu clock tuần tự
- WS chính là tín hiệu chọn kênh trái / phải
- SD là tín hiệu dữ liệu tuần tự trong việc T/R dữ liệu âm thanh số.

Tín hiệu WS và SD thay đổi theo cạnh âm của tín hiệu BCK trong khi tín hiệu SD có thể được lấy mẫu tại cạnh dương của tín hiệu BCK.

Ngoài ra, nếu bit `I2S_RX_RIGHT_FIRST` và `I2S_TX_RIGHT_FIRST` trong thanh ghi `I2S_CONF_REG` được thiết lập (mức cao 1) thì module sẽ được setup cho T/R dữ liệu của kênh phải trước. Ngược lại, nếu các bit này được xóa (mức thấp 0) thì module sẽ được setup cho T/R dữ liệu của kênh trái trước.

### Tiêu chuẩn Philips

- Tín hiệu WS chuyển đổi trạng thái **sớm hơn** tín hiệu khởi đầu và kết thúc của SD đúng 1 chu kỳ BCK.
- Tín hiệu SD truyền tải bit MSB của dữ liệu trước tiên. (Big Endian format)
- Nếu bit `I2S_RX_MSB_SHIFT` và `I2S_TX_MSB_SHIFT` trong thanh ghi `I2S_CONF_REG` được thiết lập (mức cao 1) thì module sẽ hoạt động theo tiêu chuẩn Philips.

Hình mô phỏng tiêu chuẩn Philips có trong [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 316.

### Tiêu chuẩn MSB-Alignment

- Tín hiệu WS chuyển đổi trạng thái **cùng lúc** với tín hiệu khởi đầu và kết thúc của SD.
- Tín hiệu SD truyền tải bit MSB của dữ liệu trước tiên. (Big Endian format)
- Nếu bit `I2S_RX_MSB_SHIFT` và `I2S_TX_MSB_SHIFT` trong thanh ghi `I2S_CONF_REG` được xóa (mức thấp 0) thì module sẽ hoạt động theo tiêu chuẩn MSB-Alignment.

Hình mô phỏng tiêu chuẩn MSB-Alignment có trong [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 317.

### Tiêu chuẩn PCM

- Tín hiệu WS chuyển đổi và giữ trạng thái **sớm hơn** tín hiệu khởi đầu của SD đúng 1 chu kỳ BCK. (*Nghĩa là 1 chu kỳ trước khi tín hiệu SD ghi nhận dữ liệu thì tín hiệu WS đã chuyển đổi trạng thái và giữ trạng thái trong 1 chu kỳ, khi tín hiệu SD ghi nhận dữ liệu thì tín hiệu WS sau khi giữ trạng thái được 1 chu kỳ sẽ thay đổi*)
- Tín hiệu SD truyền tải bit MSB của dữ liệu trước tiên. (Big Endian format)
- Nếu bit `I2S_RX_SHORT_SYNC` và `I2S_TX_SHORT_SYNC` trong thanh ghi `I2S_CONF_REG` được thiết lập (mức cao 1) thì module sẽ hoạt động theo tiêu chuẩn PCM.

## Reset

Có 4 loại reset chính trong module I2S:

- TX_RESET: reset đơn vị truyền tải (TX)
- RX_RESET: reset đơn vị ghi nhận (RX)
- TX_FIFO_RESET: reset bộ FIFO của đơn vị truyền tải (TX)
- RX_FIFO_RESET: reset bộ FIFO của đơn vị ghi nhận (RX)

Tất cả các loại reset này đều được thực hiện thông qua việc thiết lập (mức cao 1) các bit tương ứng trong thanh ghi `I2S_CONF_REG`.

Kiểm chứng thông tin về reset module I2S được mô tả chi tiết trong [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 317.

## Hoạt động của FIFO

FIFO đóng vai trò lưu trữ tạm thời.

Các dữ liệu gửi đi và ghi nhận được đều bắt buôc phải đi qua FIFO.

FIFO có thể access thông qua CPU hoặc DMA.

## Cơ chế hoạt động

- Bật bit `I2S_RX_FIFO_MOD_FORCE_EN` và `I2S_TX_FIFO_MOD_FORCE_EN` trong thanh ghi `I2S_FIFO_CONF_REG` lên 1.
- Kiểm soát độ dài dữ liệu thông qua các bit `I2S_TX_DATA_NUM[5:0]` và `I2S_RX_DATA_NUM[5:0]` trong thanh ghi `I2S_FIFO_CONF_REG`.
- Phần cứng sẽ tự động quản lý số lượng thông qua `RX_LEN` và `TX_LEN`.
- Nếu `RX_LEN` > `I2S_RX_DATA_NUM[5:0]` thì sẽ kích hoạt ngưỡng và cần đọc ra để tránh tràn FIFO.
- Nếu `TX_LEN` < `I2S_TX_DATA_NUM[5:0]` thì phần mềm có thể tiếp tục ghi dữ liệu vào FIFO.

Kiểm chứng thông tin về hoạt động của FIFO được mô tả chi tiết trong [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 318.

## Nguyên lý gửi dữ liệu

Bit `I2S_TX_START` của thanh ghi `I2S_CONF_REG` được thiết lập (mức cao 1) để bắt đầu quá trình truyền tải dữ liệu. Khi I2S ở chế độ master truyền tải thì module đóng vai trò tạo tín hiệu clock và dữ liệu 2 kênh.

Nếu dữ liệu trong FIFO đã truyền tải hết và không còn dữ liệu mới để gửi đi thì tín hiệu dữ liệu sẽ giữ nguyên giá trị cuối cùng đã truyền tải.

Nếu bit `I2S_TX_START` được xóa (mức thấp 0) thì quá trình truyền tải dữ liệu sẽ dừng lại sau khi hoàn thành việc gửi đi dữ liệu hiện tại.

Đối với chế độ slave truyền tải thì module sẽ chờ tín hiệu clock và dữ liệu từ thiết bị master.

Diễn ra 3 giai đoạn:

1. Đọc dữ liệu từ bộ nhớ nội và truyền tải vào trong FIFO.
2. Đọc dữ liệu từ FIFO.
3. Truyền dữ liệu tuần tự hoặc song song tùy vào cấu hình.

### Ở giai đoạn 1 gửi

Do có thể chia ra chế độ truyền tuần tự / song song nên cấu trúc dữ liệu trong FIFO cũng có sự khác biệt.

Trong đó, ở chế độ song song, 16 bit cao của 2 địa chỉ sẽ truyền vào FIFO trước, tiếp theo là 16 bit thấp của 2 địa chỉ.

Kiểm chứng thông tin về nguyên lý gửi dữ liệu được mô tả chi tiết trong [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 319.

### Ở giai đoạn 2 gửi

Chế độ đọc dữ liệu sẽ do các bit `I2S_TX_FIFO_MOD[2:0]` và `I2S_TX_CHAN_MOD[2:0]`.

`I2S_TX_FIFO_MOD[2:0]` xác định dữ liệu sẽ theo loại 16-bit hay 32-bit.

`I2S_TX_CHAN_MOD[2:0]` xác định định dạng dữ liệu sẽ truyền tải

Kiểm chứng thông tin về nguyên lý gửi dữ liệu được mô tả chi tiết trong [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 319.

### Ở giai đoạn 3 gửi

Tín hiệu dữ liệu sẽ được xác định bởi các chế độ đã chọn của I2S và `I2S_TX_BITS_MOD[5:0]` của thanh ghi `I2S_SAMPLE_RATE_CONF_REG`.

## Nguyên lý nhận dữ liệu

Bit `I2S_RX_START` của thanh ghi `I2S_CONF_REG` được thiết lập (mức cao 1) để bắt đầu quá trình ghi nhận dữ liệu. Khi I2S ở chế độ master ghi nhận thì module đóng vai trò tạo tín hiệu clock và lấy mẫu dữ liệu 2 kênh cho đến khi bit `I2S_RX_START` được xóa (mức thấp 0).

Nếu I2S ở chế độ slave ghi nhận thì module sẽ chờ tín hiệu clock và dữ liệu từ thiết bị master.

Diễn ra 3 giai đoạn:

1. Dòng dữ liệu bit tuần tự được biến đổi thành dòng dữ liệu song song 64 bit.
2. Dữ liệu song song 64 bit được truyền vào FIFO.
3. Dữ liệu từ FIFO được ghi vào bộ nhớ nội thông qua CPU/DMA.

### Ở giai đoạn 1 nhận

Dòng dữ liệu nhận được sẽ mở rộng thành dòng dữ liệu song song không padding 64 bit với 32 bit cao và 32 bit thấp dựa vào cấu hình của tín hiệu `I2SnI_WS_out` hoặc `I2SnI_WS_in`.

Về cách mở rộng dữ liệu sẽ do bit `I2S_RX_MSB_RIGHT` của thanh ghi `I2S_CONF_REG` quyết định.

Giải thích ví dụ ở trang 320 của [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf):

Trong hình, cho độ rộng dữ liệu là 16 bit, và dữ liệu tuần tự lần lượt là

1. `0x7654`
2. `0xFEDC`
3. `0x3210`
4. `0xBA98`

- Nếu `I2S_RX_RIGHT_FIRST = 1`, data0 sẽ bị loại bỏ ghi nhận, data1 làm dữ liệu đầu tiên mà I2S ghi nhận.
  - Nếu `I2S_RX_MSB_RIGHT = 1`, dữ liệu đầu tiên ghi nhận được là `{0xFEDC0000, 0x32100000}`
  - Nếu `I2S_RX_MSB_RIGHT = 0`, dữ liệu đầu tiên ghi nhận được là `{0x32100000, 0xFEDC0000}`
- Nếu `I2S_RX_RIGHT_FIRST = 0`, data0 sẽ là dữ liệu đầu tiên mà I2S ghi nhận.
  - Nếu `I2S_RX_MSB_RIGHT = 1`, dữ liệu đầu tiên ghi nhận được là `{0xFEDC0000, 0x76540000}`
  - Nếu `I2S_RX_MSB_RIGHT = 0`, dữ liệu đầu tiên ghi nhận được là `{0x76540000, 0xFEDC0000}`

Kiểm chứng thông tin về nguyên lý nhận dữ liệu được mô tả chi tiết trong [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 320.

### Ở giai đoạn 2 nhận

Dữ liệu nhận vào từ đơn vị RX sẽ lưu vào trong FIFO.

Các chế độ lưu vào được xác định bởi các bit `I2S_RX_FIFO_MOD[2:0]` trong thanh ghi `I2S_FIFO_CONF_REG`.

Kiểm chứng thông tin về nguyên lý nhận dữ liệu được mô tả chi tiết trong [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 320.

### Ở giai đoạn 3 nhận

CPU/DMA sẽ đọc dữ liệu từ FIFO và ghi vào bộ nhớ nội.

Các cấu hình đọc dữ liệu được xác định bởi `I2S_RX_MSB_RIGHT` và `I2S_RX_CHAN_MOD`.

## Chế độ Slave/ Master

Cả đơn vị TX và RX đều có thể hoạt động ở chế độ Slave hoặc Master.

Tất cả các chê độ đều có thể hỗ trợ full-duplex và half-duplex.

Xác định thông qua `I2S_RX_SLAVE_MOD` và `I2S_TX_SLAVE_MOD` trong thanh ghi `I2S_CONF_REG`.

## Ngắt của I2S

Trình bày chi tiết trong tài liệu [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 326, 327.
