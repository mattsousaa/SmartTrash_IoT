#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include "base.h"
#include "email.h"
#include <BlynkSimpleEsp8266.h>

#define TRIGGERPIN D1
#define ECHOPIN    D2

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "JM18vSGTw_rjjahGdFZ6upoqQQTK-Z7E";

// Your WiFi credentials.
// Set password to "" for open networks.
const char* SSID = "Emanuel Router";
const char* PASS = "pedrogay";

WidgetLCD lcd(V1);

char server[] = "mail.smtp2go.com";
ADC_MODE(ADC_VCC);

WiFiClient client;

byte sendEmail(){
    
byte thisByte = 0;
byte respCode;

if (client.connect(server, 2525) == 1) {
    Serial.println(F("connected"));
} else {
    Serial.println(F("connection failed"));
    return 0;
}
if (!eRcv()) return 0;

Serial.println(F("Sending EHLO"));
client.println("EHLO www.example.com");
if (!eRcv()) return 0;

Serial.println(F("Sending auth login"));
client.println("auth login");
if (!eRcv()) return 0;

Serial.println(F("Sending User"));
// Change to your base64, ASCII encoded user
client.println("bWF0ZXVzZW5nX2VjQGFsdS51ZmMuYnI="); // SMTP UserID
if (!eRcv()) return 0;

Serial.println(F("Sending Password"));
// change to your base64, ASCII encoded password
client.println("bloyT0pyM21MZEdF");//  SMTP Passw
if (!eRcv()) return 0;

Serial.println(F("Sending From"));   // change to your email address (sender)
client.println(F("MAIL From: mateusworks@outlook.com"));// not important 
if (!eRcv()) return 0;   // change to recipient address

Serial.println(F("Sending To"));
client.println(F("RCPT To: mateuseng_ec@alu.ufc.br"));
if (!eRcv()) return 0;

Serial.println(F("Sending DATA"));
client.println(F("DATA"));
if (!eRcv()) return 0;

Serial.println(F("Sending email"));   // change to recipient address
client.println(F("To: mateuseng_ec@alu.ufc.br"));   // change to your address
client.println(F("From: mateusworks@outlook.com"));
client.println(F("Subject: Emails from ESp8266\r\n"));
client.print(F("1st Trash can"));
client.print(ESP.getVcc());
client.println(F("mV"));
client.print(F("Device ID: "));
client.println(ESP.getChipId());
Serial.print(F("Voltage"));
Serial.print(ESP.getVcc());
client.println(F("."));
if (!eRcv()) return 0;

Serial.println(F("Sending QUIT"));
client.println(F("QUIT"));
if (!eRcv()) return 0;

client.stop();
Serial.println(F("disconnected"));
return 1;
}

byte eRcv(){

byte respCode;
byte thisByte;
int loopCount = 0;
while (!client.available()){
    delay(1);
    loopCount++;     // if nothing received for 10 seconds, timeout
    if (loopCount > 10000) {
        client.stop();
        Serial.println(F("\r\nTimeout"));
        return 0;
    }
}

respCode = client.peek();

while (client.available()){
    thisByte = client.read();
    Serial.write(thisByte);
}

if (respCode >= '4'){
//  efail();
return 0;
}
return 1;
}

void setup(){
  // Debug console
  Serial.begin(9600);
  delay(10);
  Serial.println("");
  Serial.println("");
  Serial.print("Connecting");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IPess: ");
  Serial.println(WiFi.localIP());

  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
  pinMode(TRIGGERPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  Blynk.begin(auth, SSID, PASS);
  
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);

  lcd.clear(); //Use it to clear the LCD Widget
  lcd.print(0, 0, "Distance in cm"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  // Please use timed events when LCD printintg in void loop to avoid sending too many commands
  // It will cause a FLOOD Error, and connection will be dropped
}

void loop(){
  
  lcd.clear();
  lcd.print(0, 0, "Level's garbage: "); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  float duration, distance;
  digitalWrite(TRIGGERPIN, LOW);  
  delayMicroseconds(3); 
  
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(12); 
  
  digitalWrite(TRIGGERPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = (duration/2) / 29.1;
 
  WidgetTerminal terminal(V0);
  terminal.println("Distance from trash can in cm:");
  terminal.println(distance);
  
  terminal.flush();

  float level = 100 - (distance/75)*100;

  WidgetLED led1(V4);
  WidgetLED led2(V3);
  WidgetLED led3(V2);
  
  if(level >= 0 && level <= 29){
    digitalWrite(D6, HIGH);  
    digitalWrite(D7, LOW);  
    digitalWrite(D8, LOW);  
    led1.on();
    led2.off();
    led3.off();
    
  } else if(level < 50){
    digitalWrite(D7, HIGH);
    digitalWrite(D8, LOW); 
    led2.on(); 
    led1.off();
   
    led3.off();
  } else{
    digitalWrite(D8, HIGH);
    led3.on();
    led1.off();
    led2.off();
  }

  Serial.print(level);
  Serial.println("%");
  lcd.print(7, 1, level);
  Blynk.run();

  if(level > 50) sendEmail();
  delay(3500);

  terminal.clear();
}