#pragma once

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#ifndef ONE_WIRE_BUS
#define ONE_WIRE_BUS 13 // GPIO13
#endif

class Sensors
{
private:
    const char* _logTAG = "Sensors";
    OneWire oneWire;
    DallasTemperature sensor;
    DeviceAddress insideThermometer;
    float x0;
    float x100;

    void printAddress(DeviceAddress deviceAddress);

    public:
        Sensors();
        void setup();
        float getTemperature();
        void calibrate(float x0, float x100);
};