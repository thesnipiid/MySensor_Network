// Enable debug prints to serial monitor
//#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_RF24
#define MY_NODE_ID 1
#define VERSION "1.0"

#include <MySensors.h>  
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 

unsigned long SLEEP_TIME = 1000; // Sleep time between reads (in milliseconds)

OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 

// Initialize temperature message
MyMessage msg(0,V_TEMP);

void before()
{
  Serial.print("Launching Before() routine...\n");  
  // Startup up the OneWire library
  sensors.begin();
}

void setup()  
{ 
  // requestTemperatures() will not block current thread
  Serial.print("Setting Up temperature Sensor ...");
  sensors.setWaitForConversion(false);
}

void presentation() 
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Temperature Sensor", VERSION );
}

void loop()     
{     
  // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();
  
  // query conversion time and sleep until conversion completed
  int16_t conversionTime = sensors.millisToWaitForConversion(sensors.getResolution());
  
  // sleep() call can be replaced by wait() call if node need to process incoming messages (or if node is repeater)
  sleep(conversionTime);
  
  // Read temperatures and send them to controller 
  // Fetch and round temperature to one decimal
  float temperature = static_cast<float>(static_cast<int>((getControllerConfig().isMetric?sensors.getTempCByIndex(0):sensors.getTempFByIndex(0)) * 10.)) / 10.;
  
  if (temperature != -127.00 && temperature != 85.00) 
  {
    // Send in the new temperature
    send(msg.setSensor(0).set(temperature,1));
  }
  
  sleep(SLEEP_TIME);
}
