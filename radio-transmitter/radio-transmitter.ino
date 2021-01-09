#include <CanSatKit.h>
#include <DHT.h>

using namespace CanSatKit;

const int lm35_pin = A0;
const int dht11_pin = 0;
BMP280 bmp;
DHT dht11(dht11_pin, DHT11);

Radio radio(Pins::Radio::ChipSelect,
            Pins::Radio::DIO0,
            433.0,
            Bandwidth_125000_Hz,
            SpreadingFactor_9,
            CodingRate_4_8);

Frame frame;

static inline float lm35_raw_to_temperature(int raw) {
  return 100 * raw * 3.3 / (1 << 12);
}

void setup() {
  SerialUSB.begin(115200);

  // Set up BMP280 (pressure & temperature)
  if (!bmp.begin()) {
    SerialUSB.println("BMP (pressure sensor) init failed!");
    while(1);
  } else {
    SerialUSB.println("BMP (pressure sensor) init success!");
  }
  bmp.setOversampling(0x10);

  // Set up LM35 (temperature)
  analogReadResolution(12);

  // Set up DHT11 (humidity & temperature)
  dht11.begin();

  // Start radio
  radio.begin();
}

int counter = 0;

void loop() {
  double temp, pres;

  float lm35temp = lm35_raw_to_temperature(analogRead(lm35_pin));

  bmp.measureTemperatureAndPressure(temp, pres);

  frame.println(++counter);
  frame.println(pres, 2);
  frame.println(temp, 2);
  frame.println(lm35temp, 2);
  frame.println(dht11.readTemperature(), 2);
  frame.println(dht11.readHumidity(), 2);

  radio.transmit(frame);
  SerialUSB.println(frame);
  frame.clear();

  delay(1000);
}
