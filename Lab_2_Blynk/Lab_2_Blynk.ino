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
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define LED 2


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "MQvFN-FAhdo6UFqzxerXzgd9m6xOgWFb";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 
char ssid[32] = "Drew";
char pass[32] = "12345678";

const int ledChannel = 0;
const int freq = 5000;
const int resolution = 10;


int ledState = 0;
int brightness = 0;
BlynkTimer timer;
int timeCounter = 0;
String message = "";


/*
 * V0 = button
 * V1 = slider
 * V2 = display
 * V3 = terminal
 */

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(10L, myTimerEvent);
  pinMode(LED, OUTPUT);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void myTimerEvent()
{
  if(timeCounter == 100)
  {
    Blynk.virtualWrite(V2, millis() / 1000);
    timeCounter = 0;
  }

  char character;

  while(Serial.available())
  {
    character = Serial.read();
    message.concat(character);
  }

  if(message != "")
  {
    Blynk.virtualWrite(V3, message);
    message = "";
  }

  timeCounter ++;
}

BLYNK_WRITE(V0)//button
{
  int pinValue = param.asInt();

  if(pinValue == 0)
  {
    ledState = 0;
    ledcWrite(ledChannel, 0);
  }
  else
  {
    ledState = 1;
    ledcWrite(ledChannel, brightness);
  }
}

BLYNK_WRITE(V1)//slider
{
  int pinValue = param.asInt();

  brightness = pinValue;

  if(ledState == 1)
  {
    ledcWrite(ledChannel, brightness);
  }
}
