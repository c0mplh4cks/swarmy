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
#include "PCF8574.h"
#include "Wire.h"
#include "Adafruit_Sensor.h"





/* === Definitions === */
int irLedSwitchPin = 3;


int ledPin = 25;
int ledAmount = 4;
int ledBrightness = 21;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledAmount, ledPin, NEO_GRB + NEO_KHZ800);


U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);


WiFiUDP udp;


int encoderOutput = 32;

volatile long encoderValue = 0;
int interval = 50;
long previousMillis = 0;
long currentMillis = 0;
float rpm = 0;


int motorPWMA, motorPWMB;
int motorPins[4] = {33, 32, 26, 27}; // M1A, M1B, M2A, M2B
int motor[4] = {0, 1, 2, 3};


const int freq = 5000;
const int resolution = 8;

int encoderLeftPin = 16;
int encoderRightPin = 14;







/* === Network === */
const char* ssid = "Hello2";
const char* password = "blablabla31415";

int port = 8888;
char packet[255];
int packetSize;
char data[64];





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
    pcf8574.digitalWrite(irLedSwitchPin, LOW);
  } else if (s == 1) {
    pcf8574.digitalWrite(irLedSwitchPin, HIGH);
  }

  return 1;
}



int irSensor(int id)
{

  if (id == 0) {
    pcf8574.digitalWrite(0, LOW);
    pcf8574.digitalWrite(1, LOW);
    pcf8574.digitalWrite(2, LOW);

  } else if (id == 1) {
    pcf8574.digitalWrite(0, HIGH);
    pcf8574.digitalWrite(1, LOW);
    pcf8574.digitalWrite(2, LOW);

  } else if (id == 2) {
    pcf8574.digitalWrite(0, LOW);
    pcf8574.digitalWrite(1, HIGH);
    pcf8574.digitalWrite(2, LOW);

  } else if (id == 3) {
    pcf8574.digitalWrite(0, HIGH);
    pcf8574.digitalWrite(1, HIGH);
    pcf8574.digitalWrite(2, LOW);

  } else if (id == 4) {
    pcf8574.digitalWrite(0, LOW);
    pcf8574.digitalWrite(1, LOW);
    pcf8574.digitalWrite(2, HIGH);

  } else if (id == 5) {
    pcf8574.digitalWrite(0, HIGH);
    pcf8574.digitalWrite(1, LOW);
    pcf8574.digitalWrite(2, HIGH);

  } else if (id == 6) {
    pcf8574.digitalWrite(0, LOW);
    pcf8574.digitalWrite(1, HIGH);
    pcf8574.digitalWrite(2, HIGH);

  } else {
    pcf8574.digitalWrite(0, HIGH);
    pcf8574.digitalWrite(1, HIGH);
    pcf8574.digitalWrite(2, HIGH);

  }

  return analogRead(A0);
}



int ledDisplay(uint16_t id, int r, int g, int b)
{
  uint32_t color = strip.Color(r, g, b);
  strip.setPixelColor(id, color);
  strip.show();

  return 1;
}



int motorA(int speed)                       // Change speed of motor A.
{

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



void updateEncoder()
{
  encoderValue++;
}



void rpmCount()
{
  currentMillis = millis();
  if ( (currentMillis - previousMillis) > interval ) {
    rpm = (float)(encoderValue * 1200 / encoderOutput);

    encoderValue = 0;
  }
}





/* === Read Packet Function === */
String read_packet()
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
    
  } else {
    out = 1;

  }

  return String(out);
}





/* === Setup === */
void setup()
{
  u8g2.begin();
  oledDisplayInfo("Swarmy", "setup...", "");

  strip.begin();
  strip.setBrightness(ledBrightness);
  strip.show();

  for (int i=0; i<ledAmount; i++) {
    ledDisplay(i, 0, 0, 0);
  }


  for (int i = 0; i < 4; i++) {
    ledcSetup(motor[i], freq, resolution);
    ledcAttachPin(motorPins[i], motor[i]);
  }


  attachInterrupt(digitalPinToInterrupt(encoderRightPin), updateEncoder, RISING);
  previousMillis = millis();


  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);

  oledDisplayInfo("Swarmy", "Connecting to...", ssid);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");


  udp.begin(port);
  Serial.print("PORT: ");
  Serial.println(port);
  Serial.print("IP: ");
  Serial.println( WiFi.localIP().toString().c_str() );

  oledDisplayInfo("Swarmy", WiFi.localIP().toString().c_str(), String(port));

}





/* === Loop === */
void loop()
{
  rpmCount();

  packetSize = udp.parsePacket();
  if (packetSize) {

    Serial.print("Received UDP packet from: ");
    Serial.println( udp.remoteIP().toString().c_str() );

    int len = udp.read(packet, 255);
    if (len > 0) {
      packet[len] = 0;
    }

    String response = read_packet();

    udp.beginPacket(udp.remoteIP(), udp.remotePort());

    response.toCharArray(data, 64);
    udp.write((uint8_t *)data, sizeof(data));

    udp.endPacket();

  }

  delay(10);


}
