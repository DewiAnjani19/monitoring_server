#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include "DHT.h"
#include <IRsend.h>

#define Threshold 200 //mq2

DHT dht1, dht2, dht3;
float WA;
char* msg;

float ga1, ga2, ga3;
int s0 = D5;
int s1 = D6;
int s2 = D7;

//Mux in "Z" pin
int Z_pin = A0;

IRsend irsend(D3);  // Set the GPIO to be used to sending the message.

HTTPClient http; 
String request_string;
//const char *host1 = "10.32.8.78";
const char *host1 = "iot.itk.ac.id";
char *ac = "hidup";
const int httpsPort = 443;

uint16_t rawDataOn[439] = {3572, 1660,  496, 370,  522, 1188,  496, 382,  510, 376,  520, 374,  520, 372,  520, 372,  520, 382,  512, 376,  518, 372,  520, 372,  520, 382,  514, 374,  520, 1186,  496, 398,  494, 400,  494, 378,  514, 398,  496, 398,  496, 374,  522, 376,  518, 1186,  496, 1188,  494, 1190,  492, 400,  496, 376,  518, 1188,  494, 380,  514, 384,  510, 398,  496, 382,  510, 400,  496, 398,  496, 376,  516, 398,  494, 400,  494, 400,  496, 398,  496, 398,  494, 384,  510, 400,  494, 398,  492, 400,  494, 400,  496, 398,  494, 382,  510, 400,  470, 424,  494, 398,  472, 422,  496, 382,  488, 422,  494, 384,  486, 424,  470, 424,  470, 424,  470, 424,  468, 1214,  470, 1212,  470, 424,  468, 426,  468, 426,  468, 426,  468, 424,  468, 9948,  3546, 1686,  468, 426,  468, 1214,  470, 424,  468, 426,  466, 428,  466, 428,  466, 426,  468, 426,  468, 426,  466, 426,  466, 428,  466, 428,  466, 428,  464, 1218,  464, 430,  466, 428,  466, 428,  464, 428,  464, 430,  438, 456,  466, 428,  464, 1218,  464, 1220,  462, 1222,  460, 432,  462, 432,  462, 1224,  458, 434,  462, 432,  458, 436,  458, 434,  458, 436,  458, 436,  456, 438,  456, 438,  456, 438,  456, 438,  452, 442,  452, 442,  452, 442,  452, 1230,  428, 466,  450, 444,  448, 446,  450, 1232,  422, 1260,  426, 468,  424, 468,  424, 470,  424, 470,  422, 472,  422, 1260,  422, 472,  422, 1260,  422, 472,  422, 472,  422, 472,  420, 474,  422, 474,  420, 472,  418, 476,  420, 472,  418, 476,  418, 1266,  420, 1264,  418, 474,  418, 476,  418, 474,  418, 478,  416, 1266,  416, 478,  414, 1288,  394, 500,  396, 498,  394, 500,  396, 498,  396, 498,  394, 498,  394, 500,  392, 500,  394, 500,  394, 500,  394, 500,  394, 500,  392, 502,  392, 502,  392, 502,  392, 504,  390, 504,  392, 1292,  390, 1292,  388, 506,  390, 504,  388, 504,  390, 506,  388, 506,  388, 506,  386, 508,  386, 508,  384, 512,  384, 534,  360, 1324,  358, 1324,  358, 534,  358, 534,  358, 536,  358, 536,  360, 534,  360, 534,  358, 536,  358, 536,  358, 534,  358, 536,  360, 534,  358, 534,  358, 536,  358, 536,  358, 536,  360, 534,  358, 534,  358, 536,  360, 534,  358, 536,  358, 534,  360, 534,  358, 536,  356, 536,  358, 1326,  356, 536,  358, 536,  356, 536,  358, 534,  358, 536,  356, 536,  358, 536,  358, 536,  358, 536,  356, 1328,  358, 1324,  356, 538,  358, 536,  358, 536,  356, 538,  358, 536,  358, 536,  358, 538,  358, 1324,  356, 1326,  358, 536,  358, 1326,  356, 1326,  358, 536,  356};  // PANASONIC_AC
uint16_t rawDataOff[439] = {3558, 1674,  482, 410,  482, 1200,  460, 434,  482, 412,  458, 436,  484, 410,  462, 430,  458, 434,  482, 412,  482, 410,  458, 436,  458, 436,  480, 414,  482, 1200,  482, 412,  482, 412,  456, 438,  460, 434,  482, 412,  460, 434,  460, 434,  482, 1200,  458, 1226,  458, 1224,  458, 436,  480, 414,  482, 1202,  482, 412,  460, 434,  460, 434,  456, 438,  458, 436,  484, 410,  478, 414,  456, 438,  458, 434,  482, 412,  458, 434,  456, 438,  482, 412,  482, 412,  464, 394,  494, 434,  456, 438,  480, 412,  466, 428,  456, 436,  480, 416,  472, 420,  456, 438,  458, 434,  482, 412,  458, 434,  482, 412,  482, 410,  460, 434,  456, 438,  458, 1224,  470, 1212,  484, 410,  482, 410,  458, 436,  460, 434,  460, 434,  460, 9958,  3538, 1696,  470, 418,  464, 1224,  458, 438,  458, 434,  458, 436,  454, 438,  482, 412,  458, 436,  458, 436,  462, 432,  458, 436,  456, 436,  458, 436,  460, 1224,  458, 436,  458, 436,  456, 438,  458, 434,  456, 438,  456, 438,  458, 434,  458, 1224,  456, 1226,  456, 1226,  456, 438,  458, 436,  456, 1226,  456, 438,  458, 436,  458, 436,  456, 438,  456, 436,  456, 438,  458, 436,  458, 438,  454, 438,  456, 436,  458, 436,  456, 438,  456, 436,  458, 438,  454, 438,  456, 438,  456, 438,  454, 1228,  456, 1226,  454, 438,  456, 438,  456, 438,  456, 438,  456, 436,  456, 1228,  454, 440,  454, 1228,  454, 440,  454, 440,  454, 440,  452, 442,  452, 468,  428, 442,  452, 466,  428, 466,  426, 466,  426, 1232,  452, 1254,  428, 466,  428, 466,  426, 468,  422, 472,  426, 1232,  450, 468,  424, 1232,  450, 470,  424, 468,  424, 470,  426, 468,  422, 472,  422, 470,  422, 470,  424, 448,  446, 448,  446, 448,  444, 448,  444, 448,  446, 446,  446, 448,  446, 448,  448, 446,  448, 446,  446, 1238,  448, 1234,  450, 444,  448, 444,  450, 442,  450, 444,  448, 446,  450, 444,  450, 442,  452, 442,  450, 442,  450, 444,  448, 1234,  448, 1236,  446, 446,  448, 446,  446, 450,  442, 450,  446, 448,  446, 450,  444, 470,  424, 468,  424, 468,  424, 470,  422, 472,  422, 472,  422, 470,  412, 482,  396, 498,  420, 472,  420, 474,  418, 474,  394, 500,  394, 500,  394, 500,  394, 498,  394, 500,  394, 500,  394, 1290,  392, 502,  390, 502,  390, 504,  392, 502,  392, 502,  392, 502,  390, 504,  390, 504,  388, 506,  388, 1296,  386, 1296,  386, 532,  362, 532,  360, 534,  360, 534,  360, 534,  360, 1322,  358, 1324,  358, 536,  360, 1322,  360, 534,  360, 1324,  358, 1324,  360, 534,  360};  // PANASONIC_AC
// Example DAIKIN of data captured by IRrecvDumpV2.ino
// uint16_t rawDataOn[133] = {4544, 2452,  414, 316,  420, 900,  416, 900,  414, 316,  420, 900,  414, 316,  420, 316,  420, 316,  418, 316,  420, 900,  416, 316,  418, 318,  418, 902,  412, 318,  418, 318,  418, 316,  420, 316,  418, 318,  418, 316,  420, 318,  418, 318,  420, 318,  418, 316,  420, 316,  418, 318,  418, 318,  418, 318,  418, 318,  418, 320,  416, 318,  418, 318,  418, 318,  418, 318,  420, 318,  418, 318,  418, 318,  416, 902,  412, 318,  416, 320,  416, 318,  416, 320,  418, 318,  418, 318,  416, 320,  418, 902,  410, 322,  416, 318,  418, 318,  418, 902,  414, 318,  416, 904,  412, 318,  416, 320,  416, 904,  412, 320,  416, 320,  416, 904,  410, 320,  416, 318,  416, 930,  384, 318,  442, 280,  454, 904,  408, 906,  408, 19928,  4540};  // UNKNOWN 26648D47
// uint16_t rawDataOff[133] = {4470, 2524,  344, 376,  360, 956,  358, 954,  360, 376,  360, 976,  338, 376,  360, 376,  360, 376,  360, 376,  360, 976,  338, 376,  360, 376,  360, 976,  338, 376,  360, 376,  360, 376,  358, 376,  360, 376,  360, 398,  338, 376,  360, 376,  358, 378,  358, 376,  360, 376,  358, 378,  358, 398,  338, 398,  338, 398,  338, 376,  358, 378,  358, 398,  338, 398,  338, 396,  338, 396,  340, 396,  338, 398,  338, 978,  336, 398,  338, 376,  358, 398,  338, 398,  338, 376,  358, 376,  360, 396,  338, 980,  336, 376,  360, 376,  358, 376,  360, 376,  360, 974,  340, 974,  340, 376,  360, 978,  336, 376,  360, 396,  340, 376,  360, 974,  340, 376,  360, 396,  340, 978,  336, 376,  360, 374,  360, 972,  342, 974,  342, 20024,  4468};  // UNKNOWN C6ADE375

