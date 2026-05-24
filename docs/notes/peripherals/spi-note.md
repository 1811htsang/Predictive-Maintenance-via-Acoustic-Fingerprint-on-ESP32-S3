# Ghi chú về SPI

## Tổng quan

Trong ESP32, SPI được tích hợp 4 bộ điều khiển SPI phần cứng, bao gồm SPI0, SPI1, SPI2 và SPI3.

Cụ thể hơn:

- SPI0 dùng làm bộ đệm để truy cập bộ nhớ ngoài là Cache
- SPI1 dùng làm master của giao thức
- SPI2 và SPI3 dùng làm maste/slave

Khi sử dụng làm master, các SPI Controller có thể điều khiển nhiều tín hiệu CS (Chip Select, CS0 ~ CS2) để kích hoạt nhiều slave khác nhau.

Các SPI1 ~ SPI3 chia sẻ 2 kênh DMA.

Thông tin về chi tiết kiến trúc thiết kế có trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) trang 127.

## Bộ tín hiệu SPI

Các tín hiệu SPI bao gồm:

- D
- Q
- CS0-CS2
- CLK
- WP
- HD

SPI0 và SPI1 chia sẻ chung đường tín hiệu thông qua Arbiter, tín hiệu line đầu ra này là `SPI`.

SPI2 có line tín hiệu riêng là `HSPI`, SPI3 có line tín hiệu riêng là `VSPI`.

Các đường tín hiệu trên có thể map chân thông qua các IO_MUX hoặc GPIO Matrix.

Các SPI Controller hỗ trợ các chế độ truyền dữ liệu khác nhau trong GP-SPI (General Purpose SPI) như:

- 4 line full-duplex
- 4 line half-duplex
- 3 line half-duplex

Trong chế độ QSPI (Quad SPI), một bộ SPI controller hỗ trợ truy cập bộ nhớ Flash hoặc SRAM thông tin các line tín hiệu như 1 bus dữ liệu SPI song song 4 bit

Thông tin về mapping chi tiết có trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) trang 127.

## Chế độ SPI

### Chế độ GP-SPI

- Hỗ trợ lập trình độ dài dữ liệu truyền và nhận khác nhau, từ 1 bit đến 64 bit.
- Các chế độ truyền dữ liệu: 4 line full-duplex/half-duplex và 3 line half-duplex.
- Master/Slave.
- Có thể lập trình CPOL (Clock Polarity) và CPHA (Clock Phase).

**Ghi chú bổ sung:**

- CPOL (Clock Polarity): Xác định mức logic của tín hiệu clock khi không truyền dữ liệu. Nếu CPOL = 0, tín hiệu clock ở mức thấp khi không truyền dữ liệu; nếu CPOL = 1, tín hiệu clock ở mức cao khi không truyền dữ liệu.
- CPHA (Clock Phase): Xác định thời điểm dữ liệu được lấy mẫu so với tín hiệu clock. Nếu CPHA = 0, dữ liệu được lấy mẫu vào cạnh đầu tiên của tín hiệu clock; nếu CPHA = 1, dữ liệu được lấy mẫu vào cạnh thứ hai của tín hiệu clock.

Hiểu đơn giản hơn, giá trị CPOL dành cho event `idle` của clock, còn CPHA dành cho event `data sampling`.
CPOL sẽ xác định độ lệch mức, còn CPHA sẽ xác định độ lệch cạnh lấy mẫu.

### Chế độ QSPI song song

- Hỗ trợ lập trình các định dạng giao thức cho các thiết bị slave cụ thể như Flash và SRAM.
- 6 loại thao tác đọc trên Flash
- Tự động chuyển đổi giữa Flash và SRAM
- Tự động chờ đối với truy cập Flash

### Chế độ DMA

- Hỗ trợ truyền dữ liệu thông qua DMA sử dụng danh sách liên kết.

### Chế độ ngắt

- Hỗ trợ ngắt cho các sự kiện như hoàn thành truyền dữ liệu, lỗi truyền dữ liệu, v.v.
- Hỗ trợ cấu hình ngắt cho từng loại sự kiện cụ thể với chế độ DMA.

