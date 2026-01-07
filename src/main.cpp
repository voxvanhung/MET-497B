#include <Arduino.h>

#include <WiFi.h>

#include <WebServer.h>



const char* ssid = "Tang 2";

const char* password = "123456789";



const int ledPin = 2; // GPIO2, typically the built-in LED on ESP32 boards



WebServer server(80);



void handleRoot() {
  // Dùng R"rawliteral(...)rawliteral" để viết HTML thoải mái không cần nối chuỗi
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        /* Đây là phần CSS - Trang trí */
        body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #f0f0f0; }
        h1 { color: #333; }
        .button {
          display: inline-block;
          padding: 15px 35px;
          font-size: 24px;
          cursor: pointer;
          text-align: center;
          text-decoration: none;
          outline: none;
          color: #fff;
          border: none;
          border-radius: 15px;
          box-shadow: 0 5px #999;
          margin: 10px;
        }
        .button:active {
          background-color: #3e8e41;
          box-shadow: 0 2px #666;
          transform: translateY(4px);
        }
        .btn-on { background-color: #4CAF50; } /* Màu xanh lá */
        .btn-off { background-color: #f44336; } /* Màu đỏ */
      </style>
    </head>
    <body>
      <h1>ĐIỀU KHIỂN ĐÈN</h1>
      <p>Trạng thái hiện tại: Đang chờ lệnh...</p>
      
      <a href="/H" class="button btn-on">BẬT ĐÈN</a>
      <a href="/L" class="button btn-off">TẮT ĐÈN</a>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}



void handleLEDOn() {

  digitalWrite(ledPin, HIGH);

  server.send(200, "text/plain", "LED is ON");

}



void handleLEDOff() {

  digitalWrite(ledPin, LOW);

  server.send(200, "text/plain", "LED is OFF");

}



void setup() {

  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, LOW); // Ensure LED is off initially



  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);

    Serial.println("Connecting to WiFi...");

  }

  Serial.println("Connected to WiFi");

  Serial.print("IP Address: ");

  Serial.println(WiFi.localIP());



  server.on("/", handleRoot);

  server.on("/H", handleLEDOn);

  server.on("/L", handleLEDOff);



  server.begin();

  Serial.println("HTTP server started");

}



void loop() {

  server.handleClient();

}