#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_bme280.h>
#include "DHT.h"

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <MySQL_Cursor.h>
#include <MySQL_Connection.h>


#define DHTPIN 5
#define DHTTYPE DHT22

int miliSecToSec = 1000000;

DHT dht(DHTPIN, DHTTYPE);
//Adafruit_bme280 bme;

int humidity;
int temp;
int pressure;
int fsr;


const char* ssid = "Snow";
const char* password = "raspberry";
WiFiServer server(80);
//IPAddress ip(192.168.43.191);
IPAddress gateway(10,42,0,1);
IPAddress subnet(135,135,135,0);
WiFiClient client;

MySQL_Connection conn((Client *)&client);
char INSERT_SQL[] = "INSERT INTO weatherData.weatherData(temp, mass, pressure, humid, Date) VALUES (%d, %d, %d, %d,CURDATE());";
char query[138];
IPAddress server_addr(10,42,0,1);
char user[] = "Ardueno";
char pass[] = "raspberry";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  boolean status = false;

  Serial.println("Beginning WiFi setup...");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  while (conn.connect(server_addr, 3306, user, pass)!= true){
  delay(200);
  Serial.print(".");
}
Serial.println("\nConnected to SQL Server");
digitalWrite(2, HIGH);
delay(130);
digitalWrite(2, LOW);
delay(130);
digitalWrite(2, HIGH);
delay(130);
digitalWrite(2, LOW);
delay(130);
digitalWrite(2, HIGH);
delay(130);
digitalWrite(2, LOW);
delay(130);
digitalWrite(13, HIGH);
delay(100);
//    while(!status){
  //status = 
  dht.begin();
 // if(!status){
//    Serial.println("Could not start dht280 sensor");
//    digitalWrite(13, LOW);
//    digitalWrite(2, HIGH);
//    delay(1000);
//    digitalWrite(2, LOW);
//    delay(1000);
//    digitalWrite(2, HIGH);
//    delay(1000);
//    digitalWrite(2, LOW);
//    digitalWrite(13, HIGH);
//    delay(3000);
//  }
//  }
  Serial.println("Setup complete");
  for( int i = 0; i<5;i++){
  digitalWrite(2, HIGH);
  delay(100);
  digitalWrite(2, LOW);
  delay(100);
  }
}

void loop() {
  digitalWrite(13,HIGH);
  delay(100);
  while(dht.readTemperature()<-100||dht.readTemperature>1000){
    digitalWrite(13,LOW);
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    digitalWrite(13, HIGH);
    Serial.println("sensor not working, attempting restart...");
    Serial.println(dht.readTemperature());
    digitalWrite(13, HIGH);
    delay(100);
    esp_sleep_enable_timer_wakeup(miliSecToSec * 10);
    esp_deep_sleep_start();
  }
  fsr = analogRead(34);
  Serial.println(fsr);
  humidity = dht.readHumidity();
  temp = dht.readTemperature();
//  pressure = dht.readPressure();
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("temp in C: ");
  Serial.println(temp);
  Serial.print("Pressure: ");
  Serial.println(pressure/100);
  MySQL_Cursor *SQL = new MySQL_Cursor(&conn);
  sprintf(query, INSERT_SQL, temp, fsr, pressure, humidity);
  SQL->execute(query);
  delete SQL;
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  digitalWrite(13, LOW);
  conn.close();
  esp_sleep_enable_timer_wakeup(miliSecToSec * 60);
  esp_deep_sleep_start();
}
