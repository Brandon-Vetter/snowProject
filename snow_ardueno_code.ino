#include <Wire.h>
#include <SPI.h>
#include "HX711.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#include "DHT.h"

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <MySQL_Cursor.h>
#include <MySQL_Connection.h>


//#define DHTPIN 5
//#define DHTTYPE DHT22

int miliSecToSec = 1000000;

//DHT dht(DHTPIN, DHTTYPE);
Adafruit_BME280 bme;
HX711 scale;

uint8_t dataPin = 25;
uint8_t clockPin = 26;

double humidity;
double temp;
int pressure;
double fsr;
double calFsr;


const char* ssid = "Snow";
const char* password = "raspberry";
WiFiServer server(80);
//IPAddress ip(192.168.43.191);
IPAddress gateway(10,42,0,1);
IPAddress subnet(135,135,135,0);
WiFiClient client;

MySQL_Connection conn((Client *)&client);
char INSERT_SQL[] = "INSERT INTO weatherData.weatherData(temp, mass, pressure, humid, Date, time) VALUES (%.2f, %.2f, %d, %.2f, CURDATE(), CURTIME());";
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
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
    if(counter == 10){
        esp_sleep_enable_timer_wakeup(miliSecToSec * 5);
        esp_deep_sleep_start();
    }
    counter++;
  }
  counter = 0;
  while (conn.connect(server_addr, 3306, user, pass)!= true){
  delay(200);
  Serial.print(".");
  if(counter == 10){
        esp_sleep_enable_timer_wakeup(miliSecToSec * 5);
        esp_deep_sleep_start();
    }
    counter++;
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
  bme.begin();
  scale.begin(dataPin, clockPin);
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
 scale.set_scale(127.15);
}

void loop() {
  digitalWrite(13,HIGH);
  delay(100);
  while(bme.readTemperature()<-100||bme.readTemperature()>1000||bme.readPressure()==0){
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
    Serial.println(bme.readTemperature());
    digitalWrite(13, HIGH);
    delay(100);
    esp_sleep_enable_timer_wakeup(miliSecToSec * 10);
    esp_deep_sleep_start();
  }
  fsr = scale.get_units(10);
  //238.4641048082X^0.42419914602406
  //2.2432605564827 * 1.002430730588^x
  //0.07234886968438X -37.641671827593
  Serial.println(fsr);
  humidity = bme.readHumidity();
  temp = bme.readTemperature();
  pressure = bme.readPressure();//in pascals
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("temp in C: ");
  Serial.println(temp);
  Serial.print("Pressure: ");
  Serial.println(pressure/100);
  MySQL_Cursor *SQL = new MySQL_Cursor(&conn);
  sprintf(query, INSERT_SQL, temp, fsr, pressure, humidity);
  SQL->execute(query);
  Serial.println(query);
  delete SQL;
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  digitalWrite(13, LOW);
  conn.close();
  esp_sleep_enable_timer_wakeup(miliSecToSec * 5);
  esp_deep_sleep_start();
}
