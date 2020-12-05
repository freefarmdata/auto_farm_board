#include <stdio.h>
#include <stdlib.h>
#include <OneWire.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_BMP280.h>
// #include <i2cdetect.h>

// capacitive soil sensor values
int soil_one = 1024;
int soil_two = 1024;
int soil_three = 1024;
int soil_four = 1024;

// BH1750 Lux Sensors
BH1750 light_sensor_one(0x23);
BH1750 light_sensor_two(0x5c);
char light_one[10] = {0};
char light_two[10] = {0};

// BMP280 Pressure and Temperature sensors
Adafruit_BMP280 bmp_sensor_one;
Adafruit_BMP280 bmp_sensor_two;
char temp_one[10] = {0};
char pressure_one[10] = {0};
char temp_two[10] = {0};
char pressure_two[10] = {0};

// The prepackaged buffer for serial json output
char package[200] = {0};

// Reads the capacitive soil sensors
void read_soil() {
  soil_one = analogRead(0);
  soil_two = analogRead(1);
  soil_three = analogRead(2);
  soil_four = analogRead(3);
}

// Reads the BH1750 lux sensors
void read_bh1750() {
  float v1 = light_sensor_one.readLightLevel();
  float v2 = light_sensor_two.readLightLevel(); 
  dtostrf(v1, 6, 3, light_one);
  dtostrf(v2, 6, 3, light_two);
}

// Reads the BMP280 temperature and pressure sensors
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
void print_package() {
  char json_str[] = "{\"soil\":[%d,%d,%d,%d],\"light\":[%s,%s],\"temp\":[%s,%s],\"pressure\":[%s,%s]}";
  sprintf(
    package, json_str,
    soil_one, soil_two, soil_three, soil_four,
    light_one, light_two,
    temp_one, temp_two,
    pressure_one, pressure_two
  );
  Serial.println(package);
}

void blink(int ms) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(ms);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup_bmp280_sensors() {
  if (!bmp_sensor_one.begin(0x77)) {
    Serial.println("BMP280 at 0x77 not found");
  }

  if (!bmp_sensor_two.begin(0x76)) {
    Serial.println("BMP280 at 0x76 not found");
  }

  bmp_sensor_one.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500
  );

  bmp_sensor_two.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500
  );
}

void setup_bh1750_sensors() {
  if (!light_sensor_one.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23)) {
    Serial.println("BH1750 at 0x23 not found");
  }
  
  if (!light_sensor_two.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5c)) {
    Serial.println("BH1750 at 0x5c not found");
  }
}


void setup() {
  Wire.begin();
  Serial.begin(9600);
  // i2cdetect();
  setup_bmp280_sensors();
  setup_bh1750_sensors();
  pinMode(LED_BUILTIN, OUTPUT);
  delay(2000);
}

void loop() {
  read_soil();
  read_bh1750();
  read_bmp280();
  print_package();
  blink(5000);
}

