
#define ARDUINOJSON_ENABLE_PROGMEM 0
// This #include statement was automatically added by the Particle IDE.
#include <HttpClient.h>

// This #include statement was automatically added by the Particle IDE.
#include <ArduinoJson.h>

// This #include statement was automatically added by the Particle IDE.
#include <Serial_LCD_SparkFun.h>
#include <Wire.h>

using namespace std;

Serial_LCD_SparkFun lcd = Serial_LCD_SparkFun();
#define PIN_AUDIO_IN A0
#define PIN_ENVELOPE_IN A1
#define PIN_GATE_IN D2
#define DISPLAY_ADDRESS1 0x72

int cycles = 0;
int Audio_In_Value;
int Envelope_In_Value;
int Gate_In_Value;

//Serial_LCD_SparkFun lcd = Serial_LCD_SparkFun();
HttpClient http;

//weather
http_request_t request1;
http_response_t response1;

//time
http_request_t request2;
http_response_t response2;

//currency
http_request_t request3;
http_response_t response3;
//weather
const size_t capacity1 = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) +  2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 280;
DynamicJsonDocument dsj1(capacity1);
//time
const size_t capacity2 = JSON_OBJECT_SIZE(2) + 100*JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(100) + 11890; //api for the stock function 
DynamicJsonDocument dsj2(capacity2);
//stock
const size_t capacity3 = JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(168) + 810;
DynamicJsonDocument dsj3(capacity3);

String key = "976ae3822d5e3cc2116c8d3aa8afc6c3";
String city = "5206379";



void setup()
{
    Wire.begin();
    Serial.begin(9600);
    pinMode(PIN_GATE_IN, INPUT);
    lcd.setBrightness(30);
    lcd.clear();
    lcd.home();
    
    
    
    
}

void loop() 
{
    Audio_In_Value = analogRead(PIN_AUDIO_IN);
    Envelope_In_Value = analogRead(PIN_ENVELOPE_IN);
    Gate_In_Value = digitalRead(PIN_GATE_IN);
    delay(50);
    getCurrency();
    
    if(Envelope_In_Value>200)
     {
         i2cSendValue();
     }
   

    delay(50); //The maximum update rate of OpenLCD is about 100Hz (10ms). A smaller delay will cause flicker
   
    
}
void i2cSendValue()
{
  
  int tem = getWeather();
  String date  = getTime();
  lcd.clear();
  lcd.home();
  delay(50);
  String temp = "Temp: ";
  Serial1.print(temp);
  Serial1.print(tem);
  Serial1.println("F");
  delay(3000);
  lcd.clear();
  lcd.home();
  Serial1.print("Time: ");
  Serial1.print(date);
  
  delay(3000);
  lcd.clear();
  lcd.home();
  Serial1.print("--- 1 USD = ");
  delay(1000);
  lcd.clear();
  lcd.home();
  Serial1.print(0.89);
  Serial1.print(" EUR");
  delay(3000);
  lcd.clear();
  
  
  
  

  
}
http_header_t headers1[] = {
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_header_t headers2[] = {
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_header_t headers3[] = {
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
int getWeather()
{
    request1.hostname = "api.openweathermap.org";
    request1.port = 80;
    request1.path = "/data/2.5/weather?id=" + city + "&appid=" + key;
    http.get(request1, response1, headers1);
    string jsonString1 = string(response1.body);
    deserializeJson(dsj1, jsonString1);
    JsonObject w = dsj1["weather"][0];
    JsonObject m = dsj1["main"];
    const char* data = w["description"];
    
    int temp = m["temp"];
    float hum = m["humidity"];
    temp = (temp - 273.15) * 9/5 + 32;
    return temp; 

}
String getTime()
{
    request2.hostname = "worldclockapi.com";
    request2.port = 80;
    request2.path = "/api/json/est/now";
    http.get(request2, response2, headers2);
    
    string jsonString = string(response2.body);
    deserializeJson(dsj2,jsonString);
    const char* t = dsj2["currentDateTime"];
    String DateTime = String(t);
  
    int index = DateTime.indexOf('T');
    String date = DateTime.substring(0,index);
    String tim = DateTime.substring(index+1, 16);
    return tim;
    
}
double getCurrency()
{
    request3.hostname = "http://data.fixer.io";
    request3.port = 80;
    request3.path = "/api/latest?access_key=9da0bbfbcdeab1ca26ab70316c1fb2cb&format=1";
    http.get(request3, response3, headers3);
    //Serial.println(response3.body);
    string jsonString3 = string(response3.body);
    deserializeJson(dsj3, jsonString3);
    JsonObject z = dsj3["rates"];
    double EuroVal =  z["EUR"];
    //double InrVal = z["INR"];
    return EuroVal;
    
    
    
    
}
