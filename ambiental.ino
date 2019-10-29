/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  Output any data on LCD widget!

  App project setup:
    LCD widget, switch to ADVANCED mode, select pin V1
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define TRIGGERPIN D1
#define ECHOPIN    D2
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "JM18vSGTw_rjjahGdFZ6upoqQQTK-Z7E";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "UFC_B1_LAB1";
char pass[] = "";

WidgetLCD lcd(V1);

void setup()
{
  // Debug console
  Serial.begin(9600);

  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
  pinMode(TRIGGERPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);

  lcd.clear(); //Use it to clear the LCD Widget
  lcd.print(0, 0, "Distance in cm"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  // Please use timed events when LCD printintg in void loop to avoid sending too many commands
  // It will cause a FLOOD Error, and connection will be dropped
}

void loop()
{
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

  float level = 100 - (distance/23)*100;

  WidgetLED led1(V4);
  WidgetLED led2(V3);
  WidgetLED led3(V2);
  
  if(level < 27){
    digitalWrite(D6, HIGH);  
    digitalWrite(D7, LOW);  
    digitalWrite(D8, LOW);  
    led1.on();
    led2.off();
    led3.off();
    
  } else if(level < 60){
    digitalWrite(D7, HIGH);
    digitalWrite(D8, LOW);  
    led2.on();
    led3.off();
  } else{
    digitalWrite(D8, HIGH);
    led3.on();
  }

  Serial.print(level);
  Serial.println("%");
  lcd.print(7, 1, level);
  Blynk.run();

  delay(1000);

}
