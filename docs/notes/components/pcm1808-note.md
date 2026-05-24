# Ghi chú về PCM1808

## Giới thiệu

PCM1808 là một bộ chuyển đổi tương tự sang số (ADC) chất lượng cao với độ phân giải 24-bit. Nó được sử dụng rộng rãi trong các ứng dụng âm thanh để chuyển đổi tín hiệu âm thanh từ dạng tương tự sang dạng số, cho phép xử lý và lưu trữ âm thanh kỹ thuật số.

## Định dạng dữ liệu

Dựa trên tài liệu kỹ thuật của [PCM1808](../references/pcm1808-datasheet.pdf) trang 17 và trang tài liệu I2S của ESP-IDF.

Khi bit FORMAT = 0, PCM1808 sẽ xuất dữ liệu theo tiêu chuẩn Phillips đề cập tại [i2s-note](i2s-note.md)

Khi bit FORMAT = 1, PCM1808 sẽ xuất dữ liệu theo tiêu chuẩn MSB-Alignment đề cập tại [i2s-note](i2s-note.md)

## Thông tin tần số lấy mẫu và độ lệch khuếch đại

PCM1808 hỗ trợ tần số lấy mẫu từ 8kHz đến 96kHz và đầu vào tần số hệ thống dao động từ 2.048MHz đến 49.152MHz để đảm bảo hoạt động ổn định và chính xác.

Ngoài ra, độ khếch đại nằm ở mức +-3% trung bình và +-6% tối đa, đảm bảo chất lượng âm thanh tốt và độ chính xác cao trong quá trình chuyển đổi tín hiệu.

## Thiết kế PCB với PCM1808

Trình bày chi tiết trong [PCM1808](../references/pcm1808-datasheet.pdf) trang 19 về cách thiết kế PCB để đảm bảo hiệu suất tối ưu của PCM1808, bao gồm các lưu ý về bố trí mạch, cách đặt các thành phần và cách xử lý tín hiệu để giảm thiểu nhiễu và cải thiện chất lượng âm thanh.
