# Fall-Detection-FSM
Máy trạng thái (Finite State Machine - FSM) cho tính năng phát hiện té ngã
Đoạn code giải quyết bài toán cốt lõi trong các thiết bị đeo (wearable devices) là: Làm sao để phát hiện té ngã chính xác, thời gian thực mà không gây báo động giả (false alarm).
- Chức năng chính
  +Bộ lọc tín hiệu số (Digital Filtering): Áp dụng thuật toán Moving Average Filter (lọc trung bình động) để loại bỏ nhiễu trắng và các xung gia tốc đột ngột không phải do té ngã.

  +Máy trạng thái hữu hạn (FSM): Chia quá trình té ngã thành 5 giai đoạn logic (Idle -> Freefall -> Impact -> Pre-alert -> Confirmed). Việc sử dụng FSM giúp code không bị rối (spaghetti code) và dễ dàng mở rộng tính năng.

  +Xử lý đa nhiệm không chặn (Non-blocking Multitasking): Tận dụng FreeRTOS để tách biệt luồng xử lý cảm biến (ưu tiên cao) và luồng truyền thông (WiFi/SIM/GPS). Điều này đảm bảo hệ thống không bị treo khi đang gửi dữ liệu lên Server.
- Ngoài ra còn áp dụng
  + Cơ chế Pre-alert (5 giây vàng): . Khi phát hiện va chạm và góc nghiêng bất thường, hệ thống chưa gửi cảnh báo ngay mà bật còi bíp nhanh trong 5 giây. Nếu người dùng vẫn tỉnh táo, họ có thể nhấn nút để hủy bỏ, giúp giảm tối đa báo động giả.
  + Xác nhận bất động (Wait Confirm): Sau va chạm, hệ thống tiếp tục theo dõi gia tốc trong 2 giây. Chỉ khi người dùng thực sự nằm im (gia tốc xấp xỉ $1g$) thì lệnh cứu hộ mới được kích hoạt.
