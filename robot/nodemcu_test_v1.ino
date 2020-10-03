#include "Arduino.h"
#include "PCF8574.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

PCF8574 pcf8574(0x20);
Adafruit_MPU6050 mpu;

int Motor1;
int Motor1prev = 0;
int Motor2;
int Motor2prev = 0;
int Grijper;
int Grijperprev = 0;
int Tilt;
int Tiltprev = 0;
int Camera;
int Cameraprev = 0;


char serialchar;
String serialData;
String serialData2;
String commando;

int PWMA = 14;
int PWMB = 0;
int DA = 12;
int DB = 2;


#define SendKey 0  //Button to send data Flash BTN on NodeMCU

int port = 8888;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "Hello2";  //Enter your wifi SSID
const char *password = "blablabla31415";  //Enter your wifi Password


void setup()
{

  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);


  for (int i = 0; i < 8; i++) {
    pcf8574.pinMode(i, OUTPUT);
  }
  pcf8574.begin();



  Serial.begin(115200);
  pinMode(SendKey, INPUT_PULLUP); //Btn to send data
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi

  // Wait for connection
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);
  delay(100);

  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DA, OUTPUT);
  pinMode(DB, OUTPUT);
  digitalWrite(DA, LOW);
  digitalWrite(DB, LOW);
  digitalWrite(PWMB, 0);
  digitalWrite(PWMA, 0);
}

void loop()
{
  WiFiClient client = server.available();

  if (client) {
    if (client.connected())
    {
      Serial.println("Client Connected");
    }

    while (client.connected()) {
      while (client.available() > 0) {

        serialchar = client.read();

        if (aanelkaarplakken(serialchar) == 1) {
          Serial.print(commando);
          Serial.println(";");

          serialData = commando;
          Serial.println(serialData);
          //serial2int(client);
          client.print( serial2int() );


          Serial.print(" A: ");
          Serial.print(Motor1);
          Serial.print(" B: ");
          Serial.print(Motor2);


          if (Motor1 >= 0) {
            digitalWrite(DA, LOW);
          }
          else {
            digitalWrite(DA, HIGH);
          }
          analogWrite(PWMA, Motor1);

          if (Motor2 >= 0) {
            digitalWrite(DB, LOW);
          }
          else {
            digitalWrite(DB, HIGH);
          }
          analogWrite(PWMB, Motor2);

        }
      }
    }
    client.stop();
  }
}
