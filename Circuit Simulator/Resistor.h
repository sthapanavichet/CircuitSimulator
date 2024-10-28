// Resistor.h - Header file for Resistor

#ifndef _RESISTOR_H_
#define _RESISTOR_H_
#include "Component.h"

class Resistor : public Component {
private:
    double resistance; // Resistance in ohms

public:
    Resistor(double res, float r, float g, float b, std::string compName);
    std::string GetName() const override;
    void Update(double _current, double _voltage) override;
    double GetVoltage(double _current, double timestep);
    void Display() override;
};

#endif // _RESISTOR_H_
