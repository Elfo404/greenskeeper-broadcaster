#include <Arduino.h>
#include <ArduinoBLE.h>
#include <DHT.h>
#include <limits.h>

#define ANALOG_READ_RESOLUTION 12 // the nano has a 12 bit ADC, so we set the analog read resolution to 12 bits.
#define DHTPIN A0                 // pin connected to the DHT sensor
#define SOILMOISTUREPIN A1        // pin connected to the soil moisture sensor
#define SOILMOISTUREPOWERPIN D2   // digital pin to power the soil moisture sensor
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
uint8_t manufactData[7] = {0, 0, 0, 0, 0, 0, 0};

void setup()
{
  // Serial.begin(9600);
  dht.begin();
  BLE.setLocalName("BLE Sensor");
  BLE.setConnectable(false);
  analogReadResolution(ANALOG_READ_RESOLUTION);
}

void prepareData(uint8_t *data, uint8_t counter, int16_t temperature, uint16_t humidity, uint16_t soilMoisture)
{
  // First byte is a counter
  data[0] = ++counter;

  // 2nd and 3rd bytes are for temperature
  data[1] = temperature >> 8;
  data[2] = temperature & 0xff;

  // 4th and 5th bytes are for humidity
  data[3] = humidity >> 8;
  data[4] = humidity & 0xff;

  // 6th and 7th bytes are for soil moisture
  data[5] = soilMoisture >> 8;
  data[6] = soilMoisture & 0xff;
}

uint16_t readSoilMoistureData()
{
  digitalWrite(SOILMOISTUREPOWERPIN, HIGH);
  delay(100);
  uint16_t sensorValue = map(4096 - analogRead(SOILMOISTUREPIN), 0, 4096, 0, 10000);

  digitalWrite(SOILMOISTUREPOWERPIN, LOW);
  return sensorValue;
}

void loop()
{
  Serial.println("Hello!");

  uint8_t counter = 0;
  int16_t temperature = 0;
  uint16_t humidity = 0,
           soilMoisture = 0;

  while (true)
  {
    if (!BLE.begin())
    {
      Serial.println("starting BLE failed!");
    }

    if (counter == 255)
    {
      counter = 0;
    }
    else
    {
      counter++;
    }

    BLEAdvertisingData advData;
    BLE.setScanResponseData(advData);

    humidity = dht.readHumidity() * 100.0;
    // Serial.print("Humidity: ");
    // Serial.println(humidity);

    temperature = dht.readTemperature() * 100.0;
    // Serial.print("Temperature: ");
    // Serial.println(temperature);

    // soilMoisture will be a value between 0 (wet) a 4096 (dry).
    // We'll map this to a 0-100 scale (0 being dry).
    soilMoisture = readSoilMoistureData();
    // Serial.print("Soil Moisture: ");
    // Serial.println(soilMoisture);

    prepareData(manufactData, counter, temperature, humidity, soilMoisture);

    advData.setManufacturerData(0x00, manufactData, sizeof(manufactData));
    BLE.setAdvertisingData(advData);

    // Advertise for 10 seconds
    BLE.advertise();
    delay(1000 * 10);

    //
    BLE.stopAdvertise();
    BLE.end();

    // Sleep for 30 minutes
    delay(1000 * 60 * 30);
  }
}
