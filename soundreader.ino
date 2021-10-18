#define ARDUINOJSON_ENABLE_PROGMEM 0
// This #include statement was automatically added by the Particle IDE.
#include <HttpClient.h>
#include <Wire.h>
// This #include statement was automatically added by the Particle IDE.
#include <ArduinoJson.h>

using namespace std;

#define PIN_AUDIO_IN A0
#define PIN_ENVELOPE_IN A1
#define PIN_GATE_IN D2
#define DISPLAY_ADDRESS1 0x72



HttpClient cheese;
http_request_t request2;
http_response_t response2;
const size_t capacity2 = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) +  2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 280;
                
DynamicJsonDocument dsj2(capacity2);
String key = "976ae3822d5e3cc2116c8d3aa8afc6c3";
String city = "5206379"; //Pittsburg area
int Audio_In_Value;
int Envelope_In_Value;
int Gate_In_Value;




void setup()
{
    Wire.begin();
    Serial.begin(9600);
    pinMode(PIN_GATE_IN, INPUT);
    Wire.beginTransmission(DISPLAY_ADDRESS1);
    Wire.write('|'); //Put LCD into setting mode
    Wire.write('-'); //Send clear display command
    Wire.endTransmission();
}

void loop() 
{
    //Send the four characters to the display
    delay(50); //The maximum update rate of OpenLCD is about 100Hz (10ms). A smaller delay will cause flicker
    Audio_In_Value = analogRead(PIN_AUDIO_IN);
    Envelope_In_Value = analogRead(PIN_ENVELOPE_IN);
    Gate_In_Value = digitalRead(PIN_GATE_IN);
    delay(100);
    Serial.println(Envelope_In_Value);
    // if(Envelope_In_Value>300)
    //  {
    //      i2cSendValue();
    //  }
    
    
    
}
void i2cSendValue()
{
  Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1

  Wire.write('|'); //Put LCD into setting mode
  Wire.write('-'); //Send clear display command
  int temperature = getWeather();
  Wire.print("Temperature is: ");
  Wire.println(temperature);
  Wire.println(" °F");
  delay(2000);
  Wire.write('|');
  Wire.write('-');
  

  Wire.endTransmission(); //Stop I2C transmission
}

http_header_t headers[] = {
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

int getWeather()
{
    request2.hostname = "api.openweathermap.org";
    request2.port = 80;
    request2.path = "/data/2.5/weather?id=" + city + "&appid=" + key;
    cheese.get(request2, response2, headers);
    string jsonString2 = string(response2.body);
    deserializeJson(dsj2, jsonString2);
    JsonObject w = dsj2["weather"][0];
    JsonObject m = dsj2["main"];
    const char* data = w["description"];
    
    int temp = m["temp"];
    
    temp = (temp - 273.15) * 9/5 + 32;
    return temp;
    
}