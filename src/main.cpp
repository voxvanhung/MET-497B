#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h> // <--- Thêm thư viện bảo mật này
const char* ssid = "DTU";     // <--- 1. SỬA WIFI
const char* password = "";    // <--- 2. SỬA PASS

// <--- 3. DÁN LINK GOOGLE SCRIPT CỦA BẠN VÀO GIỮA 2 DẤU NGOẶC KÉP DƯỚI ĐÂY:
String serverName = "https://script.google.com/macros/s/AKfycbzO_nDiiTH8Mg4bFWC5RLFpANdx708knElh46bsTN5vTM_1C2SsHsWUS9SQHGmCNEJj/exec";

// Chân đèn LED (Thường là chân 2 trên ESP32)
const int ledPin = 2; 

unsigned long lastTime = 0;
unsigned long timerDelay = 5000; // Kiểm tra 5 giây 1 lần

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Dang ket noi Wifi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nDa ket noi Wifi xong!");
}

void loop() {
  // Cứ sau 5 giây thì chạy kiểm tra 1 lần
  if ((millis() - lastTime) > timerDelay) {
    
    if(WiFi.status() == WL_CONNECTED) {
      
      // 1. Tạo một "Anh bảo vệ" riêng chuyên lo việc HTTPS
      WiFiClientSecure client;
      client.setInsecure(); // Ra lệnh cho anh này: "Cứ cho qua, không cần kiểm tra chứng chỉ"

      HTTPClient http;

      // 2. Dùng anh bảo vệ này để kết nối
      // (Lưu ý: Phải truyền biến 'client' vào hàm begin)
      http.begin(client, serverName); 
      
      // --- (Bỏ dòng http.setInsecure() cũ đi) ---
      
      http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
      
      int httpResponseCode = http.GET();
      
      // ... (Phần còn lại giữ nguyên như cũ) ...
      if (httpResponseCode > 0) {
        String payload = http.getString();
        Serial.println("\n--- DU LIEU TU GOOGLE ---");
        Serial.println(payload);
        
        if (payload.indexOf("\"bank_money\":1") > 0) {
          digitalWrite(ledPin, HIGH);
          Serial.println("=> CO TIEN -> BAT DEN");
        }
        else {
          digitalWrite(ledPin, LOW);
          Serial.println("=> KHONG CO TIEN -> TAT DEN");
        }
      }
      else {
        Serial.print("Loi ket noi: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    lastTime = millis();
  }
}