// Logic Finite State Machine - Fall Detection (Trích lược từ dự án Smart Cane)
void detectFall() {
  // Lấy dữ liệu sensor qua bộ lọc Moving Average
  float totalAccel = accelFilter.update(rawAccel);
  float tiltAngle = calculateTiltAngle();
  unsigned long now = millis();

  switch (fallState) {
    case FALL_IDLE: // Chờ phát hiện rơi tự do
      if (totalAccel < FREEFALL_THRESHOLD) {
        fallState = FALL_FREEFALL;
        fallStateStartTime = now;
      }
      break;

    case FALL_FREEFALL: // Kiểm tra va chạm ngay sau khi rơi
      if (totalAccel > IMPACT_THRESHOLD) {
        fallState = FALL_IMPACT;
      } else if ((now - fallStateStartTime) > 500) {
        fallState = FALL_IDLE; // Timeout nếu không có va chạm
      }
      break;

    case FALL_IMPACT: // Kiểm tra góc nghiêng của gậy sau va chạm
      if (tiltAngle > FALL_ANGLE_THRESHOLD) {
        fallState = FALL_PRE_ALERT;
        fallStateStartTime = now;
        currentPattern = BUZZER_FALL_PRE_ALERT; // Bíp cảnh báo người dùng
      } else {
        fallState = FALL_IDLE;
      }
      break;

    case FALL_PRE_ALERT: // Giai đoạn 5 giây cho phép người dùng hủy cảnh báo
      if (userPressedCancel()) {
        fallState = FALL_IDLE;
        currentPattern = BUZZER_OFF;
      } else if ((now - fallStateStartTime) > 5000) { // Hết 5s không hủy
        fallState = FALL_WAIT_CONFIRM;
        fallStateStartTime = now;
      }
      break;

    case FALL_WAIT_CONFIRM: // Xác nhận người dùng có bất động hay không
      if ((now - fallStateStartTime) > 2000) {
        if (abs(totalAccel - 1.0) < 0.3) { // Nếu nằm im 2s
          fallState = FALL_CONFIRMED;
          triggerEmergencySystems(); // Kích hoạt SMS/Call/Server Alert
        } else {
          fallState = FALL_IDLE; // Đã đứng dậy thành công
        }
      }
      break;
  }
}
