// Inductor.h - header file for Indcutor

#ifndef _INDUCTOR_H_
#define _INDUCTOR_H_
#include "Component.h"

class Inductor : public Component {
private:
    double inductance; // Inductance in henrys
    double current; // Current through the inductor

public:
    Inductor(double ind, float r, float g, float b, std::string compName);
    std::string GetName() const override;
    void Update(double _current, double _voltage) override;
    double GetVoltage(double _current, double timestep) override;
    void Display() override;
};

#endif // _INDUCTOR_H_
