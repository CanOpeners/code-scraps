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

void loop() {
  double temp, pres;

  float lm35temp = lm35_raw_to_temperature(analogRead(lm35_pin));

  /*SerialUSB.print("\x1b[31m[DHT11]\x1b[m: ");
  SerialUSB.print("temp_degC=");
  SerialUSB.print(dht11.readTemperature(), 2);
  SerialUSB.print(" humidity_%=");
  SerialUSB.println(dht11.readHumidity(), 2);*/

  bmp.measureTemperatureAndPressure(temp, pres);

  /*SerialUSB.print("[BMP] Pressure = ");
  SerialUSB.print(pres, 2);
  SerialUSB.println(" hPa");
  SerialUSB.print("[BMP] Temperature = ");
  SerialUSB.print(temp, 2);
  SerialUSB.println(" deg C");
  SerialUSB.print("[LM35] Temperature = ");
  SerialUSB.print(lm35temp, 2);
  SerialUSB.println(" deg C\n");*/

  frame.print("bmp_pres_hPa=");
  frame.print(pres, 2);
  frame.print('\n');
  frame.print("bmp_temp_degC=");
  frame.print(temp, 2);
  frame.print('\n');
  frame.print("lm35_temp_degC=");
  frame.print(lm35temp, 2);
  frame.print('\n');
  frame.print("dht_temp_degC=");
  frame.print(dht11.readTemperature(), 2);
  frame.print('\n');
  frame.print("dht_hum_pcent=");
  frame.print(dht11.readHumidity(), 2);
  frame.print('\n');

  radio.transmit(frame);
  SerialUSB.println(frame);
  frame.clear();

  delay(1000);
}
