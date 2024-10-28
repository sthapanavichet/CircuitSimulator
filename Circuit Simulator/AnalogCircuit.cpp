//AnalogCircuit.cpp - function definitions for the analog circuit
//
// 20-May-22  M. Watler         Created.
// 27-May-22  M. Watler         Added graphical display.

#include <iomanip>//setw
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>//M_PI
#include <stdlib.h>//abs

#include "AnalogCircuit.h"
#include "Capacitor.h"
#include "Inductor.h"
#include "Resistor.h"

using namespace std;

int xpos, ypos;
int windowWidth, windowHeight;
double scalingFactor;

void start() {
	cout << "BEGIN" << endl;
	AnalogCircuit circuit("RLC.dat");
	circuit.run();
	cout << "Press any key + ENTER to end..." << endl;
	char key;
	cin >> key;
	glutLeaveMainLoop();
	cout << "DONE!" << endl;
}

void renderComponent(float x, float y, Component *component) {
	glRasterPos2f(x, y);  // Position of the text

	// Loop over each character and display it
	for (char c : component->GetName()) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

	component->Display();
	glBegin(GL_LINES);
	glVertex2f(x + component->GetName().length() * 14, y);
	glVertex2f(x + component->GetName().length() * 14 + 30, y);
	glEnd();
	glFlush();
	glColor3f(1.0, 1.0, 1.0);
}

void AnalogCircuit::display() {//draw a point on the screen
	//	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	glVertex2f(xpos, ypos);
	glEnd();
	glFlush();
}

AnalogCircuit::AnalogCircuit(string filename) {//dump data to filename, initialize variables
	// Open the file to dump data
	I = 0;

	fout.open(filename);
	if (!fout.is_open()) {
		std::cerr << "Unable to open file: " << filename << std::endl;
	}
	else {
		fout << std::setw(10) << "time"
			<< std::setw(12) << "Voltage"
			<< std::setw(12) << "Current"
			<< std::setw(12) << "V_cap"
			<< std::setw(12) << "V_ind"
			<< std::setw(12) << "V_res"
			<< std::endl;
		fout << std::fixed << std::setprecision(7);
	}
}

void AnalogCircuit::run() {
	component.push_back(new Capacitor(0.000100, 0.0, 1.0, 0.0, "C1"));//100uF, Green
	component.push_back(new Inductor(0.020, 0.0, 0.0, 1.0, "L1"));//20mH, Blue
	component.push_back(new Resistor(10, 1.0, 0.0, 0.0, "R1"));//10ohm, Red

	//Horizontal line
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2f(windowWidth * 0.15, windowHeight * scalingFactor / 2);
	glVertex2f(windowWidth, windowHeight * scalingFactor / 2);
	glEnd();
	glFlush();

	//Vertical line
	
	glBegin(GL_LINES);
	glVertex2f(windowWidth * 0.15, 0);
	glVertex2f(windowWidth * 0.15, windowHeight * scalingFactor);
	glEnd();
	glFlush();

	//Horizontal line markers
	int numOsc = freq * timeMax * 2; // number of times the wave will oscillate, we want the horizontal maker every half wavelength
	float period = windowWidth * 0.85 / numOsc; // the line starts from 15% of the window width
	float startX = windowWidth * 0.15;
	glBegin(GL_LINES);
	for (int l = 0; l < numOsc; l++) {
		glVertex2f(startX + period * l, windowHeight * scalingFactor / 2 + 10);
		glVertex2f(startX + period * l, windowHeight * scalingFactor / 2 - 10);
	}
	glEnd();
	glFlush();

	//Vertical line markers
	float intervals = 8; // intervals of vertical line makers from -Vpeak to Vpeak
	float startY = (windowHeight * (scalingFactor - 1)) / 2;
	float vertGap = windowHeight / intervals;
	glBegin(GL_LINES);
	for (int l = 0; l < intervals + 1; l++) {
		glVertex2f(windowWidth * 0.15 + 10, startY + vertGap * l);
		glVertex2f(windowWidth * 0.15 - 10, startY + vertGap * l);
	}
	glEnd();
	glFlush();

	//Display each component's name and colour
	int y = windowHeight;
	for (list<Component*>::iterator it = component.begin(); it != component.end(); ++it) {
		renderComponent(0, y, *it);
		y -= 20; // each line is 10 pixels long because the font is 12
	}

	//Run the simulation for the first 0.06 seconds (timeMax is 0.1 sec)
	//Dump data to a file as well as display on the screen
	for (double time = 0.0; time < 0.6 * timeMax; time += T) {
		double V = Vpeak * sin(2.0 * M_PI * freq * time);
		ypos = (windowHeight * (V / Vpeak)) / 2.0 + (scalingFactor * windowHeight / 2.0);
		xpos = windowWidth * (0.15 + 0.85 * (time / timeMax));
		
		glColor3f(1.0, 1.0, 1.0); //white
		display();
		
		fout << setw(12) <<time;
		CostFunctionV(I, V);
	}

	//Run the simulation to the end (timeMax is 0.1 sec)
	//Dump data to a file as well as display on the screen
	for (double time = 0.6 * timeMax; time < timeMax; time += T) {
		double V = 0.0;
		xpos = windowWidth * (0.15 + 0.85 * (time / timeMax));
		
		fout << setw(12) << time;
		CostFunctionV(I, V);
	}
}

void AnalogCircuit::CostFunctionV(double& current, double voltage) {
	double I1 = current;
	double J1 = 0.0;
	double J0 = 0.0;
	double alpha = 1.0;

	do {
		double sumVoltage = 0.0;
		list<Component*>::iterator it;
		for (it = component.begin(); it != component.end(); ++it) {
			double Vcomponent = (*it)->GetVoltage(I1, T);
			sumVoltage += Vcomponent;
		}
		J1 = sumVoltage - voltage;
		//Reduce alpha if J1 and J0 are of opposite signs or are equal to each other
		if ((abs(J0 - J1) != (J0 - J1)) || J0 == J1)alpha /= 2.0;

		if (abs(J1) > tolerance) {
			if (J1 < 0) {//increase the current
				I1 += alpha;
			}
			else {
				I1 -= alpha;
			}
			J0 = J1;
		}
		if (alpha < tolerance / 1000000.0) {//reset alpha
			alpha = ((double)rand() / (RAND_MAX)) + 0.5;//between 0.5 and 1.5
		}
	} while (abs(J1) > tolerance);

	fout << setw(12) << voltage;
	fout << setw(12) << I1;
	list<Component*>::iterator it;
	for (it = component.begin(); it != component.end(); ++it) {
		double compVoltage = (*it)->GetVoltage(I1, T);
		fout << setw(12) << compVoltage;
		ypos = (windowHeight * (compVoltage / Vpeak)) / 2.0 + (scalingFactor * windowHeight / 2.0); // calculate the y position of the voltage 
		(*it)->Display();  // set the color to the component's color
		display();  // display the voltage on the graph
		(*it)->Update(I1, compVoltage);  // update any necessary information of the component
	}
	fout << endl;

	current = I1;
}

AnalogCircuit::~AnalogCircuit() {//perform cleanup
	fout.close();
}
