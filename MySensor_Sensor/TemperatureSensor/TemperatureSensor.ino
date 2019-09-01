// Enable debug prints to serial monitor
//#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_RF24
#define MY_NODE_ID 1
#define VERSION "1.1"
#define TEMP_SENSOR_ID 0
#define BATTERY_SENSOR_ID 1

#include <MySensors.h>  
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 

int BATTERY_SENSE_PIN = A0;  // select the input pin for the battery sense point
unsigned long SLEEP_TIME = 1*1000; // Sleep time between reads (in milliseconds)
float Battery_Conv_Factor = (1.1*((470.0+120.0)/120.0))/1023;
float Battery_Cut_Off = 3.2;
float Battery_Full = 4.2;
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 

// Initialize temperature message
MyMessage Msg_Temp(TEMP_SENSOR_ID,V_TEMP);

void before()
{
  // Startup up the OneWire library
  sensors.begin();
}

void setup()  
{ 
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
  analogReference(INTERNAL);
}

void presentation() 
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Temperature Sensor", VERSION );
  present(0,S_TEMP,"Sens0 : Temperature",false);
}

void loop()     
{     
  // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();
  
  // query conversion time and sleep until conversion completed
  int16_t conversionTime = sensors.millisToWaitForConversion(sensors.getResolution());
  
  // sleep() call can be replaced by wait() call if node need to process incoming messages (or if node is repeater)
  sleep(conversionTime);
  // get the battery Voltage
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  float batteryV  = sensorValue * Battery_Conv_Factor;
  int batteryPcnt = (((Battery_Full-batteryV))/Battery_Cut_Off)*100.0;
  // Read temperatures and send them to controller 
  // Fetch and round temperature to one decimal
  float temperature = static_cast<float>(static_cast<int>((getControllerConfig().isMetric?sensors.getTempCByIndex(0):sensors.getTempFByIndex(0)) * 10.)) / 10.;
  if (temperature != -127.00 && temperature != 85.00) 
  {
    // Send in the new temperature
    send(Msg_Temp.setSensor(0).set(temperature,1));
  }
  
  sendBatteryLevel(batteryPcnt);
  
  sleep(SLEEP_TIME);
}
