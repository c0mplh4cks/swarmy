/*

  Swarmy
  by: c0mplh4cks
  version 1.0.4

    Description
    This is the code which is supposed to run
    on the Swarmy's NodeMCU.


*/



/* === Including Dependencies === */
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiUdp.h>

#include "Adafruit_NeoPixel.h"
#include "U8g2lib.h"
#include "Wire.h"
#include "Adafruit_Sensor.h"

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"



/* === Definitions === */

int irLedSwitchPin = 2;

int motorPWMA, motorPWMB;
int motorPins[4] = {19, 18, 17, 16}; // M1A, M1B, M2A, M2B
int motor[4] = {0, 1, 2, 3};


int S0 = 27;
int S1 = 26;
int S2 = 25;


int ledPin = 33;
const int ledAmount = 4;
int ledBrightness = 10;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledAmount, ledPin, NEO_GRB + NEO_KHZ800);

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);

const int freq = 5000;
const int resolution = 8;


/* === Network === */
const char* ssid = "Hello3";
const char* password = "blablabla31415";

int port = 8888;
int packetSize = 0;
String packet;

WiFiServer wifiServer(port);


/* === Functions === */

int oledDisplayInfo(String first, String second, String third)
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_12_tf);

  u8g2.setCursor(0, 8);
  u8g2.print(first);
  u8g2.setCursor(0, 20);
  u8g2.print(second);
  u8g2.setCursor(0, 32);
  u8g2.print(third);

  u8g2.sendBuffer();
  delay(50);

  return 1;
}



int irLedSwitch(int s) {
  if (s == 0) {
    digitalWrite(irLedSwitchPin, LOW);
    Serial.print(" IrledOFF ");
  } else if (s == 1) {
    digitalWrite(irLedSwitchPin, HIGH);
    Serial.print(" IrledON ");
  }

  return 1;
}


int irSensor(int id)
{

  if (id == 0) {
  
    digitalWrite(S0, LOW);
    digitalWrite(S1, LOW);
    digitalWrite(S2, LOW);

  } else if (id == 1) {
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    digitalWrite(S2, LOW);

  } else if (id == 2) {
    digitalWrite(S0, LOW);
    digitalWrite(S1, HIGH);
    digitalWrite(S2, LOW);

  } else if (id == 3) {
    digitalWrite(S0, HIGH);
    digitalWrite(S1, HIGH);
    digitalWrite(S2, LOW);

  } else if (id == 4) {
    digitalWrite(S0, LOW);
    digitalWrite(S1, LOW);
    digitalWrite(S2, HIGH);

  } else if (id == 5) {
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    digitalWrite(S2, HIGH);

  } else if (id == 6) {
    digitalWrite(S0, LOW);
    digitalWrite(S1, HIGH);
    digitalWrite(S2, HIGH);

  } else {
    digitalWrite(S0, HIGH);
    digitalWrite(S1, HIGH);
    digitalWrite(S2, HIGH);

  }
  Serial.print(" Irled: ");
  Serial.print( map(analogRead(34), 0, 4096, 1, 255));
  return map(analogRead(34), 0, 4096, 1, 255);
}


int ledDisplay(uint16_t id, int r, int g, int b)
{
  uint32_t color = strip.Color(r, g, b);
  strip.setPixelColor(id, color);
  strip.show();

  return 1;
}

int changeBrightness (int brightness)
{
  strip.setBrightness(brightness);
  strip.show();

 return 1;
}

int motorA(int speed)                       // Change speed of motor A.
{
 Serial.print(" Motor A speed: ");
 Serial.print(speed);
  if (speed >= 0) {
    speed = constrain(abs(speed), 0, 255);
    ledcWrite(motor[0], 255);
    ledcWrite(motor[1], 255-speed);

  } else {
    speed = constrain(abs(speed), 0, 255);
    ledcWrite(motor[1], 255);
    ledcWrite(motor[0], 255-speed);

  }

return 1;
}


int motorB(int speed)
{
Serial.print(" Motor B speed: ");
 Serial.print(speed);
  if (speed >= 0) {
    speed = constrain(abs(speed), 0, 255);
    ledcWrite(motor[2], 255);
    ledcWrite(motor[3], 255-speed);

  } else {
    speed = constrain(abs(speed), 0, 255);
    ledcWrite(motor[3], 255);
    ledcWrite(motor[2], 255-speed);

  }

  return 1;
}



/* === Read Packet Function === */

int read_packet()
{
  char command = packet[0];
  String args[6];
  int id = 0;
  String build = "";
  int out;


  for (int i=1; i<packetSize; i++) {

    if (packet[i] == ';') {
      args[id] = build;
      build = "";
      id++;

    } else {
      build = build + packet[i];

    }
  }


  if (command == 'A') {
    out = motorA( args[0].toInt() );

  } else if (command == 'B') {
    out = motorB( args[0].toInt() );

  } else if (command == 'C') {
    out = ledDisplay( args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt() );

  } else if (command == 'D') {
    out = oledDisplayInfo( args[0], args[1], args[2] );

  } else if (command == 'I') {
    out = irSensor( args[0].toInt() );

  } else if (command == 'L') {
    out = irLedSwitch( args[0].toInt() );

  } else if (command == 'W') {
    out = changeBrightness( args[0].toInt() );

  } else {
    out = 1;

  }

  return out;
}


/* === Setup === */
void setup()
{
  
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  u8g2.begin();
  oledDisplayInfo("Swarmy", "setup...", "");

  strip.begin();
  strip.setBrightness(ledBrightness);
  strip.show();

  for (int i=0; i<ledAmount; i++) {
    ledDisplay(i, 0, 0, 0);
    delay(10);
  }


  for (int i = 0; i < 4; i++) {
    ledcSetup(motor[i], freq, resolution);
    ledcAttachPin(motorPins[i], motor[i]);
  }

  pinMode(irLedSwitchPin, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  
  digitalWrite(irLedSwitchPin, LOW);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);
  digitalWrite(S2, HIGH);


  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);

  oledDisplayInfo("Swarmy", "connecting to...", ssid);

  Serial.print("connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    for (int i=0; i<ledAmount; i++) { ledDisplay(i, 255, 255, 0); }
    delay(100);
    for (int i=0; i<ledAmount; i++) { ledDisplay(i, 0, 0, 0); }
    Serial.print(".");

  }
  Serial.println("Connected!");

  for (int i=0; i<ledAmount; i++) { ledDisplay(i, 0, 255, 0); }


  wifiServer.begin();
  Serial.print("PORT: ");
  Serial.println(port);
  Serial.print("IP: ");
  Serial.println( WiFi.localIP().toString().c_str() );

  oledDisplayInfo("Swarmy", WiFi.localIP().toString().c_str(), String(port));

}


/* === Loop === */
void loop()
{

  WiFiClient client = wifiServer.available();

  if (client) {
    if (client.connected())
    {
      Serial.println("Client Connected");
    }

    while (client.connected()) {
      while (client.available() > 0) {
        char receivedch = client.read();
        if (receivedch != 0) {
         packet = packet + receivedch; 
        } else {
          Serial.println(packet);
          packetSize = packet.length();
          int response = read_packet();
          client.write(response);
          packet = "";
        }
      }
      //Send Data to connected client
      while (Serial.available() > 0)
      {
        client.write(Serial.read());
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
