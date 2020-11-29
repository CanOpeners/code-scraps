#include <CanSatKit.h>

using namespace CanSatKit;

const int lm35_pin = A0;
BMP280 bmp;

static inline float lm35_raw_to_temperature(int raw) {
  return 100 * raw * 3.3 / (1 << 12);
}

void setup() {
  SerialUSB.begin(9600);

  if (!bmp.begin()) {
    SerialUSB.println("BMP (pressure sensor) init failed!");
    while(1);
  } else {
    SerialUSB.println("BMP (pressure sensor) init success!");
  }

  bmp.setOversampling(0x10);

  analogReadResolution(12);
}

void loop() {
  double temp, pres;

  float lm35temp = lm35_raw_to_temperature(analogRead(lm35_pin));

  bmp.measureTemperatureAndPressure(temp, pres);

  SerialUSB.print("[BMP] Pressure = ");
  SerialUSB.print(pres, 2);
  SerialUSB.println(" hPa");
  SerialUSB.print("[BMP] Temperature = ");
  SerialUSB.print(temp, 2);
  SerialUSB.println(" deg C");
  SerialUSB.print("[LM35] Temperature = ");
  SerialUSB.print(lm35temp, 2);
  SerialUSB.println(" deg C\n");

  delay(50);
}
