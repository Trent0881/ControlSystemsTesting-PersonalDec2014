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
#define SimLength 300

int main()
{
srand(45);
int ang_freq = 2;
int spike;

float granularity = 30;

float K_p = 0.9;
float K_i = 0.166667;
float K_d = 0.03333;

int best_case_error = 50000;

std::ofstream output_file; 
output_file.open("Data.csv");
	if (!output_file.good()) // Exit program and throw an error if file not found
	{
		cout << "ERROR: Data.csv file not openable" << endl;
		return 0;
	}
	
	// SIMULATION
	
	int inVector[SimLength];
	int outVector[SimLength] = {0};
	int nextVal;
	int integral_of_error = 0;
	int errorVector[SimLength] = {0};
	int TimeDelay = 0;
	int deriv_of_error = 0;
	float net_error = 0;
	
for (int i = 0; i < SimLength; i++)
{
	if ( (i/10)%2 == 1 )
	{
		spike =  30 * (sin( 4 * ang_freq * (PI * i/180)));
	}
	else
	{
		spike = 0;
	}
	inVector[i] =  ((sin( ang_freq * (PI * i/180))   + 1) + 50 + spike ) * sin( 3 * ang_freq * (PI * (i-17)/180)) + 20;

	if (i > SimLength*0.9)
	{
		inVector[i] = 50;
	}
}

	for (int i = 0; i < SimLength; i++)
	{
		if (i > 0)
		{

		errorVector[i] = inVector[i] - outVector[i-1];
		
		nextVal = errorVector[i]*K_p + integral_of_error*K_i + deriv_of_error*K_d;
		
		integral_of_error = integral_of_error + errorVector[i];
		
		deriv_of_error = errorVector[i] - errorVector[i-1];	
		// Above is the desired next value. Non physical.
		}
		else
		{
			nextVal = 0;
		}
		
		for (int k = 1; k < (SimLength - i) ; k++)
		{
			outVector[i+k-1+TimeDelay] = outVector[i+k-1+TimeDelay] + nextVal/(k);
		}
	}
	
	for (int i = 0; i < SimLength; i++)
	{
		// Output file
		if (i == 0)
		{
			output_file << "Signal, Control, Error" << endl;
		}
		output_file << inVector[i] << ", " << outVector[i] << ", " << inVector[i]-outVector[i] << endl;
		
	}
	float signal_energy = 0;
	for (int m = 0; m < SimLength; m++)
	{
		net_error = net_error + abs( inVector[m]-outVector[m] );
		signal_energy = signal_energy + abs (inVector[m]);
	}
	
	cout << (net_error)/signal_energy << " and " << net_error; 

} // END OF MAIN PROGRAM
