#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
WiFiClient wifiClient;
int redPin = 14;
int greenPin = 16;
int buzzer = 12;
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char* host = "192.168.1.150";
void setup() {
    WiFi.begin("dlink", "");
    Serial.begin(9600);
    pinMode(redPin, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(greenPin, OUTPUT);
    lcd.init();
    lcd.backlight();
}
void loop() {
    int rawData = analogRead(A0);
    float vcc = 5.0;
    float voltage = rawData * (vcc / 1024.0);
    float temperature = voltage * 100;
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(temperature);
    lcd.print(" *C");
    lcd.setCursor(0,1);
    lcd.print(temperature > 35 ? "Abnormal" : "Normal");
    lcd.print(" Temperature");
    
    String postData="";
    String my_device = "340722SPE0542022";
    postData = "device="+my_device+"&distance="+(String)temperature;

    if(temperature > 35){
        digitalWrite(greenPin,LOW);
        digitalWrite(redPin,HIGH);
        digitalWrite(buzzer, HIGH);   
        delay(100);
        sendData(80, "192.168.1.150", "/iot/" , postData); 
    delay(1000);
    }
    else{
      digitalWrite(greenPin,HIGH);
      digitalWrite(redPin,LOW);
      digitalWrite(buzzer, LOW);    
      delay(1000);
      }
   delay(500);
}
void sendData(const int httpPort, const char* host,const char* filepath , String data){
  
  wifiClient.connect(host, httpPort); 

  wifiClient.println("POST "+(String)filepath+" HTTP/1.1");
  wifiClient.println("Host: " + (String)host);
  wifiClient.println("User-Agent: ESP8266/1.0");
  wifiClient.println("Content-Type: application/x-www-form-urlencoded");
  wifiClient.println("Content-Length: " +(String)data.length());
  wifiClient.println();
  wifiClient.print(data);

  Serial.println("Response: " + wifiClient.readStringUntil('\n'));

}