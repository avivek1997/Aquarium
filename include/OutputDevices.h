class OutputDevice
{
private:
  int _pin;
  bool _isInverted = false;
  String _deviceName = "";
public:
  bool currentState = false;
  long deviceONtime = 0;
  OutputDevice(int pin, String deviceName, bool isInverted = false)
  {
    _pin = pin;
    _isInverted = isInverted;
    _deviceName = deviceName;
    pinMode(_pin, OUTPUT);
    SetOff();
  }
  void Run()
  {
    if(currentState) deviceONtime++;
    else deviceONtime=0;
  }
  void SetOn(String reason="")
  {
    if(reason !="") Serial.println("\n Device "+_deviceName+" Turned ON Due to "+reason);
    _isInverted ? digitalWrite(_pin, LOW) : digitalWrite(_pin, HIGH);
    currentState = true;
  }
  void SetOff(String reason="")
  {
    if(reason !="") Serial.println("\n Device "+_deviceName+" Turned OFF Due to "+reason);
    _isInverted ? digitalWrite(_pin, HIGH) : digitalWrite(_pin, LOW);
    currentState = false;
  }
};