// Capacitor.cpp - Implementation file for Capacitor class

#include "Capacitor.h"
#include "AnalogCircuit.h"

Capacitor::Capacitor(double cap, float r, float g, float b, std::string compName) {
    capacitance = cap;
    name = compName;
    voltage = 0;
    Red = r;
    Green = g;
    Blue = b;
}

std::string Capacitor::GetName() const{
    return name;
}

void Capacitor::Update(double _current, double _voltage) {
    voltage = _voltage;
}

double Capacitor::GetVoltage(double _current, double timestep) {
    
    double newVoltage = voltage + ((_current * timestep) / capacitance);
    return newVoltage;
}

void Capacitor::Display() {
    glColor3f(Red, Green, Blue);
}

