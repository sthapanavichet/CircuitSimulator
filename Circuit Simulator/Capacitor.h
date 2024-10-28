//Capacitor.h - header file for the capacitor

#ifndef _CAPACITOR_H_
#define _CAPACITOR_H_

#include "Component.h"

class Capacitor : public Component {
private:
    double capacitance; // Capacitance in farads
    double voltage; // Voltage across the capacitor

public:
    Capacitor(double cap, float r, float g, float b, std::string compName);
    std::string GetName() const override;
    void Update(double _current, double _voltage) override;
    double GetVoltage(double _current, double timestep) override;
    void Display() override;
};

#endif // _CAPACITOR_H_

