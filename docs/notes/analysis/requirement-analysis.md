# Requirement Analysis

Ghi chú này dựa trên các tài liệu tổng hợp ý tưởng thiết kế của [sys-des](/docs/notes/system-design-note.md) nhằm trình bày thống nhất các yêu cầu mà dự án đặt ra để đảm bảo hoạt động ổn định và hiệu quả của hệ thống.

## User Requirements

### Object

Đối tượng được xác định lựa chọn là các vòng bạc đạn - "bearings" sử dụng trong công nghiệp.

Đây là một thành phần quan trọng trong nhiều loại máy móc và thiết bị, và việc giám sát tình trạng của chúng có thể giúp ngăn ngừa sự cố và tăng tuổi thọ của thiết bị.

Nguyên nhân lựa chọn vòng bạc đạn là vì chúng thường xuyên phải chịu tải trọng và ma sát, dẫn đến mài mòn và hư hỏng theo thời gian. Thông qua tìm kiếm tài liệu và các nguồn dữ liệu công khai thì nguồn dữ liệu âm thanh về vòng bạc đạn là phổ biến và dễ tiếp cận, giúp việc thu thập dữ liệu và phát triển mô hình dự đoán trở nên khả thi hơn.

### Input

Đầu vào của hệ thống là khối dữ liệu âm thanh của vòng bạc đạn. Dữ liệu này được thu thập hoặc truyền vào thông qua hai nguồn chính sau:

- Dataset âm thanh từ Laptop kết nối với ESP32 sử dụng jack âm thanh 3.5mm, giúp thu thập dữ liệu trực tiếp từ vòng bạc đạn trong quá trình hoạt động thực tế. Nguồn âm thanh này lấy trực tiếp từ các dataset công khai về vòng bạc đạn, giúp đảm bảo tính đa dạng và đại diện của dữ liệu cho việc huấn luyện mô hình dự đoán.
- Nguồn âm thanh trực tiếp từ microphone kết nối với ESP32, microphone này sẽ thu thập lại quá trình mà vòng bạc đạn hoạt động và phát ra âm thanh, giúp thu thập dữ liệu thực tế và đa dạng hơn về các tình trạng khác nhau của vòng bạc đạn.

### Output

Đầu ra của hệ thống là dự đoán về tình trạng của vòng bạc đạn dựa trên dữ liệu âm thanh đã được xử lý và phân tích. Dự đoán này có thể bao gồm các thông tin như:

- Mẫu âm thanh được lưu trữ trên vùng nhớ hệ thống.
- Phổ âm thanh đã được phân tích và trích xuất các đặc trưng quan trọng.
- Dự đoán về tình trạng của vòng bạc đạn, ví dụ như "bình thường", "có dấu hiệu hư hỏng", hoặc "hư hỏng nghiêm trọng". Dự đoán này sẽ giúp người dùng hoặc hệ thống tự động có thể đưa ra các biện pháp bảo trì hoặc thay thế kịp thời để tránh sự cố và tăng tuổi thọ của thiết bị.

## Functional Requirements

### Performance

Hệ thống được kỳ vọng sẽ hoạt động ở các thông số sau:

- Tốc độ lấy mẫu là 96kHz để đảm bảo chất lượng âm thanh đủ cao để phân tích và dự đoán chính xác tình trạng của vòng bạc đạn.
- Vòng bạc đạn được tham chiếu mẫu ở tốc độ 2000 RPM dưới tải trọng hướng tâm 6000 lbs theo dataset âm thanh của NASA.
- Độ chính xác của mô hình dự đoán được kỳ vọng đạt trên 90% để đảm bảo rằng hệ thống có thể phân biệt chính xác giữa các tình trạng khác nhau của vòng bạc đạn và cung cấp thông tin hữu ích cho việc bảo trì và quản lý thiết bị.
- Tính di động cao và nhỏ gọn với việc sử dụng pin có thể sạc lại, giúp hệ thống có thể được sử dụng trong nhiều môi trường khác nhau và dễ dàng di chuyển đến các vị trí cần giám sát vòng bạc đạn.

### Operation Mode

Hệ thống sẽ hoạt động ở hai chế độ chính:

- Chế độ hiệu chỉnh - Caliber Mode: Trong chế độ này, hệ thống sẽ được hiệu chỉnh để đảm bảo rằng các cảm biến và thiết bị thu thập dữ liệu hoạt động chính xác và cung cấp dữ liệu đáng tin cậy. Quá trình hiệu chỉnh có thể bao gồm việc điều chỉnh các tham số của cảm biến, kiểm tra và xác nhận chất lượng dữ liệu thu thập được, và đảm bảo rằng hệ thống sẵn sàng để hoạt động trong môi trường thực tế.
- Chế độ hoạt động - Operation Mode: Trong chế độ này, hệ thống sẽ thu thập dữ liệu âm thanh từ vòng bạc đạn, xử lý và phân tích dữ liệu để đưa ra dự đoán về tình trạng của vòng bạc đạn. Hệ thống sẽ hoạt động liên tục để giám sát tình trạng của vòng bạc đạn và cung cấp thông tin kịp thời về các vấn đề tiềm ẩn hoặc sự cố có thể xảy ra, giúp người dùng hoặc hệ thống tự động có thể đưa ra các biện pháp bảo trì hoặc thay thế kịp thời để tránh sự cố và tăng tuổi thọ của thiết bị.

## Non-Functional Requirements

### Environmental

Hệ thống được kỳ vọng hoạt động ở môi trường có độ ồn nền lên đến 80dB, điều này có thể ảnh hưởng đến chất lượng dữ liệu âm thanh thu thập được và độ chính xác của mô hình dự đoán.

Do đó, hệ thống cần được thiết kế để có khả năng xử lý và phân tích dữ liệu âm thanh trong môi trường có độ ồn cao, có thể bao gồm việc sử dụng các kỹ thuật lọc và xử lý tín hiệu để giảm thiểu ảnh hưởng của tiếng ồn nền và cải thiện độ chính xác của dự đoán về tình trạng của vòng bạc đạn.

### Power

Hệ thống được kỳ vọng hoạt động tối thiểu 8 tiếng liên tục với một lần sạc đầy pin, điều này yêu cầu hệ thống phải được thiết kế để tiêu thụ năng lượng hiệu quả và sử dụng các thành phần có mức tiêu thụ năng lượng thấp.

Hệ thống cũng cần được thiết kế để có thể chỉ báo được tính trạng pin và trạng thái sạc.

Điều này có thể bao gồm việc sử dụng các cảm biến hoặc mạch điện để theo dõi mức pin và trạng thái sạc, và cung cấp thông tin này cho người dùng hoặc hệ thống tự động để đảm bảo rằng hệ thống luôn có đủ năng lượng để hoạt động hiệu quả trong môi trường thực tế.

## Constraints

- Hệ thống được ràng buộc các tham chiếu theo dataset âm thanh của NASA nhằm đảm bảo rằng dữ liệu thu thập được có chất lượng và tính đại diện cao cho việc huấn luyện mô hình dự đoán về tình trạng của vòng bạc đạn.
- Hệ thống được ràng buộc bởi các giới hạn về phần cứng và tài nguyên, bao gồm khả năng xử lý của vi điều khiển, dung lượng lưu trữ, và mức tiêu thụ năng lượng, điều này yêu cầu hệ thống phải được thiết kế để hoạt động hiệu quả trong các giới hạn này và đảm bảo rằng nó có thể cung cấp dự đoán chính xác về tình trạng của vòng bạc đạn trong môi trường thực tế.
