#include <fstream>
#include "States.h"

Values::Values(double Q, string action, State *next)
{
	Qvalue = Q;
	policy = action;
	Nextstate = next;
}

State::State(int Reward)
{
	reward = Reward;
}

void State::getOptimal()
{
	optimalQvalue = 0.0;
	optimalPolicy = "   ";
	for (int i = 0; i < Allvalues.size(); i++)
		if (optimalQvalue <= Allvalues[i].Qvalue) {
			optimalQvalue = Allvalues[i].Qvalue;
			optimalPolicy = Allvalues[i].policy;
			optimalNextState = Allvalues[i].Nextstate;
		}		
}


void States::init()
{
	State *state0 = new State(-1);
	State *state1 = new State(-1);
	State *state2 = new State(-1);
	State *state3 = new State(-1);
	State *state4 = new State(-1);
	State *state5 = new State(-10);
	State *state6 = new State(-1);
	State *state7 = new State(-10);
	State *state8 = new State(-1);
	State *state9 = new State(-10);
	State *state10 = new State(-1);
	State *state11 = new State(-1);

	state0->Allvalues.emplace_back(0., "a12", state1);
	state0->Allvalues.emplace_back(0., "a13", state4);
	states.emplace_back(state0);

	state1->Allvalues.emplace_back(0., "a21", state0);
	state1->Allvalues.emplace_back(0., "a23", state4);
	state1->Allvalues.emplace_back(0., "a12", state5);
	state1->Allvalues.emplace_back(0., "a13", state2);
	states.emplace_back(state1);

	state2->Allvalues.emplace_back(0., "a21", state6);
	state2->Allvalues.emplace_back(0., "a23", state3);
	state2->Allvalues.emplace_back(0., "a31", state1);
	state2->Allvalues.emplace_back(0., "a32", state5);
	states.emplace_back(state2);

	state3->Allvalues.emplace_back(0., "a31", state6);
	state3->Allvalues.emplace_back(0., "a32", state2);
	states.emplace_back(state3);

	state4->Allvalues.emplace_back(0., "a12", state8);
	state4->Allvalues.emplace_back(0., "a13", state7);
	state4->Allvalues.emplace_back(0., "a31", state0);
	state4->Allvalues.emplace_back(0., "a32", state1);
	states.emplace_back(state4);

	state5->Allvalues.emplace_back(0., "a21", state1);
	state5->Allvalues.emplace_back(0., "a23", state2);
	states.emplace_back(state5);

	state6->Allvalues.emplace_back(0., "a12", state2);
	state6->Allvalues.emplace_back(0., "a13", state3);
	state6->Allvalues.emplace_back(0., "a31", state9);
	state6->Allvalues.emplace_back(0., "a32", state10);
	states.emplace_back(state6);

	state7->Allvalues.emplace_back(0., "a31", state4);
	state7->Allvalues.emplace_back(0., "a32", state8);
	states.emplace_back(state7);

	state8->Allvalues.emplace_back(0., "a21", state4);
	state8->Allvalues.emplace_back(0., "a23", state7);
	state8->Allvalues.emplace_back(0., "a31", state10);
	state8->Allvalues.emplace_back(0., "a32", state11);
	states.emplace_back(state8);

	state9->Allvalues.emplace_back(0., "a12", state10);
	state9->Allvalues.emplace_back(0., "a13", state6);
	states.emplace_back(state9);

	state10->Allvalues.emplace_back(0., "a12", state11);
	state10->Allvalues.emplace_back(0., "a13", state8);
	state10->Allvalues.emplace_back(0., "a21", state9);
	state10->Allvalues.emplace_back(0., "a23", state6);
	states.emplace_back(state10);

	state11->Allvalues.emplace_back(0., "a21", state10);
	state11->Allvalues.emplace_back(0., "a23", state8);
	states.emplace_back(state11);
}
double States::maxDelta(double *delta)
{
	double max = 0.;
	for (int i = 0; i < 12; i++)
		if (max < delta[i])
			max = delta[i];
	return max;
}

void States::Qlearning()
{
	ofstream outputfile;
	outputfile.open("Qlearning_results.txt");
	if (outputfile.fail()) {
		perror("myfile.txt");
	}
	outputfile << "*************The results of Q learning*************" << endl << endl;
	double Delta[12];
	double max = 0.;
	for (int explorationLImit = 0; explorationLImit < 10; explorationLImit++)
	{
		numExploration = 1;
		while (numExploration <= explorationLImit)
		{
			lambda = 1.0 / numExploration;

			for (int i = 0; i < 12; i++)  states[i]->getOptimal();
			for (int i = 0; i < 12; i++)
			{
				for (int j = 0; j < states[3]->Allvalues.size(); j++) {
					states[3]->Allvalues[j].Qvalue = 100.;
					states[3]->Allvalues[j].policy = "None";
				}

				for (int j = 0; j < states[i]->Allvalues.size(); j++)
					states[i]->Allvalues[j].Qvalue
					+= lambda * (states[i]->Allvalues[j].Nextstate->reward
						+ gamma * states[i]->Allvalues[j].Nextstate->optimalQvalue
						- states[i]->Allvalues[j].Qvalue);
			}
			numExploration++;
		}
		for (int i = 0; i < 12; i++)  states[i]->getOptimal();

		for (int i = 0; i < 12; i++)
			Delta[i] = 1.;
		numExploitarion = 1;
		while (maxDelta(Delta) > delta)
		{
			for (int i = 0; i < 12; i++)
				Delta[i] = states[i]->optimalQvalue;
			lambda = lambda / numExploitarion;
			for (int i = 0; i < 12; i++)
			{
				for (int j = 0; j < states[3]->Allvalues.size(); j++) {
					states[3]->Allvalues[j].Qvalue = 100.;
					states[3]->Allvalues[j].policy = "None";
				}

				states[i]->optimalQvalue += lambda * (states[i]->optimalNextState->reward
					+ states[i]->optimalNextState->optimalQvalue - states[i]->optimalQvalue);
			}

			for (int i = 0; i < 12; i++)
				Delta[i] = abs(Delta[i] - states[i]->optimalQvalue);

			numExploitarion++;
		}
		
		outputfile << "The optimal Q values and optimal policies are: " << endl << endl;
		for (int i = 0; i < 12; i++)
			outputfile << "State" << i << ": " << states[i]->optimalQvalue << "     "
			<< states[i]->optimalPolicy << endl;
		outputfile << "The number of exploration is : " << numExploration - 1 << endl;
		outputfile << "The number of exploitation is : " << numExploitarion - 1 << endl;
		outputfile << "The total number of iteration is : " << numExploration + numExploitarion - 2 << endl;
		outputfile << "-------------------------------------------------------------------------" << endl << endl;
	}
	outputfile.close();
}

