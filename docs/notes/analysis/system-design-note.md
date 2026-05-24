# Ghi chú về thiết kế hệ thống

## Giới thiệu

Ghi chú này tổng hợp các thiết kế hệ thống của dự án nhằm đảm bảo hoạt động ổn định và hiệu quả.

## Ý tưởng thiết kế

Thiết kế sử dụng sẽ lấy ý tưởng từ tài liệu [paper](../references/audio-data-acquisition-system-design-based-on-arm-and-dsp.pdf) nhằm đảm bảo tính ổn định và hiệu quả của hệ thống đã được kiểm chứng.

Đối với thiết kế hệ thống của tài liệu, nhóm tác giả sử dụng kết hợp giữa 1 vi điều khiển ARM với 1 DSP để xử lý âm thanh phức tạp.

Do đó, một câu hỏi đặt ra là ta có thể sử dụng một vi điều khiển duy nhất với 1 bộ thu thập dữ liệu sử dụng MAX9812, PCM1808, TL072IDT để thay thế cho thiết kế ban đầu hay không?

Trong quá trình thiết kế tài liệu, API và bản thân phần cứng đã cho phép hỗ trợ DMA tự động với I2S trên ESP32, điều này giúp giảm tải cho CPU và cải thiện hiệu suất xử lý âm thanh. Do đó, bản thân đề tài cũng sẽ có sự mở rộng về nội dung hướng tới để kiểm chứng khả năng vận dụng của hệ thống này trong công nghiệp như bảo trì tiên đoán - predictive maintenance, một ứng dụng phổ biến của việc thu thập dữ liệu âm thanh trong công nghiệp.

Tại sao lại chọn đề tài này ? Bởi vì bản thân âm thanh là dạng dữ liệu phức tạp và có khả năng khai thác như một nguồn thông tin trong kiểm chứng và bảo vệ thiết bị công nghiệp, đặc biệt là trong bảo trì tiên đoán. Việc thiết kế một hệ thống thu thập dữ liệu âm thanh hiệu quả sẽ mở ra nhiều cơ hội ứng dụng trong thực tế, từ việc giám sát tình trạng máy móc đến phát hiện sự cố sớm.

## Đánh giá về đề tài

Dựa theo các tài liệu đã thu thập, có thể tập trung vào các điểm sau:

### Về diễn trình lịch sử và các khảo sát nghiên cứu trước đây

Giai đoạn 2014 - 2015:

- Bắt đầu với nền móng cho fingerprint và thu thập dữ liệu chính xác
- Tài liệu [2014](../references/Do%20You%20Hear%20What%20I%20Hear%20-%20Fingerprinting%20Smart%20Devices%20Through.pdf) đã công bố các nghiên cứu liên quan về việc sử dụng các sai lệch nhỏ trong khâu thiết kế và đóng gói thiết bị để tạo ra một "dấu vân tay" duy nhất cho mỗi thiết bị, cho phép nhận diện thiết bị thông qua các đặc điểm âm thanh của nó với phương pháp MFCC trên đặc tính phổ.
- Tài liệu [2015](../references/Audio%20Data%20Acquisition%20System%20Design%20Based%20on%20ARM%20and%20DSP.pdf) đã trình bày một thiết kế hệ thống thu thập dữ liệu âm thanh dựa trên ARM và DSP, tập trung vào việc đảm bảo độ ổn định và hiệu quả trong việc thu thập dữ liệu âm thanh chất lượng cao ở độ phân giải 96kHz/24-bit.

Giai đoạn 2019 - 2022:

- Tập trung vào việc triển khai các giải pháp thu thập dữ liệu và tích hợp tính toán biên, ML mức nhúng.
- Tài liệu [2019](../references/Predictive%20maintenance%20with%20sensor%20data%20analytics%20on%20a%20Raspberry%20Pi-based%20experimental%20platform.pdf) đã sử dụng phân tích đa biến để dự báo trạng thái máy khử ẩm trên nền tảng Raspberry Pi + PIC18F4525 và khảo chứng sử dụng SAS để tìm ra các yếu tố quan trọng ảnh hưởng đến hiệu suất của máy khử ẩm, từ đó xây dựng mô hình dự báo hiệu quả.
- Tài liệu [2022](../references/Predictive%20Maintenance%20-%20An%20Embedded%20System%20Approach.pdf) đề xuất tiếp cận theo kiến trúc Mobile Edge Computing để triển khai giảm tải chi phí cho các doanh nghiệp SMEs.

Giai đoạn 2023 - 2025:

