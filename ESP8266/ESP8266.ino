#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
 
/*se define los pines a utilizar llevando en cuenta que los puertos 
del nodemcu no son los mismos que del integrado esp8266*/
 
#define D0   16 //GPIO16 - WAKE UP    perim
#define D1   5  //GPIO5               inter
#define D2   4  //GPIO14              par_r
#define D5   14 //GPIO14 - HSCLK      par_g
#define D6   12 //GPIO12 - HMISO      par_b
 
// WiFi Parameters
const char* ssid = "Nombre de la red";
const char* password = "Contraseña de la red";
 
void setup() {
  Serial.begin(115200);
 
  //se configura los pines a utilizar como salida
  pinMode(D0,OUTPUT);  //D0 perim
  pinMode(D1,OUTPUT);  //D1 inter
  pinMode(D2,OUTPUT);  //D2 park_r
  pinMode(D5,OUTPUT);  //D5 park_g
  pinMode(D6,OUTPUT);  //D6 park_b
 
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
}
 
void loop() {
  // Check WiFi Status
  Serial.println("Conectando...");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://plazaapp.herokuapp.com/luces/status");
    Serial.println("Obteniendo...");
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
      // Parsing
      const size_t bufferSize = JSON_OBJECT_SIZE(9) + 150;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      // Parameters
      Serial.println("Valores obtenidos:");
      int perim = root["perim"]; // 0
      int inter = root["inter"]; // 1
      int park = root["park"]; // 0
      int par_r = root["par_r"]; // 11
      int par_g = root["par_g"]; // 21
      int par_b = root["par_b"]; // 92
      // Output to serial monitor
      Serial.print("perim:");
      Serial.println(perim);
      Serial.print("inter:");
      Serial.println(inter);
      Serial.print("park:");
      Serial.println(park);
      Serial.print("par_r:");
      Serial.println(par_r);
      Serial.print("par_g:");
      Serial.println(par_g);
      Serial.print("par_b:");
      Serial.println(par_b);
 
      Serial.println("Trabajando sobre los leds...");
      if (perim == 1){
        analogWrite(D0,HIGH);
      }
      if (inter == 1){
        analogWrite(D1,HIGH);
      }
      if(park == 1){
        analogWrite(D2,par_r);    //luces RGB
        analogWrite(D5,par_g);
        analogWrite(D6,par_b);
      }
       if (perim == 0){
        analogWrite(D0,LOW);
      }
      if (inter == 0){
        analogWrite(D1,LOW);
      }
      if(park == 0){
        analogWrite(D2,0);
        analogWrite(D5,0);
        analogWrite(D6,0);
      }
      Serial.println("");
    }
    http.end();   //Close connection
  }
  // Delay
  delay(10000);
}