Kiểm chứng các thông tin trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) trang 128.

## GP-SPI

Sử dụng 4 dây chính (SCLK, MOSI, MISO, CS) để truyền dữ liệu giữa Master và Slave.

SPI1 ~ SPI3 có thể giao tiếp với các slaves khác như một SPI master tiêu chuẩn.

SPI2 và SPI3 có thể được cấu hình làm SPI master/slave.

Mỗi SPI controller hỗ trợ tối đa 3 tín hiệu CS để giao tiếp với nhiều slave khác nhau.

Khi không sử dụng DMA, độ dài dữ liệu truyền/nhận tối đa trong 1 lần truyền là 64 byte, xét theo công nhân với 1 byte.

Thông tin về các lệnh hỗ trợ vởi GP-SPI slave chế độ half-duplex có trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) trang 129.

### Chế độ 4 line full-duplex

Trong chế độ này, SPI có thể làm master/slave.

Cấu hình chế độ này bằng cách thiết lập các thanh ghi:

- bit `SPI_DOUTDIN` và `SPI_USR_MOSI` trong `SPI_USER_REG`

Độ dài truyền/nhận dữ liệu được cấu hình bởi các thanh ghi:

- `SPI_MISO_DLEN_REG` và `SPI_MOSI_DLEN_REG` cho master
- `SPI_SLV_RDBUF_DLEN_REG` và `SPI_SLV_WRBUF_DLEN_REG` cho slave

Khởi động truyền/nhận dữ liệu bằng cách thiết lập bit `SPI_USR` trong `SPI_CMD_REG`.

### Chế độ 4 line half-duplex

Trong chế độ này, SPI có thể làm master/slave.

Hỗ trợ linh hoạt các định dạng giao tiếp.

Định dạng giao tiếp được chỉ định như sau:

- `command` dài 0~16 bit
- `address` dài 0~32 (GP-SPI master)/64 (QSPI master) bit
- `dummy` dài 0~256 bit
- `data` dài 0~512 bit (64 bytes)

Các thành phần này được điều khiển bởi bit `SPI_USR_COMMAND`, `SPI_USR_ADDR`, `SPI_USR_DUMMY`, và `SPI_USR_MOSI`/`SPI_USR_MISO` trong thanh ghi `SPI_USER_REG`.

Mỗi phase kích hoạt bởi set bit = 1 tương ứng trong `SPI_USER_REG`.

Khi là master, thanh ghi này có thể được cấu hình để xác định kích hoạt từng phase

Khi là slave, bắt buộc các tiêu chuẩn định dạng giao tiếp, CS phải giữ mức thấp trong toàn bộ quá trình truyền dữ liệu. Nếu CS mức cao thì truyền dữ liệu sẽ bị reset.

Master có thể ghi vào thanh ghi trạng thái `SPI_SLV_WR_STATUS_REG` của slave và quyết định xem có nên đọc dữ liệu từ thanh ghi `SPI_SLV_WR_STATUS_REG` hay thanh ghi `SPI_RD_STATUS_REG` thông qua bit `SPI_SLV_STATUS_READBACK` trong thanh ghi `SPI_SLAVE1_REG`.

Master có thể duy trì liên lạc với thiết bị phụ bằng cách đọc và ghi vào thanh ghi trạng thái của thiết bị phụ, từ đó dễ dàng thực hiện các giao tiếp phức tạp.

Độ dài truyền/nhận dữ liệu được cấu hình bởi các thanh ghi:

- `SPI_MISO_DLEN_REG` và `SPI_MOSI_DLEN_REG` cho master
- `SPI_SLV_RDBUF_DLEN_REG` và `SPI_SLV_WRBUF_DLEN_REG` cho slave

Việc nhận hoặc truyền dữ liệu được điều khiển bởi bit `SPI_USR_MOSI` hoặc `SPI_USR_MISO` trong thanh ghi `SPI_USER_REG`.

Bit `SPI_USR` trong thanh ghi `SPI_CMD_REG` cần được cấu hình để khởi tạo quá trình truyền dữ liệu.

