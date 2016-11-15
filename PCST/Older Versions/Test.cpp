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
float optimal_p;
float optimal_i;
float optimal_d;

int ang_freq = 2;
int spike;
int inVector[SimLength];
		
int gain = 1;

float granularity = 100;

int best_case_error = 50000;

std::ofstream output_file; 
output_file.open("Data.csv");
if (!output_file.good()) // Exit program and throw an error if file not found
{
	cout << "ERROR: Data.csv file not openable" << endl;
	return 0;
}
		
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

	
for (float K_p = 0; K_p < granularity; K_p++)
{
	for (float K_i = 0; K_i < granularity; K_i++)
	{
		for (float K_d = 0; K_d < granularity; K_d++)
		{
			// A SINGLE SIMULATION
			int outVector[SimLength] = {0};
			int nextVal;
			int integral_of_error = 0;
			int errorVector[SimLength];
			int TimeDelay = 1;
			int deriv_of_error = 0;
			int net_error = 0;
			float perc_change;
			int temp;
			for (int i = 0; i < SimLength; i++)
			{
				errorVector[i] = inVector[i] - outVector[i-1];
				
				perc_change = (float)errorVector[i]/((float)(inVector[i]+outVector[i-1]));

				if (fabs(perc_change) >= 0.2)
				{
					//cout << "Integral tracking stopped" << endl;
					temp = 0;
					//cout << "YES: " << perc_change << endl;
				}
				else
				{
					temp = K_i;
					//cout << perc_change << endl;
				}
				nextVal = gain*(errorVector[i]*K_p/granularity + integral_of_error*temp/granularity + deriv_of_error*K_d/granularity);
				
				integral_of_error = integral_of_error + errorVector[i];
				
				deriv_of_error = errorVector[i] - errorVector[i-1];	
				//cout << nextVal << endl;
				// Above is the desired next value. Non physical.
				
				for (int k = 1; k < (SimLength - i) ; k++)
				{
					outVector[i+k-1+TimeDelay] = outVector[i+k-1+TimeDelay] + nextVal/(k*k);
				}
				
				//cout << integral_of_error << endl;
			}
			
			
			for (int m = 0; m < SimLength; m++)
			{
				net_error = net_error + abs( inVector[m]-outVector[m] );
			}
			if ( abs(net_error) < abs(best_case_error) )
			{
				cout << net_error << endl;
				best_case_error = net_error;			
				optimal_p = K_p/granularity;
				optimal_i = K_i/granularity;
				optimal_d = K_d/granularity;
				if ( abs(best_case_error) <= 2000)
				{
					cout << "BCE is " << best_case_error << " and it is at Kp, Ki, Kd of " << optimal_p << ", " << optimal_i << ", " << optimal_d << endl; 
					return 0;
				}
			}
		integral_of_error = 0;
			
		//cout << "50 done" << endl;
	}
	}
	cout << "2500 done" << endl;
	cout << "BCE is " << best_case_error << " and it is at Kp, Ki, Kd of " << optimal_p << ", " << optimal_i << ", " << optimal_d << endl;
}

			
} // END OF MAIN PROGRAM