- Tập trung vào đánh giá lại các vấn đề cốt lõi của predictive maintenance và các giải pháp mới, đặc biệt là liên quan đến vấn đề XAI và Big Data.
- Tài liệu [2023-1](../references/Application%20of%20Embedded%20Systems%20in%20Industrial%20IoT%20for%20Predictive%20Maintenance.pdf) và [2023-2](../references/Challenges%20in%20predictive%20maintenance%20–%20A%20review.pdf) đã bổ sung các vấn đề của đề tài bên cạnh các giải pháp đã được đề xuất, tập trung vào lượng dữ liệu thu thập khi triển khai quá lớn (đặc biệt với chất lượng và tần số thu thập cao như 96kHz/24-bit), dẫn đến khó khăn trong việc xử lý và lưu trữ dữ liệu, cũng như các vấn đề về bảo mật và quyền riêng tư khi thu thập dữ liệu từ các thiết bị công nghiệp.
- Tài liệu [2024](../references/Explainable%20Predictive%20Maintenance%20-%20A%20Survey%20of%20Current%20Methods%20Challenges%20and%20Opportunities.pdf) làm rõ hơn về các vấn đề liên quan đến XAI trong predictive maintenance, đặc biệt là việc giải thích các mô hình dự báo và đảm bảo tính minh bạch trong quá trình ra quyết định dựa trên dữ liệu thu thập được.
- Tài liệu [2025](../references/Acoustic%20fingerprint%20in%20vehicle%20manufacturing%20as%20a%20basis%20for%20future%20applications.pdf) bổ sung thêm về quy trình lấy mẫu định kỳ chuẩn và phân tích dữ liệu nhằm giảm tải vấn đề thu thập dữ liệu dư thừa.

### Các vấn đề chưa được giải quyết

1. Dữ liệu nhiễu: Việc chúng ta thu thập dữ liệu nhưng không có cách nào để đảm bảo rằng dữ liệu đó không bị nhiễu hoặc có chất lượng kém, đặc biệt là trong môi trường công nghiệp nơi có nhiều nhiễu điện từ, nhiệt độ cao, và các yếu tố khác có thể ảnh hưởng đến chất lượng dữ liệu âm thanh thu thập được.
2. Xử lý dữ liệu lớn: Với chất lượng thu thập cao như 96kHz/24-bit, lượng dữ liệu thu thập được sẽ rất lớn, dẫn đến khó khăn trong việc xử lý và lưu trữ dữ liệu, đặc biệt là khi triển khai trên các thiết bị có tài nguyên hạn chế.
3. Thiếu tính tổng quát: Các giải pháp hiện tại có thể không đủ linh hoạt để áp dụng cho nhiều loại thiết bị công nghiệp khác nhau, đặc biệt là khi các thiết bị này có các đặc điểm âm thanh khác nhau và yêu cầu thu thập dữ liệu khác nhau.
4. Mất cân bằng dữ liệu: Trong quá trình thu thập dữ liệu, có thể xảy ra tình trạng mất cân bằng dữ liệu, nơi một số loại sự kiện hoặc trạng thái máy móc được thu thập nhiều hơn so với các loại khác, dẫn đến khó khăn trong việc xây dựng các mô hình dự báo chính xác.

## Thay đổi thiết kế tương ứng

Từ thiết kế ban đầu, đã có một số thay đổi để phù hợp với yêu cầu của dự án:

- Thay thế cấu trúc thiết kế hệ thống của bài báo bằng một thiết kế mới sử dụng ESP32-S3-DevKitC N16R8 làm vi điều khiển chính, kết hợp với các bộ thu thập dữ liệu như MAX9812, PCM1808, TL072IDT để xử lý âm thanh.
- Vận dụng các giao thức như SPI, I2S, GPIO, RCC, IWDG, I2C và DMA để đảm bảo hiệu quả trong việc thu thập và xử lý dữ liệu âm thanh tương tự (96kHz/24-bit) như trong thiết kế ban đầu.

Dựa theo các tài liệu thu thập, có thể mở rộng thiết kế để bổ sung các tính năng như:

- Quản lý quy trình htu thập dữ liệu định kỳ chuẩn để giảm tải vấn đề thu thập dữ liệu dư thừa, đặc biệt là trong môi trường công nghiệp nơi có nhiều thiết bị và sự kiện xảy ra liên tục.
- Tích hợp các giải pháp xử lý dữ liệu để đảm bảo chất lượng dữ liệu thu thập được, bao gồm các kỹ thuật lọc nhiễu và xử lý tín hiệu để loại bỏ các yếu tố tác động đến chất lượng dữ liệu âm thanh thu thập được.
- Phát triển các mô hình dự báo dựa trên dữ liệu thu thập được, đặc biệt là trong lĩnh vực bảo trì tiên đoán, để giúp các doanh nghiệp công nghiệp có thể dự đoán và ngăn ngừa sự cố trước khi chúng xảy ra.
- Quản lý tài nguyên hệ thống với tách biệt các tác vụ thu thập dữ liệu và xử lý dữ liệu để đảm bảo hiệu quả trong việc sử dụng tài nguyên, đặc biệt là khi triển khai trên các thiết bị có tài nguyên hạn chế.