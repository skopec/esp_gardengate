#include <ESP8266WiFi.h>

#define INP 12

//unsigned long t;
unsigned int stav;
unsigned long cas;

// WiFi parameters
#define WiFiNumber 4
#define Attempts   5
const char* ssid[] = {"AndroidAP", "HostDoDomu", "WCisFi", "Internet"};
const char* password[] = {"jzrl6578", "HulDoRuky", "Pi:3.14159265359", "PRiU2N24j6"};
 
// replace with your channel's thingspeak API key, 
String apiKey = "4JCJLTXKEHIP12TO"; // "556JKEK75IV9A2M8";

//const char* server = "api.thingspeak.com";
const char* server = "pico.skopec.cz";

WiFiClient client;

int ctiStav(void) { 
  //
  // vrati stav vstupniho kontaktu (trva do 200ms)
  // 0=zavreno, 1=otevreno
  //
  unsigned int i,j,k;
  do {
    i=digitalRead(INP);
    delay(50);
    j=digitalRead(INP);
    delay(50);
    k=digitalRead(INP);
    delay(50);
  } while(!((i==j)&&(j==k)));
  return i;
}

void setup() {
  int i,j;
  Serial.begin(9600);
  Serial.println(); 
  // Connect to WiFi
  while (WiFi.status()!=WL_CONNECTED){
    for(i=0;i<WiFiNumber&&WiFi.status()!=WL_CONNECTED;++i) {
      Serial.print("Connecting ");
      Serial.println(ssid[i]);
      WiFi.begin(ssid[i], password[i]);
      for(j=0;(j<Attempts)&&(WiFi.status()!=WL_CONNECTED);++j) {
        delay(1000);
        Serial.print(".");
        //Serial.println(ssid[i]);
        //Serial.println(WiFi.status());
      }  
    }
  }  
  Serial.println("");
  Serial.println("WiFi connected");
    
  // Print the IP address
  Serial.println(WiFi.localIP());
  //
  //  
  //
  pinMode(INP, INPUT);
  
    stav = -1; // -> urcite bude zmena
}


// the loop function runs over and over again forever
void loop() {
  unsigned int s;
    s=ctiStav();
    if(stav!=s) {   // doslo ke zmene stavu
      stav=s;
      if (client.connect(server,31415)) {  // server, 80  "184.106.153.149" or api.thingspeak.com
        if(stav==1) { // doslo k otevreni
          client.print("OTE ");
          client.print((millis()-cas)/1000);
          cas = millis();
          client.print("\n");            
        } else {      // doslo k zavreni
          client.print("ZAV ");
          client.print((millis()-cas)/1000);
          cas = millis();
          client.print("\n");
        }
        client.stop();  
      }
    }  
    delay(800); // dorovnej cas asi do 1 sekundy
}

