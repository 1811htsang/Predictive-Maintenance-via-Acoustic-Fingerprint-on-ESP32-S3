# System Architecture Analysis

Ghi chú này dựa trên các tài liệu tổng hợp ý tưởng thiết kế của [sys-des](/docs/notes/system-design-note.md) và các phân tích yêu cầu rõ ràng của [req-ana](requirement-analysis.md) nhằm trình bày thống nhất các phân tích về kiến trúc hệ thống mà dự án đặt ra để đảm bảo hoạt động ổn định và hiệu quả của hệ thống.

## Required Components

Dựa trên các yêu cầu đã được phân tích, hệ thống sẽ cần phải bao gồm các thành phần chính sau:

- SoC ESP32-S3 để xử lý dữ liệu và điều khiển các thành phần khác của hệ thống.
- Cảm biến âm thanh như PCM1808 để thu thập dữ liệu âm thanh từ vòng bạc đạn.
- Op-amp như TL072 để khuếch đại tín hiệu âm thanh trước khi đưa vào SoC.
- Microphone MAX9812 để thu thập dữ liệu thực tế và đa dạng hơn về các tình trạng khác nhau của vòng bạc đạn.
- Jack 3.5mm PJ-313B để kết nối với laptop truyền âm thanh từ dataset vào hệ thống.
- Thẻ microSD để lưu trữ dữ liệu âm thanh và cập nhật firmware.
- Pin sạc lại để cung cấp năng lượng cho hệ thống và đảm bảo tính di động.
- Mạch sạc pin để quản lý quá trình sạc và đảm bảo an toàn cho hệ thống.
- Mạch quản lý tín hiệu sạc để theo dõi mức pin và trạng thái sạc.

## Clock Tree Design

Để đảm bảo hoạt động ổn định và hiệu quả của hệ thống, thiết kế clock tree sẽ được thực hiện như sau:

- SoC ESP32-S3 có nguồn clock riêng biệt giúp hoạt động ở mức tối đa 240Mhz.
- PCM1808 dùng nguồn clock thạch anh 24.576Mhz với IC 74HCU04 để tạo xung clock sạch cho việc lấy mẫu âm thanh ở tốc độ 96kHz.

## Software Architecture

Phần mềm của hệ thống sẽ được thiết kế theo kiến trúc phân lớp để đảm bảo tính modular và dễ bảo trì. Các lớp chính bao gồm:

- Lớp MCAL dùng API của ESP-IDF để giao tiếp với phần cứng và quản lý các thiết bị ngoại vi.
- Lớp Middleware để xử lý dữ liệu âm thanh, thực hiện các thuật toán phân tích và trích xuất đặc trưng.
- Lớp Application để quản lý logic ứng dụng, giao tiếp với người dùng và xử lý các tác vụ liên quan đến dữ liệu và kết quả phân tích.