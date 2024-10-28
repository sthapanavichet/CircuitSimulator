// Inductor.cpp - Implementation file for Inductor class

#include "Inductor.h"
#include "AnalogCircuit.h"

Inductor::Inductor(double ind, float r, float g, float b, std::string compName) {
    inductance = ind;
    name = compName;
    current = 0;
    Red = r;
    Green = g;
    Blue = b;
}

std::string Inductor::GetName() const {
    return name;
}

void Inductor::Update(double _current, double _voltage) {
    current = _current;
    // Implementation of inductor-specific update logic
}

double Inductor::GetVoltage(double _current, double timestep) {
    return (inductance * (_current - current)) / timestep;
}

void Inductor::Display() {
    glColor3f(Red, Green, Blue);
}
