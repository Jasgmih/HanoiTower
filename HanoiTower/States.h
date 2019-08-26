#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;
class State;
class Values {
public:
	Values(double Q, string action, State *next);
	double Qvalue;
	string policy;
	State *Nextstate;
};

class State {
public:
	State(int Reward);
	void getOptimal();
	double optimalQvalue;
	string optimalPolicy;
	State *optimalNextState;
	int reward;
	vector <Values> Allvalues;
};


class States
{
public:
	void init();
	void Qlearning();
private:
	vector <State *> states;
	double gamma = 0.9;
	double lambda = 1.;
	double delta = 0.0001;
	int numExploration = 1;
	int numExploitarion = 1;
	double maxDelta(double *delta);
};

