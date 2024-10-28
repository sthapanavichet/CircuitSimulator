// Resistor.cpp - Implementation file for the Resistor class

#include "Resistor.h"
#include "AnalogCircuit.h"

Resistor::Resistor(double res, float r, float g, float b, std::string compName) {
    resistance = res;
    name = compName;
    Red = r;
    Green = g;
    Blue = b;
}

std::string Resistor::GetName() const {
    return name;
}

void Resistor::Update(double _current, double _voltage) {
    // Implementation of resistor-specific update logic
}

double Resistor::GetVoltage(double _current, double timestep) {
    return _current * resistance; // V = IR
}

void Resistor::Display() {
    glColor3f(Red, Green, Blue);
}