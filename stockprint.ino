
#define ARDUINOJSON_ENABLE_PROGMEM 0

// This #include statement was automatically added by the Particle IDE.
#include <HttpClient.h>

// This #include statement was automatically added by the Particle IDE.
#include <ArduinoJson.h>
using namespace std;

const size_t capacity1 = JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(171) + 920;
DynamicJsonDocument dsj1(capacity1);

HttpClient http;
//stock function 
http_request_t request2;
http_response_t response2;



void setup()
{
Serial.begin(9600);
    Wire.begin();
}

void loop() 
{
    getStock();
}
http_header_t headers2[] = {
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
void getStock()
 {

         request2.hostname = "openexchangerates.org";
         request2.port = 80;
         request2.path = "/api/latest.json?app_id=40a3f519f410490e83df51e400269856";
         http.get(request2, response2, headers2);
        //  string jsonString2 = string(response2.body);
        //  deserializeJson(dsj1, jsonString2);
         Serial.println(response2.body);
        
        // String jsonString1 = string(response2.body);
        
        // deserializeJson(dsj1,jsonString1);
        //  JsonObject create1 = dsj1["companiesPriceList"];
        
        // int stock = create1["price"];
        // // const char* stocks = create1["2019-07-23 17:08:00"];
        
        // Serial.println(stock);
    
    
 }