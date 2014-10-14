#include <iostream>
#include "rv.h"
#include "event.h"

// Simulates an General queueing system.  The simulation terminates once 500000 customers depart from the system.
int main(){
	using namespace std;
	enum { ARR, DEP1, DEP2, DEP3 };     	// Define the 1 arrival for 3 queues and 3 final departure rates
	double lambda = 0.0;         		// for Arrival rate
	double mu1 = 0.0;              		// Service rate for queue 1
	double mu2 = 0.0;              		// Service rate for queue 2
	double mu3 = 0.0;              		// Service rate for queue 3
	double rs1 = 0.0;			//probabilty for arriving customer enters queue 1
	double rs2 = 0.0;			//probabilty for arriving customer enters queue 2
	double r11 = 0.0;			//probabilty for arriving customer depart queue 1, enters queue 1
	double r13 = 0.0;			//probabilty for arriving customer depart queue 1, enter queue 3
	double r3d = 0.0;			//probabilty for arriving customer departs queue 3, leaves network
	double r32 = 0.0;			//probabilty for arriving customer departs queue 3, ,goes back to queue 2

	double clock = 0.0;           		// System clock
	int Ndep = 0; 				//to generate overall dept
	double area1 = 0.0;			//area of the region for queue 1
	double area2 = 0.0;			//area of the region for queue 2
	double area3 = 0.0;			//area of the region for queue 3

	double rho = 0.0;
	double eNSimulation1 = 0.0;		//expected number of customer for queue 1
	double eNSimulation2 = 0.0;		//expected number of customer for queue 2
	double eNSimulation3 = 0.0;		//expected number of customer for queue 3

	double eTSimulation1 = 0.0;		//expected time for queue 1
	double eTSimulation2 = 0.0;		//expected time for queue 2
	double eTSimulation3 = 0.0;		//expected time for queue 3
	double throughputSim1 = 0.0;		//throughput for queue 1
	double throughputSim2 = 0.0;		//throughput for queue 2
	double throughputSim3 = 0.0;		//throughput for queue 3

	double utilizationSim1 = 0.0;		//utilization for queue 1
	double utilizationSim2 = 0.0;		//utilization for queue 2
	double utilizationSim3 = 0.0;		//utilization for queue 3

	//required for theoritical calculation
	double eNTheoritical1 = 0.0;		//expected number of customer for queue 1
	double eNTheoritical2 = 0.0;		//expected number of customer for queue 2
	double eNTheoritical3 = 0.0;		//expected number of customer for queue 3
	double eTTheoritical1 = 0.0;		//expected time for queue 1
	double eTTheoritical2 = 0.0;		//expected time for queue 2
	double eTTheoritical3 = 0.0;		//expected time for queue 3
	double throughputTheo1 = 0.0;		//through put for queue 1
	double throughputTheo2 = 0.0;		//through put for queue 2
	double throughputTheo3 = 0.0;		//through put for queue 3
	double utilizationTheo1 = 0.0;		//utilization for queue 1
	double utilizationTheo2 = 0.0;		//utilization for queue 2
	double utilizationTheo3 = 0.0;		//utilization for queue 3

	EventList Elist;              		// Create event list

	int currentCustCount1; 			//current customer in queue 1
	int currentCustCount2; 			//current customer in queue 2
	int currentCustCount3;  		//current customer in queue 3
	int totalArr1;  			//total number of arrivals in queue 1
	int totalArr2		 		//total number of arrivals in queue 2
	int totalArr3;  			//total number of arrivals in queue 3
	int totalDept1;  			//total number of departures in queue 1
	int totalDept2;  			//total number of departures in queue 2
	int totalDept3;  			//total number of departures in queue 3
	int done;  				//total number of departure for all queues



	//code to enter the value from user
	cout << "Enter rs1 (probabilty for arriving customer enters queue 1) value - ";
	cin >> rs1;
	cout << "Enter value of rs2 (probabilty for arriving customer enters queue 2) value - ";
	cin >> rs2;
	cout << "Enter value of r11 (probabilty for arriving customer depart queue 1, enters queue 1) value - ";
	cin >> r11;
	cout << "Enter value of r13 (probabilty for arriving customer depart queue 1, enter queue 3) value - ";
	cin >> r13;
	cout << "Enter value of r3d (probabilty for arriving customer departs queue 3, leaves network) value - ";
	cin >> r3d;
	cout << "Enter value of r32 (probabilty for arriving customer departs queue 3, ,goes back to queue 2) value - ";
	cin >> r32;
	cout << "Enter value of mu1 (Service time of cutomer in queue 1) value - ";
	cin >> mu1;
	cout << "Enter value of mu2 (Service time of cutomer in queue 2) value - ";
	cin >> mu2;
	cout << "Enter value of mu3 Service time of cutomer in queue 3) value - ";
	cin >> mu3;
	//rho = 0.1;
	while (lambda < 10) {
		lambda += 1; //increment to next lambda value
		cout << endl;
		cout << endl;
		cout << "for values of lambda " << lambda << "  values are given as " << endl;
		cout << endl;
		//to reset all the values 
		clock = 0.0;           // System clock
		currentCustCount1 = 0;
		currentCustCount2 = 0;
		currentCustCount3 = 0;
		totalArr1 = 0;
		totalArr2 = 0;
		totalArr3 = 0;
		totalDept1 = 0;
		totalDept2 = 0;
		totalDept3 = 0;
		area1 = 0.0;
		area2 = 0.0;
		area3 = 0.0;
		utilizationSim1 = 0.0;
		utilizationSim2 = 0.0;
		utilizationSim3 = 0.0;
		eNSimulation1 = 0.0;
		eNSimulation2 = 0.0;
		eNSimulation3 = 0.0;
		eTSimulation1 = 0.0;
		eTSimulation2 = 0.0;
		eTSimulation3 = 0.0;
		throughputSim1 = 0.0;
		throughputSim2 = 0.0;
		throughputSim3 = 0.0;
		done = 0;
		Ndep = 0;
		Event* CurrentEvent;
		Elist.insert(exp_rv(lambda), ARR); // Generate first arrival event for queue 1

		while (!done){
			CurrentEvent = Elist.get();               // Get next Event from list
			double prev = clock;                      // Store old clock value
			clock = CurrentEvent->time;               // Update system clock 
			//to find the System Utilization
			//for queue 1
			if (currentCustCount1 > 0) {
				utilizationSim1 += (clock - prev); 
			}
			//for queue 2
			if (currentCustCount2 > 0) {
				utilizationSim2 += (clock - prev); 
			}
			//for queue 3
			if (currentCustCount3 > 0) {
				utilizationSim3 += (clock - prev); 
			}
			switch (CurrentEvent->type) {
			case ARR:                                 // If arrival is administrator jobs
				//to find area of the region
				area1 += currentCustCount1 * (clock - prev);
				area2 += currentCustCount2 * (clock - prev);
				area3 += currentCustCount3 * (clock - prev);
				//calculate which queue the arrival will be next
				if (uni_rv() < rs1) {
					
					//process arrival at queue 1
					currentCustCount1++; 
					totalArr1++; 

					if (currentCustCount1 == 1) {
						
						Elist.insert(clock + exp_rv(mu1), DEP1);
					}
				}
				else {
					
					//process arrival at queue 2
					currentCustCount2++; 
					totalArr2++; 

					if (currentCustCount2 == 1) {
						Elist.insert(clock + exp_rv(mu2), DEP2);
					}
				}
				//generate next arrival
				Elist.insert(clock + exp_rv(lambda), ARR); //  generate next arrival

				break;
			case DEP1:                                 
				//departure for queue 1
				area1 += currentCustCount1 * (clock - prev);
				area2 += currentCustCount2 * (clock - prev);
				area3 += currentCustCount3 * (clock - prev);
				currentCustCount1--; 
				totalDept1++; 

				if (currentCustCount1 > 0) {
					Elist.insert(clock + exp_rv(mu1), DEP1);
				}

				//the departing customer can go to either queue 3 or back to this queue
				if (uni_rv() < r11) {
					
					//process arrival at queue 1
					currentCustCount1++; 
					totalArr1++;

					if (currentCustCount1 == 1) {
						Elist.insert(clock + exp_rv(mu1), DEP1);
					}
				}
				else {
					//process arrival at queue 3
					currentCustCount3++; 
					totalArr3++; 

					if (currentCustCount3 == 1) {
							Elist.insert(clock + exp_rv(mu3), DEP3);
					}
				}
				break;
			case DEP2:                                 // If departure
				//departure for queue 2
				area1 += currentCustCount1 * (clock - prev);
				area2 += currentCustCount2 * (clock - prev);
				area3 += currentCustCount3 * (clock - prev);
				currentCustCount2--; 
				totalDept2++; 

				if (currentCustCount2 > 0) {
					Elist.insert(clock + exp_rv(mu2), DEP2);
				}

				//process arrival at queue 3
				currentCustCount3++; 
				totalArr3++; 

				if (currentCustCount3 == 1) {
					Elist.insert(clock + exp_rv(mu3), DEP3);
				}
				break;
			case DEP3:                                 // If departure
				//departure for queue 3
				area1 += currentCustCount1 * (clock - prev);
				area2 += currentCustCount2 * (clock - prev);
				area3 += currentCustCount3 * (clock - prev);
				currentCustCount3--; 
				totalDept3++; 

				if (currentCustCount3 > 0) {
					Elist.insert(clock + exp_rv(mu3), DEP3);
				}

				//the departing customer can either leave the queueing network or go to queue 2
				if (uni_rv() < r3d) {
					Ndep++;
				}
				else {
					//process arrival at queue 2
					currentCustCount2++; 
					totalArr2++; 

					if (currentCustCount2 == 1) {
							Elist.insert(clock + exp_rv(mu2), DEP2);
					}
				}
				break;
			}
			if (Ndep > 500000) done = 1;        // End condition
		}
		//theta values for theoretical values
		double theta1 = (lambda * rs1) / (1 - r11);
		double theta2 = ((lambda * rs2 )+ (theta1 * r11 * r32)) / (1 - r32);
		double theta3 = (theta1 * r13) + theta2;

		//rho values for theoretical values
		double rho1 = theta1 / mu1;
		double rho2 = theta2 / mu2;
		double rho3 = theta3 / mu3;

		//expected number calculation for each queue
		eNSimulation1 = area1 / clock;
		eNSimulation2 = area2 / clock;
		eNSimulation3 = area3 / clock;

		//expected time in each queue
		eTSimulation1 = area1 / totalArr1;
		eTSimulation2 = area2 / totalArr2;
		eTSimulation3 = area3 / totalArr3;

		//utilization for each queue
		utilizationSim1 = utilizationSim1 / clock;
		utilizationSim2 = utilizationSim2 / clock;
		utilizationSim3 = utilizationSim3 / clock;

		//throughput for each queue (taken at the departure)
		throughputSim1 = totalDept1 / clock;
		throughputSim2 = totalDept2 / clock;
		throughputSim3 = totalDept3 / clock;
		
		cout << "************* Simulation results are as follows ********************" << endl;
		cout << endl;
		
		cout << "Expected number of jobs for queue 1 in a system E[N] is : " << eNSimulation1 << endl;
		cout << "Expected number of jobs for queue 2 in a system E[N] is : " << eNSimulation2 << endl;
		cout << "Expected number of jobs for queue 3 in a system E[N] is : " << eNSimulation3 << endl;

		cout << "Expected time for queue 1 in a system E[T] is : " << eTSimulation1 << endl;
		cout << "Expected time for queue 2 in a system E[T] is : " << eTSimulation2 << endl;
		cout << "Expected time for queue 3 in a system E[T] is : " << eTSimulation3 << endl;

		cout << "Utilization for queue 1 is : " << utilizationSim1 << endl;
		cout << "Utilization for queue 2 is : " << utilizationSim2 << endl;
		cout << "Utilization for queue 3 is : " << utilizationSim3 << endl;

		cout << "Throughput for queue 1 is : " << throughputSim1 << endl;
		cout << "Throughput for queue 2 is : " << throughputSim2 << endl;
		cout << "Throughput for queue 3 is : " << throughputSim3 << endl;

		cout << "************* Theoretical results are as follows ********************" << endl;
		cout << endl;
		cout << "Expected number of jobs for queue 1 in a system E[N] is : " << rho1 / (1 - rho1) << endl;
		cout << "Expected number of jobs for queue 2 in a system E[N] is : " << rho2 / (1 - rho2) << endl;
		cout << "Expected number of jobs for queue 3 in a system E[N] is : " << rho3 / (1 - rho3) << endl;

		cout << "Expected time for queue 1 in a system E[T] is : " << (rho1 / (1 - rho1)) / theta1 << endl;
		cout << "Expected time for queue 2 in a system E[T] is : " << (rho2 / (1 - rho2)) / theta2 << endl;
		cout << "Expected time for queue 3 in a system E[T] is : " << (rho3 / (1 - rho3)) / theta3 << endl;

		cout << "Utilization for queue 1 is : " << rho1 << endl;
		cout << "Utilization for queue 2 is : " << rho2 << endl;
		cout << "Utilization for queue 3 is : " << rho3 << endl;

		cout << "Throughput for queue 1 is : " << theta1 << endl;
		cout << "Throughput for queue 2 is : " << theta2 << endl;
		cout << "Throughput for queue 3 is : " << theta3 << endl;

	}
	fflush(stdin);
	getchar();
}
