#define I2C_SDA              21
#define I2C_SCL              22
// I2C BUS pins
#define I2C_SDA_2            18
#define I2C_SCL_2            19

#include "Pcf8574.h"
#include "Pcf8574Board.h"



#define NO_OF_BOARDS 2
int MAX_PINS = 0;

// I2C for SIM800 (to keep it running when powered from battery)
TwoWire I2CPower = TwoWire(0);

// I2C for PCF8574T 
TwoWire PCF8574T = TwoWire(1);

Pcf8574Board pcf8574Board(&PCF8574T);

#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

bool setPowerBoostKeepOn(int en){
  I2CPower.beginTransmission(IP5306_ADDR);
  I2CPower.write(IP5306_REG_SYS_CTL0);
  if (en) {
    I2CPower.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    I2CPower.write(0x35); // 0x37 is default reg value
  }
  return I2CPower.endTransmission() == 0;
}

void setup() {
  Serial.begin(115200);

  // Start I2C communication for both buses
  I2CPower.begin(I2C_SDA, I2C_SCL, 400000);    // For power management
  PCF8574T.begin(I2C_SDA_2, I2C_SCL_2, 400000);  // For PCF8574
  // Keep power when running from battery
  bool isOk = setPowerBoostKeepOn(1);
  Serial.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));
  
  Wire = PCF8574T; // Switch to the second I2C bus
  
  pcf8574Board.initPcf8574Boards(NO_OF_BOARDS);
  MAX_PINS = NO_OF_BOARDS * 8;

  for (int i = 1; i <= MAX_PINS; i++) {
    pcf8574Board.switchOff(i);
  }
}

void loop() {
  Wire = PCF8574T; // Switch to the second I2C bus
  Serial.print("MAX PIN ");
  Serial.println(MAX_PINS);
  
  for (int i = 1; i <= MAX_PINS; i++) {
    pcf8574Board.switchOn(i);
  }
  delay(2000);

  for (int i = 1; i <= MAX_PINS; i++) {
    pcf8574Board.switchOff(i);
  }
  delay(2000);
  
}