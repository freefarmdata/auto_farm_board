#include <stdio.h>
#include <stdlib.h>
#include <dht11.h>
#include <OneWire.h> 
#include "DallasTemperature.h"

int soil_1_pin = 0;
int soil_2_pin = 1;
int soil_3_pin = 2;
int soil_4_pin = 3;
int soil_1_val = 1024;
int soil_2_val = 1024;
int soil_3_val = 1024;
int soil_4_val = 1024;

dht11 DHT11;
int dht11_pin = 4;
char dht11_temp[10] = {0};
char dht11_humid[10] = {0};

int ds18b20_pin = 2;
OneWire one_wire(ds18b20_pin);
DallasTemperature sensors(&one_wire);
char ds_1_temp[10] = {0};
char ds_2_temp[10] = {0};

char package[200] = {0};

// Reads the capacitive soil sensors
void read_soil_sensors() {
  soil_1_val = analogRead(soil_1_pin);
  soil_2_val = analogRead(soil_2_pin);
  soil_3_val = analogRead(soil_3_pin);
  soil_4_val = analogRead(soil_4_pin);
}

// Reads the DHT11 for the temperature and humidity readings
void read_dht11() {
  DHT11.read(dht11_pin);
  float t = (float)DHT11.temperature;
  float h = (float)DHT11.humidity;
  dtostrf(t, 3, 1, dht11_temp);
  dtostrf(h, 3, 1, dht11_humid);
}

// Reads the one wire ds18b20 devices for temperature
void read_ds18b20() {
  sensors.requestTemperatures();
  float v1 = sensors.getTempCByIndex(0);
  float v2 = sensors.getTempCByIndex(1);
  dtostrf(v1, 6, 3, ds_1_temp);
  dtostrf(v2, 6, 3, ds_2_temp);
}

// Populates and prints the serial message
void print_serial_package() {
  sprintf(
    package, "{\"soil\":[%d,%d,%d,%d],\"temp\":[%s, %s]},\"humid\":[%s,%s]}",
    soil_1_val, soil_2_val, soil_3_val, soil_4_val,
    ds_1_temp, ds_2_temp,
    dht11_humid, dht11_temp
  );
  Serial.println(package);
}

void blink(int ms) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(ms);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
  sensors.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  read_soil_sensors();
  read_dht11();
  read_ds18b20();
  print_serial_package();
  blink(500);
}
