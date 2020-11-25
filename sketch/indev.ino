#include <stdio.h>
#include <stdlib.h>
#include <OneWire.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_BMP280.h>
#include <i2cdetect.h>

BH1750 light_sensor_one(0x23);
BH1750 light_sensor_two(0x5c);
char light_one[10] = {0};
char light_two[10] = {0};

Adafruit_BMP280 bmp_sensor_one;
Adafruit_BMP280 bmp_sensor_two;
char temp_one[10] = {0};
char pressure_one[10] = {0};
char temp_two[10] = {0};
char pressure_two[10] = {0};

char package[200] = {0};

// Reads the BH1750 lux sensors
void read_bh1750() {
  float v1 = light_sensor_one.readLightLevel();
  float v2 = light_sensor_two.readLightLevel(); 
  dtostrf(v1, 6, 2, light_one);
  dtostrf(v2, 6, 2, light_two);
}

void read_bmp280() {
  float t1 = bmp_sensor_one.readTemperature();
  float p1 = bmp_sensor_one.readPressure() / 100;
  float t2 = bmp_sensor_two.readTemperature();
  float p2 = bmp_sensor_two.readPressure() / 100;
  dtostrf(t1, 6, 3, temp_one);
  dtostrf(p1, 6, 3, pressure_one);
  dtostrf(t2, 6, 3, temp_two);
  dtostrf(p2, 6, 3, pressure_two);
}

// Populates and prints the serial message
void print_serial_package() {
  sprintf(
    package, "{\"light\":[%s,%s]}",
    light_one, light_two
  );
  Serial.println(package);
}

void blink(int ms) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(ms);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup_bmp280_sensors() {
  bmp_sensor_one.begin(0x77);
  bmp_sensor_two.begin(0x76);

  bmp_sensor_one.setSampling(Adafruit_BMP280::MODE_NORMAL,
                Adafruit_BMP280::SAMPLING_X2,
                Adafruit_BMP280::SAMPLING_X16,
                Adafruit_BMP280::FILTER_X16,
                Adafruit_BMP280::STANDBY_MS_500);

  bmp_sensor_two.setSampling(Adafruit_BMP280::MODE_NORMAL,
            Adafruit_BMP280::SAMPLING_X2,
            Adafruit_BMP280::SAMPLING_X16,
            Adafruit_BMP280::FILTER_X16,
            Adafruit_BMP280::STANDBY_MS_500);
}


void setup() {
  Wire.begin();
  Serial.begin(9600);
  i2cdetect();
  setup_bmp280_sensors();
  light_sensor_one.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23);
  light_sensor_two.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5c);
  
  pinMode(LED_BUILTIN, OUTPUT);
  delay(2000);
}

void loop() {
  read_bh1750();
  print_serial_package();
  blink(1000);
}
