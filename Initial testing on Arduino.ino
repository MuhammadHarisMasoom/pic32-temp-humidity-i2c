#include <Wire.h>

float adc_val_temp;
float adc_val_hum;
float t_degC;
float hum;
unsigned int data[6];         // data array contains 4 bytes coming from sensor for temperature conversion

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(0x44);

}

void loop() {
  // put your main code here, to run repeatedly:

  Wire.beginTransmission(0x44);

  Wire.write(0xFD);  // This is the hex code that requests very precise data from sensor

  delay(10);                  // delay of 0.01 seconds as stated in sensor data sheet's pseudo code

  Wire.requestFrom(0x44, 6, true);                // Request 6 bytes from slave. true explicitly stops communication

  data[0] = Wire.read();                    // read funciton returns byte received
  data[1] = Wire.read();

  data[3] = Wire.read();
  data[4] = Wire.read();

  Wire.endTransmission();

  adc_val_temp = data[0] * 256 + data[1];        // conversion function from data sheet
  adc_val_hum = data[4] * 256 + data[3];

  t_degC = -45 + (175 * adc_val_temp) / 65535;    // converting into degree Centigrade
  hum = -6 + 125 * adc_val_hum / 65535;

  Serial.print("Tempetature = ");
  Serial.print(t_degC);
  Serial.print("\tHumidity = ");
  Serial.println(hum);

  delay(1000);
}