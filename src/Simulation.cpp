#include <iostream>
#include <fstream>
#include "Simulation.h"
#include "Action.h"
#include "Auxiliary.h"
#include "Plan.h"

using namespace std;

Simulation::Simulation(const string &configFilePath)
{
    isRunning = false;
    planCounter = 0;
    actionsLog = vector<BaseAction *>();
    plans = vector<Plan>();
    settlements = vector<Settlement*>();
    facilitiesOptions = vector<FacilityType>();
}

void Simulation::start()
{
    isRunning = true;
}