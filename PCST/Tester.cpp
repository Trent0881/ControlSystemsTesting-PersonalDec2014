#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::ifstream;
using namespace std;
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <fstream>

#define PI 3.1415926535897
#define SimLength 100

int main()
{
	
srand(time(NULL));

float granularity = 30;


std::ofstream output_file; 
output_file.open("Data.csv");
if (!output_file.good()) // Exit program and throw an error if file not found
{
	cout << "ERROR: Data.csv file not openable" << endl;
	return 0;
}

	// System variables (history)
	float Theta1[SimLength] = {0};
	float Omega1[SimLength] = {0};
	float Tau1[SimLength];
	
	float ArmLength1 = 15; // centimeters

	// Initial conditions: arm is motionless.
	Theta1[0] = 2; // degrees
	Omega1[0] = 0;
	Tau1[0] = 0;	
	
	float GravityTorque;
	float gravity = 1; // meters per second^2
	int mass = 1;
	
	float DesiredTheta1[SimLength] = {0};
	float Theta1Err[SimLength] = {0};
	float DesiredOmega1[SimLength] = {0};
	float DesiredTau1[SimLength] = {0};
	
	float NoiseForce = 0; // For now
	float NoiseTorque; // Doesn't need IC
	
	float ControlTorque; 
	float OverallTorque; // Doesn't need IC
	
	int TimeDelay = 0;
	
for (int i = 0; i < SimLength; i++)
{

	// Define the action from gravity (zero if angle is 0 deg)
	GravityTorque = ArmLength1 / 2 * gravity * mass * sin(PI*Theta1[i]/180);
	// Define the action from random noise (wind?)
	NoiseTorque = ArmLength1 * NoiseForce * cos(PI*Theta1[i]/180);
	
	
	Theta1Err[i] = DesiredTheta1[i] - Theta1[i];
	// Define the action from the controller torque (motor?)
	if (abs((int)Theta1Err[i]) > 1)
	{
		if (Theta1Err[i] < 0)
		{
			ControlTorque = Theta1Err[i]*0.131;
			cout << "!!!" << endl;	
		}
		else if (Theta1Err[i] > 0)
		{
			ControlTorque = Theta1Err[i]*0.131;	
			cout << "..." << endl;
		}
	}
	else
	{
		ControlTorque = 0;
		cout << "HUH!" << endl;
	}
	
	OverallTorque = GravityTorque + NoiseTorque + ControlTorque; // + more!
	
	// Define action based on torque
	Tau1[i] = OverallTorque;
	
	if (i > 0)
	{
		//for (int j = 0; j < i; j++)
		Omega1[i] = Omega1[i] + Tau1[i]*12/(mass*ArmLength1);
		

		if (i > 0)
		{
			//for (int k = 0; k < i; k++)
			Theta1[i] = Theta1[i] + Omega1[i];
		}
	}
	Theta1[i+1] = Theta1[i];
	Omega1[i+1] = Omega1[i];
	Tau1[i+1] = Tau1[i];
	
	output_file << Theta1[i] << ", " << Omega1[i] << ", " << Tau1[i] << endl;
	cout << Theta1[i] << endl;
}
	
	for (int i = 0; i < SimLength; i++)
	{
		// Output file
		if (i == 0)
		{
			//output_file << "Signal, Control, Error" << endl;
		}
	//	output_file << inVector[i] << ", " << outVector[i] << ", " << inVector[i]-outVector[i] << endl;
		
	}
	//float signal_energy = 0;
	for (int m = 0; m < SimLength; m++)
	{
		//net_error = net_error + abs( inVector[m]-outVector[m] );
		//signal_energy = signal_energy + abs (inVector[m]);
	}

} // END OF MAIN PROGRAM