//mencari nilai MIN fuzzy
float Min(float a, float b)
{
  if (a < b)
  {
      return a;
  }
  else if (b < a)
  {
      return b;
  }
  else
  {
      return a;
  }
}

void multiplex()
{
  ga1 = readMux(0);
  ga2 = readMux(1) + 10;
  ga3 = readMux(2);
  delay(1000);
}

float readMux(int channel)
{
  int controlPin[] = {s0, s1, s2};
  int muxChannel[8][3]={
    {LOW,LOW,LOW}, //channel 0
    {HIGH,LOW,LOW}, //channel 1
    {LOW,HIGH,LOW}, //channel 2
    // {1,1,0}, //channel 3
    // {0,0,1}, //channel 4
    // {1,0,1}, //channel 5
    // {0,1,1}, //channel 6
    // {1,1,1}, //channel 7
  };

  //loop through the 3 sig
  for(int i = 0; i < 3; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the Z pin
  int val = analogRead(Z_pin);
  return val;
}

//Fuzzy
void fuzzy(float suhu, float kelembaban)
{
  //output
  float mati = 0;
  float hidup = 1; 

  //Fuzzyfikasi suhu
  float dingin = 0;
  float normal = 0;
  float panas = 0;

  if (suhu >= 0 && suhu <= 18) 
  {
    dingin = 1;
  } 
  else if (suhu >= 18 && suhu <= 22.5) 
  {
    dingin = (22.5 - suhu) / (22.5 - 18);
    normal = (suhu - 18) / (22.5 - 18);
  } 
  else if (suhu >= 22.5 && suhu <= 27) 
  {
    normal = (27 - suhu) / (27 - 22.5);
    panas = (suhu - 22.5) / (27 - 22.5);
  } 
  else if (suhu >= 27) 
  {
    panas = 1;
  }

  //Fuzzyfikasi kelembaban
  float kering = 0;
  float sedang = 0;
  float basah = 0;

  if (kelembaban >= 0 && kelembaban <= 40) 
  {
    kering = 1;
  } 
  else if (kelembaban >= 40 && kelembaban <= 60) 
  {
    kering = (60 - kelembaban) / (60 - 40);
    sedang = (kelembaban - 40) / (60 - 40);
  } 
  else if (kelembaban >= 60 && kelembaban <= 80) 
  {
    sedang = (80 - kelembaban) / (80 - 60);
    basah = (kelembaban - 60) / (80 - 60);
  } 
  else if (kelembaban >= 80) {
    basah = 1;
  }

  //defuzzyfikasi
  WA = 0;
  float pem = 0;
  float pen = 0;
  //pembanding
  pem += Min(dingin, kering) * hidup;
  pem += Min(dingin, sedang) * mati;
  pem += Min(dingin, basah) * mati;
  pem += Min(normal, kering) * hidup;
  pem += Min(normal, sedang) * hidup;
  pem += Min(normal, basah) * hidup;
  pem += Min(panas, kering) * hidup;
  pem += Min(panas, sedang) * hidup;
  pem += Min(panas, basah) * hidup;
  //penyebut
  pen += Min(dingin, kering);
  pen += Min(dingin, sedang);
  pen += Min(dingin, basah);
  pen += Min(normal, kering);
  pen += Min(normal, sedang);
  pen += Min(normal, basah);
  pen += Min(panas, kering);
  pen += Min(panas, sedang);
  pen += Min(panas, basah);

  WA = pem / pen;
  
  Serial.println("");
  Serial.print("dingin : ");
  Serial.println(dingin);
  Serial.print("normal : ");
  Serial.println(normal);
  Serial.print("panas : ");
  Serial.println(panas);
  Serial.print("kering : ");
  Serial.println(kering);
  Serial.print("sedang : ");
  Serial.println(sedang);
  Serial.print("basah : ");
  Serial.println(basah);
  Serial.print("Defuzzyfikasi : ");
  Serial.print(pem);
  Serial.print("/");
  Serial.print(pen);
  Serial.print(" = ");
  Serial.println(WA);
  if (WA <= 0.5) {
    msg = "mati";
    Serial.println(msg);
  } else  {
    msg = "hidup";
    Serial.println(msg);
  }
}

void setup() 
{
  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.begin("ITK-LAB.X","K@mpusM3rdeka!"); // nama wifi ssid dan Passwordnya
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(5000); 
  }

  // KONEKSI DHT22
  dht1.setup(D1); 
  dht2.setup(D0);
  dht3.setup(D2);

  // MUX
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);

  // KONEKSI IR
  irsend.begin();
}

