#include "sensors.hpp"

Sensors::Sensors()
{
    oneWire = OneWire(ONE_WIRE_BUS);
    sensor = DallasTemperature(&oneWire);
    x0 = 0.0f;
    x100 = 100.0f;
}

void Sensors::setup()
{
  // locate devices on the bus
  ESP_LOGD(_logTAG, "Locating sensor devices...");
  sensor.begin();
  ESP_LOGD(_logTAG, "Found %d devices.", sensor.getDeviceCount());

  // report parasite power requirements
  ESP_LOGD(_logTAG, "Parasite power is: %s", sensor.isParasitePowerMode() ? "ON" : "OFF");
 
  if (!sensor.getAddress(insideThermometer, 0))
  {
    ESP_LOGE(_logTAG, "Unable to find address for Device 0");
  }
  else
  {
    printAddress(insideThermometer);
  }
  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensor.setResolution(insideThermometer, 9);
}

void Sensors::calibrate(float x0, float x100)
{
  this->x0 = x0;
  this->x100 = x100;
}

float Sensors::getTemperature()
{
  float result = DEVICE_DISCONNECTED_C;

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  ESP_LOGD(_logTAG, "Requesting temperatures...");
  sensor.requestTemperaturesByAddress(insideThermometer); // Send the command to get temperatures

  if (sensor.isConnected(insideThermometer)) {
    result = sensor.getTempC(insideThermometer, 3U);

    float m = 100 / (x100 - x0);
    float b = -m * x0;

    result = m * result + b;

    if (result != DEVICE_DISCONNECTED_C)
    {
      ESP_LOGD(_logTAG, "Temperature for the device 1 (index 0) is: %f", result);
    }
    else
    {
      ESP_LOGE(_logTAG, "Error: Could not read temperature data");
    }
  }
  else
  {
    ESP_LOGE(_logTAG, "Error: Device 1 (index 0) is not connected");
  }

  return result;
}


// function to print a device address
void Sensors::printAddress(DeviceAddress deviceAddress)
{
  String address = "";
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) address += "0";
    address += String(deviceAddress[i], HEX);
  }
  ESP_LOGI(_logTAG, "Sensor address: %s", address.c_str());
}