### Chế độ 3 line half-duplex

Trong chế độ này phải tuân thủ định dạng giao tiếp nghiêm ngặt

Có thể bật chế độ này bằng cách thiết lập bit `SPI_SIO` trong thanh ghi `SPI_USER_REG`.

Trong chế độ này định dạng `command` + `address` + `received data` + `sent data` và `received data` + `sent data` không áp dụng cho DMA.

Khi SPI đóng vai trò là slave, CS của master phải hoạt hiệu ít nhất 1 chu kỳ trước khi quá trình đọc/ghi bắt đầu và vô hiệu hóa ít nhất 1 chu kỳ sau khi quá trình đọc/ghi kết thúc.

### Bộ đệm dữ liệu

SPI của ESP32 có bộ đệm dữ liệu 16×32 bit để đệm các thao tác gửi và nhận dữ liệu.

Dữ liệu nhận được mặc định được ghi từ byte thấp của `SPI_W0_REG` và quá trình ghi kết thúc ở `SPI_W15_REG`.

Nếu độ dài dữ liệu vượt quá 64 byte, phần dư sẽ được ghi từ `SPI_W0_REG`. Điều này có nghĩa bộ đệm dữ liệu có thể hoạt động như một vòng đệm.

Các khối bộ đệm dữ liệu `SPI_W0_REG` ~ `SPI_W7_REG` và `SPI_W8_REG` ~ `SPI_W15_REG` tương ứng với phần thấp và phần cao. 2 bộ này có thể được sử dụng riêng biệt, điều khiển bởi bit `SPI_USR_MOSI_HIGHPART` và `SPI_USR_MISO_HIGHPART` trong thanh ghi `SPI_USER_REG`.

Kiểm chứng thông tin có trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) trang 130.

### Kiểm soát clock

Tần số clock đầu ra tối đa của ESP32 GP-SPI master là $f_{\text{APB}}/2$ và tần số clock đầu vào tối đa của ESP32 GP-SPI slave là $f_{\text{APB}}/8$.

Master có thể tạo ra các tần số xung nhịp khác thông qua phép chia tần số với công thức
$$f_{\text{SPI}}=\frac{f_{\text{APB}}}{(\text{SPI\_CLKCNT\_N}+1)(\text{SPI\_CLKDIV\_PRE}+1)} $$

Trong đó:

- `SPI_CLKCNT_N` và `SPI_CLKDIV_PRE` là các giá trị được cấu hình trong thanh ghi `SPI_CLOCK_REG`.

2 bit `SPI_CLKDIV_PRE`, `SPI_CLKCNT_N` chỉ sử dụng khi `SPI_CLK_EQU_SYSCLK` trong thanh ghi `SPI_CLOCK_REG` được đặt thành 0, trong chế độ slave thì nên set về 0.

Nếu bit `SPI_CLK_EQU_SYSCLK` được đặt thành 1, tần số clock đầu ra của SPI master sẽ bằng tần số APB.

Với các tần số clock khác, `SPI_CLK_EQU_SYSCLK` phải được đặt thành 0.

Ở chế độ slave, `SPI_CLKCNT_N`, `SPI_CLKCNT_L`, `SPI_CLKCNT_H` và `SPI_CLKDIV_PRE` đều phải bằng 0.

#### CPOL và CPHA

CPOL và CPHA được điều khiển bởi các thành phần sau:

- bit `SPI_CK_IDLE_EDGE` trong thanh ghi `SPI_PIN_REG`
- bit `SPI_CK_OUT_EDGE`, `SPI_CK_I_EDGE` trong thanh ghi `SPI_USER_REG`
- bit `SPI_MISO_DELAY_MODE[1:0]`, `SPI_MISO_DELAY_NUM[2:0]`, `SPI_MOSI_DELAY_MODE[1:0]`, bit `SPI_MOSI_DELAY_NUM[2:0]` trong thanh ghi `SPI_CTRL2_REG`.

Các chế độ bao gồm:

- mode0: CPOL=0, CPHA=0
- mode1: CPOL=0, CPHA=1
- mode2: CPOL=1, CPHA=0
- mode3: CPOL=1, CPHA=1

