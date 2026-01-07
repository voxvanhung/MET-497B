#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h> // <-- Thư viện giúp ESP32 "lướt web"

const char* ssid = "Tang 2";     // <--- SỬA LẠI WIFI
const char* password = "123456789";    // <--- SỬA LẠI PASS

// Địa chỉ trang web chúng ta muốn đọc (Giống như nhập địa chỉ vào Chrome)
const char* serverName = "http://jsonplaceholder.typicode.com/todos/1";

// Thời gian chờ giữa các lần đọc (5 giây)
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);

  // 1. Kết nối Wifi
  WiFi.begin(ssid, password);
  Serial.println("Dang ket noi Wifi...");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nDa ket noi Wifi thanh cong!");
  Serial.println("Bat dau doc du lieu tu web...");
}

void loop() {
  // Cứ sau 5 giây (5000ms) thì chạy đoạn này một lần
  if ((millis() - lastTime) > timerDelay) {
    
    // Kiểm tra xem Wifi còn sống không
    if(WiFi.status() == WL_CONNECTED) {
      
      HTTPClient http; // Khởi tạo "người đưa thư"

      // Bước 1: Nhập địa chỉ web
      http.begin(serverName); 
      
      // Bước 2: Gửi yêu cầu "GET" (Giống như bấm Enter trên trình duyệt)
      Serial.println("\nDang goi den Server...");
      int httpResponseCode = http.GET(); 

      // Bước 3: Kiểm tra xem Server có trả lời không
      if (httpResponseCode > 0) {
        // Mã 200 nghĩa là OK (Thành công)
        Serial.print("Server tra loi ma so: ");
        Serial.println(httpResponseCode);
        
        // Bước 4: Đọc nội dung Server gửi về
        String payload = http.getString(); 
        Serial.println("--- NOI DUNG NHAN DUOC ---");
        Serial.println(payload); // <-- Đây là cái chúng ta cần!
        Serial.println("--------------------------");
      }
      else {
        Serial.print("Loi roi, ma loi: ");
        Serial.println(httpResponseCode);
      }
      
      // Bước 5: Cúp máy (Giải phóng tài nguyên)
      http.end();
    }
    else {
      Serial.println("Mat ket noi Wifi!");
    }
    
    // Đặt lại đồng hồ đếm giờ
    lastTime = millis();
  }
}