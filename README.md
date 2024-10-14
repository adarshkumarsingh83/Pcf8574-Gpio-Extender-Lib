# Pcf8574-Gpio-Extender-Lib

<a href="https://github.com/adarshkumarsingh83/Pcf8574-Gpio-Extender-Lib"> Original Repository </a>

## Updates?
Currently simple changes in Pcf8574Board.h
```
private:
    int _boardAddress[8] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
    TwoWire *wire;     // wire object so secondary I2C Bus can be used.
    typedef struct BoardPin {
      int boardNo;
      int boardPin;
      bool processed;
    } BoardPinData;

    int _totalPcf8574Boards;
    Pcf8574  * _pcf8574Boards;


    Pcf8574Board::BoardPinData findBoardPin(int no);
    Pcf8574 getPcf8574Object(int i);

  public:
    Pcf8574Board(TwoWire *wire) {
      this->wire = wire;
      //wire->begin();
    }

``` 

In <a href="https://github.com/oguzalp7/ESP32-Pcf8574-Gpio-Extender-Lib/blob/main/examples/Pcf8574-lib-8boards/Pcf8574-lib-8boards.ino">examples/esp32-two-wire </a>:

```
// Above setup()
// I2C for SIM800 (to keep it running when powered from battery)
TwoWire I2CPower = TwoWire(0);  // default I2C Bus for more critical tasks and maybe handling inputs.

// I2C for PCF8574T 
TwoWire PCF8574T = TwoWire(1);    // PCF8574T (red ones)

Pcf8574Board pcf8574Board(&PCF8574T);

``` 


```
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

``` 

On the other hand, Pcf8574 class doesn't. So, we manually switch the I2C Bus in the loop.
``` 
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
``` 