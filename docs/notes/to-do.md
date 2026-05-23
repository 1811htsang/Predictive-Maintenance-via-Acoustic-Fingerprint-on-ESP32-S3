# To do List

## Ghi chú

Lộ trình triển khai chi tiết, đi từ

  1. **Cốt lõi** (Hardware interface & I2S Driver)
  2. **Kiến trúc** (Dual-core & Memory Management)
  3. **DSP** (Digital Signal Processing)
  4. **Giao tiếp dữ liệu** (Connectivity & Data Visualization)
  5. **Kiểm chứng và tối ưu** (Validation & Optimization)

## Các công việc phụ

- [ ] Sửa đổi lại README để cập nhật thông tin về dự án và hướng dẫn sử dụng
- [ ] Viết tài liệu hướng dẫn sử dụng và cấu hình hệ thống cho người dùng

## Giai đoạn Cốt lõi

- [x] Thiết kế khung dự án (Project Structure)
- [x] Bổ sung các tài liệu căn bản của dự án
- [x] Tìm hiểu về thiết kế reset và clock nội bộ của ESP32
- [x] Viết driver setup clock cho ESP32 lên tối đa 240MHz
- [x] Tìm hiểu về I2S Protocol và cách hoạt động của I2S trên ESP32
- [x] Tìm hiểu các linh kiện phần cứng hỗ trợ thiết kế
- [x] Tìm hiểu về SPI Protocol và cách hoạt động của SPI trên ESP32
- [x] Tái cấu trúc và thay đổi cách tận dụng driver từ ESP-IDF thay thế cho bare-metal driver
- [x] Mua linh kiện phần cứng cần thiết cho việc phát triển và thử nghiệm
- [x] Hàn mạch PCM1808 để thử nghiệm và cấu hình I2S
- [x] Cấu hình I2S để thu tín hiệu âm thanh từ PCM1808 và MAX9812
- [x] Cắm mạch và kiểm tra tín hiệu hoạt động

## Giai đoạn Kiến trúc phần cứng

- [x] Thiết kế PCB theo mẫu sử dụng PCM1808, MAX9812, ESP32, TL072IDT gốc
- [x] Bổ sung thiết kế phân tách đường tín hiệu cơ học với PJ-342S để giảm nhiễu và cải thiện chất lượng âm thanh
- [x] Làm mượt tín hiệu clock cho PCM1808 với mạch Pierce Oscillator để giảm jitter và cải thiện chất lượng âm thanh
- [x] Bổ sung đầu thu siêu âm TCT40-16R để thu tín hiệu siêu âm và tích hợp vào hệ thống làm stereo mode
- [x] Bổ sung mạch ổn áp DC-DC 1 MHz 3/6V-12V để cung cấp nguồn ổn định cho hệ thống
- [x] Thêm các linh kiện bổ sung để tách nguồn xuống 9V, 5V, 3.3V cho các thành phần khác nhau của hệ thống
- [ ] Bổ sung LDO cho mạch để giảm nhiễu và cải thiện chất lượng âm thanh
- [ ] Kiểm tra lại kích thước và bố trí linh kiện đã về hàng để đảm bảo thiết kế PCB phù hợp và tối ưu
- [ ] Đặt gia công in mạch

## Giai đoạn Xử lý dữ liệu & Truyền thông

- [x] Lựa chọn dataset và thuật toán xử lý dữ liệu âm thanh phù hợp để kiểm chứng khả năng thu thập dữ liệu âm thanh chất lượng cao của hệ thống
- [ ] Thiết kế server nội bộ để ESP32 giao tiếp và truyền dữ liệu âm thanh về kit xử lý trung tâm
- [ ] Thiết kế driver giao tiếp giữa ESP32 và server nội bộ
- [ ] Tìm hiểu cơ chế giao tiếp với PSRAM để tận dụng làm ring buffer cho dữ liệu âm thanh
- [ ] Cân nhắc vấn đề sử dụng Core CIEDPC để xử lý hệ thống theo hướng sự kiện (event-driven) để tối ưu hiệu suất và giảm độ trễ với 2 core của ESP32

## Giai đoạn Kiểm chứng và tối ưu