void loop() 
{
  // Create a WiFiClientSecure object.
  WiFiClientSecure client;
  // Set the fingerprint to connect the server.
  //client.setFingerprint(fingerprint);
  client.setInsecure();
  // If the host is not responding,return.
  if(!client.connect(host1, httpsPort)){
    Serial.println("Connection Failed!");
    //return;
  }

  //DHT22
  // delay(10000);
  multiplex();
  float t1 = dht1.getTemperature();
  float h1 = dht1.getHumidity();
  float t2 = dht2.getTemperature();
  float h2 = dht2.getHumidity();
  float t3 = dht3.getTemperature();
  float h3 = dht3.getHumidity();

  Serial.print(t1);
  Serial.print("\t");
  Serial.println(h1);
  Serial.print(t2);
  Serial.print("\t");
  Serial.println(h2);
  Serial.print(t3);
  Serial.print("\t");
  Serial.println(h3);

  Serial.print(ga1);
  Serial.print("\t");
  Serial.print(ga2);
  Serial.print("\t");
  Serial.print(ga3);
  Serial.print("\t");

  //konfigurasi 3 sensor
  float Temp = 0;
  float Hum = 0;
  float Gasanalog = 0;
// Menghitung rata-rata suhu
  Temp = (t1 + t2 + t3) / 3;
// Menghitung rata-rata kelembaban
  Hum = (h1 + h2 + h3) / 3;
// Menghitung rata-rata kelembaban
  Gasanalog = (ga1 + ga2 + ga3) / 3;

  fuzzy(Temp, Hum);

  //menampilkan  nilai rata-rata
  Serial.print("Suhu: ");
  Serial.print(Temp);
  Serial.print(" C\tKelembaban: ");
  Serial.print(Hum);
  Serial.print("%\tgas: ");
  Serial.print(Gasanalog);
  Serial.println(" ");

  //output gas
    if(Gasanalog > Threshold)
  {
    Serial.print(" | Smoke detected!");
  }
  else if(Gasanalog <= Threshold)
  {
    Serial.print(" | No Smoke detected");
  }
  Serial.println("");
  delay(2000); // wait 2s for next reading

  //output AC
  if (WA > 0.5) {
    if(ac != "hidup") {
      irsend.sendRaw(rawDataOn, 439, 38);
      ac = "hidup";
    }
    //irsend.sendDaikin(stateOn); //pass the buffer,length,optionally frequency
    Serial.println(F("AC Switched On"));
  } else if (WA <= 0.5) {
    if(ac != "mati") {
      irsend.sendRaw(rawDataOff, 439, 38);
      ac = "mati";
    }
    //irsend.sendDaikin(stateOff); // pass the buffer, length, optionally frequency
    Serial.println(F("AC Switched Off"));
  }
  delay(20000);

  DynamicJsonDocument jsonDocument(128);
  jsonDocument["data1"] = Temp;
  jsonDocument["data2"] = Hum;
  jsonDocument["data3"] = Gasanalog;
  jsonDocument["data4"] = t1;
  jsonDocument["data5"] = t2;
  jsonDocument["data6"] = t3;
  jsonDocument["data7"] = h1;
  jsonDocument["data8"] = h2;
  jsonDocument["data9"] = h3;
  jsonDocument["data10"] = ga1;
  jsonDocument["data11"] = ga2;
  jsonDocument["data12"] = ga3;
  jsonDocument["data13"] = WA;
  jsonDocument["data14"] = msg;

  // Serialize JSON to String
  String payload;
  serializeJson(jsonDocument, payload);

  // Send request to the server with POST method
  if(client.connect(host1, httpsPort)){
    client.println("POST /monitoring_server/insert.php HTTP/1.1");
    client.print("Host: ");
    client.println(host1);
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.println(payload);

    // client.println("POST /monitoring_server/insert.php HTTP/1.1");
    // client.println("Host: " + String(host1));
    // client.println("Content-Type: application/json");
    // client.println("Connection: close");
    // client.print("Content-Length: ");
    // client.println(payload.length());
    // client.println();
    // client.println(payload);
  }
  
  Serial.println("------");
  delay(10000); 
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        //return;
    }
  }
}