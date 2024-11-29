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
    settlements = vector<Settlement *>();
    facilitiesOptions = vector<FacilityType>();
}

void Simulation::start()
{
    open();
    while (isRunning)
    {
        string userInput;
        cin >> userInput;
        vector<string> parsedAction = Auxiliary::parseArguments(userInput);
        ActionType actionType = getActionType(parsedAction[0]);
        BaseAction *action;

        switch (actionType)
        {
        case ActionType::STEP:
            int numOfSteps = stoi(parsedAction[1]);
            action = new SimulateStep(numOfSteps);
            break;

        case ActionType::FACILITY:
            action = new AddFacility(parsedAction[1], static_cast<FacilityCategory>(stoi(parsedAction[2])), stoi(parsedAction[3]), stoi(parsedAction[4]), stoi(parsedAction[5]), stoi(parsedAction[6]));
            break;

        case ActionType::CLOSE:
            action = new Close();
            break;

        default:
            break;
        }

        action->act(*this);
        //! check if need to add to log failed action also
        if (action->getStatus() == ActionStatus::COMPLETED)
        {
            this->addAction(action->clone()); //! clone ???
        }
        else
        {
            cout << action->toString() << endl;
        }
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    Plan newPlan(planCounter++, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
}

void Simulation::addAction(BaseAction *action)
{
    //! should trigger action?
    actionsLog.push_back(action->clone());
}

bool Simulation::addSettlement(Settlement *settlement)
{
    if (isSettlementExists(settlement->getName()))
    {
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility)
{
    if (isFacilityExists(facility.getName()))
    {
        return false;
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for (Settlement *sett : settlements)
    {
        if (sett->getName() == settlementName)
        {
            return false;
        }
    }
    return true;
}

bool Simulation::isFacilityExists(const string &facilityName)
{
    for (FacilityType fac : facilitiesOptions)
    {
        if (fac.getName() == facilityName)
        {
            return false;
        }
    }
    return true;
}

//! check about if sett doesn't exist
Settlement &Simulation::getSettlement(const string &settlementName)
{

    for (Settlement *sett : settlements)
    {
        if (sett->getName() == settlementName)
        {
            return *sett;
        }
    }
}

Plan &Simulation::getPlan(const int planID)
{

    for (Plan plan : plans)
    {
        if (plan.getID() == planID)
        {
            return plan;
        }
    }
}

void Simulation::step()
{
    for (Plan plan : plans)
    {
        plan.step();
    }
}

void Simulation::start()
{
    open();
}

void Simulation::open()
{
    isRunning = true;
}

void Simulation::close()
{
    isRunning = false;
    cout << "The simulation has started";
}