Bảng thông tin trang 131 trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) mô tả chi tiết về các thiết lập này.

Ngoài ra cần lưu ý thêm mode0 và mode2, các thanh ghi được cấu hình khác nhau ở chế độ không DMA và chế độ DMA, và dữ liệu SPI slave được xuất ra trước ở chế độ DMA.

#### Timing

Như đã đề cập, tín hiệu dữ liệu được mapping vào pin vật lý thông qua IO_MUX hoặc GPIO Matrix nên sẽ delay 2 chu kỳ $\text{clk}_{\text{APB}}$khi đi qua GPIO Matrix. Tín hiệu đầu ra sẽ không bị delay.

Khi GP-SPI làm master và tín hiệu dữ liệu không được SPI Controller nhận qua ma trận GPIO:

- Nếu tần số clock đầu ra của GP-SPI là $\text{clk}_{\text{APB}}/2$, thì thanh ghi `SPI_MISO_DELAY_MODE` phải được đặt thành 0 khi cấu hình CPOL.
- Nếu tần số xung nhịp đầu ra của GP-SPI < $\text{clk}_{\text{APB}}/4$, thì thanh ghi `SPI_MISO_DELAY_MODE` có thể được đặt thành giá trị tương ứng khi cấu hình CPOL.

Khi GP-SPI làm master và tín hiệu dữ liệu không được SPI Controller nhận qua ma trận GPIO:

- Nếu tần số clock đầu ra của GP-SPI là $\text{clk}_{\text{APB}}/2$, thanh ghi `SPI_MOSI_DELAY_MODE = 0`, `dummy` phải hoạt hiệu (`SPI_USR_DUMMY = 1`) trong 1 chu kỳ $\text{clk}_{\text{SPI}}$ (`SPI_USR_DUMMY_CYC LELEN = 0`) khi cấu hình CPOL.
- Nếu tần số clock đầu ra của GP-SPI là $\text{clk}_{\text{APB}}/4$, thanh ghi `SPI_MISO_DELAY_MODE = 0` khi cấu hình CPOL.
- Nếu tần số clock đầu ra của GP-SPI < $\text{clk}_{\text{APB}}/8$, thanh ghi `SPI_MOSI_DELAY_MODE` có thể được đặt thành giá trị tương ứng khi cấu hình CPOL.

Khi sử dụng GP-SPI ở chế độ slave, tín hiệu clock và tín hiệu data phải được dẫn đến SPI controller thông qua cùng một đường dẫn, nghĩa là cả tín hiệu clk và tín hiệu data không đi qua GPIO matrix, hoặc cả hai đều đi qua GPIO matrix. Điều này rất quan trọng để đảm bảo các tín hiệu không bị trễ với các khoảng thời gian khác nhau trước khi đến phần cứng SPI.

Kiểm chứng thông tin trong trang 133 của [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf).

## Phần cứng hỗ trợ ngắt

SPI tạo ra 2 loại ngắt là ngắt SPI và ngắt SPI DMA.

SPI xem sự hoàn tất của quá trình truyền/nhận dữ liệu là sự hoàn tất của controller và tạo ra 1 ngắt. Khi SPI được cấu hình làm slave mode, slave sẽ tạo các status reg & bộ đệm đọc/ghi ngắt dựa theo các hoạt động.

Thông tin về các ngắt có trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) trang 134.

## QSPI

Thông tin về kết nối sử dụng QSPI cho Flash và SRAM có trong trang 133, 134 của [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf).

SPI1 ~ SPI3 có thể được cấu hình làm QSPI master để giao tiếp với các thiết bị ngoại vi như Flash và SRAM sử dụng giao thức QSPI. Tần số tối đa có thể cho ra là $f_{\text{APB}}$ với cấu hình clock tương tự như GP-SPI.

## Định dạng giao thức

Định dạng giao thức tương tự GP-SPI 4 line half-duplex, ngoại trừ `address` và `data` có thể cấu hình để thực hiện 2-line/4-line truyền dữ liệu.
