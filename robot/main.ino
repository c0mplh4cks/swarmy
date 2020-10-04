/*

  Swarmy
  by: c0mplh4cks

  * Description *
    This is the code which is supposed to run
    on the Swarmy's NodeMCU.


*/



/* === Including Dependencies === */
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "PCF8574.h"
#include "Wire.h"
#include "Adafruit_Sensor.h"





/* === Definitions === */
PCF8574 pcf8574(0x20);
WiFiUDP Udp;

//

int PWMA = 14;                              // Defining PINs.
int PWMB = 0;
int DA = 12;
int DB = 2;





/* === Network === */
const char* ssid = "";                      // WiFi Network Name.
const char* password = "";                  // WiFi Password.

unsigned int port = 8888;                   // PORT.
char packet[255];                           // Packet buffer.





/* === Write and Read Functions === */
int motorA(int speed)                       // Change speed of motor A.
{
  if (speed >= 0) { digitalWrite(DA, LOW); }
  else { digitalWrite(DA, HIGH); }

  analogWrite(PWMA, speed);

  return 1;
}



int motorB(int speed)                       // Change speed of motor B.
{
  if (speed >= 0) { digitalWrite(DB, LOW); }
  else { digitalWrite(DB, HIGH); }

  analogWrite(PWMB, speed);

  return 1;
}





/* === Read Packet Function === */
void read_packet()
{

  char command = packet[0];
  String build = "";

  for (int i=0; i<5; i++)
  {
    if (packet[i+1] == ';') { break; }
    build = build + packet[i+1];
  }

  int value = build.toInt();


  int out;

  if (command == 'A') { out = motorA(value); }
  if (command == 'B') { out = motorB(value); }

}





/* === Setup === */
void setup()
{

  Serial.begin(115200);                     // Start serial communication.
  Serial.println();


  for (int i=0; i<8; i++) {                 // Setting up PCF8574 pins.
    pcf8574.pinMode(i, OUTPUT);
  }
  pcf8574.begin();

  pinMode(0, INPUT_PULLUP);                 // Setting up Flash button.

  pinMode(PWMA, OUTPUT);                    // Setting up pins required.
  pinMode(PWMB, OUTPUT);                    // for controlling motors.
  pinMode(DA, OUTPUT);
  pinMode(DB, OUTPUT);
  digitalWrite(DA, LOW);
  digitalWrite(DB, LOW);
  digitalWrite(PWMB, 0);
  digitalWrite(PWMA, 0);


  WiFi.begin(ssid, password);               // Connect to WiFi network.

  Serial.print("Connecting to ");           // Give Feedback while connecting.
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");


  Udp.begin(port);                          // Start UDP listener.
  Serial.print("PORT: ");
  Serial.println(port);
  Serial.print("IP: ");
  Serial.println( WiFi.localIP().toString().c_str() );

}





/* === Loop === */
void loop()
{

  int packetSize = Udp.parsePacket();       // Receiving packet
  if (packetSize)
  {


    Serial.print("Received UDP packet from: ");
    Serial.println( Udp.remoteIP().toString().c_str() );

    int len = Udp.read(packet, 255);
    if (len > 0) { packet[len] = 0; }

    //Serial.print("Packet Data: ");
    //Serial.println(packet);

    read_packet();
  }


}
