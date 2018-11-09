#include <ArduinoJson.h>        
#include <ESP8266WiFi.h>
/*se define los pines a utilizar llevando en cuenta que los puertos 
del nodemcu no son los mismos que del integrado esp8266*/

#define D0   16 //GPIO16 - WAKE UP    perim
#define D1   5  //GPIO5               inter
#define D2   4  //GPIO14              par_r
#define D5   14 //GPIO14 - HSCLK      par_g
#define D6   12 //GPIO12 - HMISO      par_b

int perim;
int inter;
int park;
int par_r;
int par_g;
int par_b;

WiFiClient client;

// nombre de la redwifi
const char* ssid     = "MaquetaApp";
//password wifi
const char* password = "12345678";

// Name address 
const char* server = "127.0.0.1:3000";

// Replace with your unique URL resource
const char* resource = "luces/estatus";


const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response

// los tipos de datos que se extraen 
struct clientData {
  int perim[1];
  int inter[1];
  int park[1];
  int par_r[3];
  int par_g[3];
  int par_b[3];
  
};


void setup() {
  //se configura la velocidad de conexion a 115200 baudios
    Serial.begin(115200);
  delay(10);
  //se configura los pines a utilizar como salida
  pinMode(D0,OUTPUT);  //D0 perim
  pinMode(D1,OUTPUT);  //D1 inter
  pinMode(D2,OUTPUT);  //D2 park_r
  pinMode(D5,OUTPUT);  //D5 park_g
  pinMode(D6,OUTPUT);  //D6 park_b

  // se inicia la conexion wifi

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if(connect(server)) {
    if(sendRequest(server, resource) && skipResponseHeaders()) {
      clientData clientData;
      if(readReponseContent(&clientData)) {
        printclientData(&clientData);
      }
    }
  }
  disconnect();
  wait();
}

// Open connection to the HTTP server
bool connect(const char* hostName) {
  Serial.print("Connect to ");
  Serial.println(hostName);

  bool ok = client.connect(hostName, 80);

  Serial.println(ok ? "Connected" : "Connection Failed!");
  return ok;
}

// Send the HTTP GET request to the server
bool sendRequest(const char* host, const char* resource) {
  Serial.print("GET ");
  Serial.println(resource);

  client.print("GET ");
  client.print(resource);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();

  return true;
}

// Skip HTTP headers so that we are at the beginning of the response's body
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    Serial.println("No response or invalid response!");
  }
  return ok;
}



bool readReponseContent(struct clientData* clientData) {
  // Compute optimal size of the JSON buffer according to what we need to parse.
  // See https://bblanchon.github.io/ArduinoJson/assistant/
  const size_t bufferSize = JSON_OBJECT_SIZE(5)+50;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject & root = jsonBuffer.parseObject(client);

  if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }

  clientData->perim, root["perim"];
  clientData->inter, root["inter"];
  clientData->park, root["park"];
  clientData->par_r, root["par_r"];
  clientData->par_g, root["par_g"];
  clientData->par_b, root["par_b"];


  return true;
}


void printclientData(const struct clientData* clientData) {
  if (perim == HIGH){    // no estoy segura si en el programa devuelve 1 o HIGH es solo cuestion de modificar en caso contrario
    digitalWrite(D0,HIGH);
  }
  if (inter == HIGH){
    digitalWrite(D1,HIGH);
  }
  if(park == HIGH){
    digitalWrite(D2,par_r);    //luces RGB
    digitalWrite(D5,par_g);
    digitalWrite(D6,par_b);
  }
   if (perim == LOW){
    digitalWrite(D0,LOW);
  }
  if (inter == LOW){
    digitalWrite(D1,LOW);
  }
  if(park == LOW){
    digitalWrite(D2,0);
    digitalWrite(D5,0);
    digitalWrite(D6,0);
  } 

}

// Close the connection with the HTTP server
void disconnect() {
  Serial.println("Disconnect");
  client.stop();
}

// Pause for a 1 minute
void wait() {
  Serial.println("Wait 60 seconds");
  delay(60000);
}
