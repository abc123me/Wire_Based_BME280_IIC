#include "BME280.h"

BME280 bme;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  if(!bme.begin()) {
    Serial.println("BME280 failed to initialize!");
    while(1);
  }
  Serial.print("Chip ID 0x");
  Serial.print(bme.readChipID(), HEX); // Read sensor ID
  Serial.print(bme.hasHumidity() ? " (BME280)" : " (BMP280)");
  Serial.println(" initialized!");
}

void loop() {
  Serial.print("Temperature: ");
  Serial.print(bme.readTemperatureF());
  Serial.print("°F, Pressure: ");
  Serial.print(bme.readPressurePSI());
  Serial.print("PSI");
  if(bme.hasHumidity()) {
    Serial.print("Humidity: ");
    Serial.print(bme.readHumidityRH());
    Serial.println("%");
  } else Serial.println();
  delay(1000);
}
