#include <iostream>
#include <fstream>
#include "Simulation.h"
#include "Action.h"
#include "Auxiliary.h"
#include "Plan.h"
#include "SelectionPolicy.h"

using namespace std;

Simulation::Simulation(const string &configFilePath) : isRunning(false),
                                                       planCounter(0),
                                                       actionsLog(),
                                                       plans(),
                                                       settlements(),
                                                       facilitiesOptions()
{
    initSimulation(configFilePath);
}

Simulation::Simulation(const Simulation &other) : isRunning(other.isRunning),
                                                  planCounter(other.planCounter),
                                                  actionsLog(),
                                                  plans(other.plans),
                                                  settlements(),
                                                  facilitiesOptions(other.facilitiesOptions)
{
    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }

    for (Settlement *sett : other.settlements)
    {
        settlements.push_back(new Settlement(*sett));
    }
}

Simulation::Simulation(Simulation &&other) : isRunning(other.isRunning),
                                             planCounter(other.planCounter),
                                             actionsLog(move(other.actionsLog)),
                                             plans(move(other.plans)),
                                             settlements(move(other.settlements)),
                                             facilitiesOptions(move(other.facilitiesOptions))
{
}

Simulation::~Simulation()
{
    for (BaseAction *action : actionsLog)
    {
        delete action;
    }

    for (Settlement *sett : settlements)
    {
        delete sett;
    }
}

const Simulation &Simulation::operator=(const Simulation &other)
{
    if (this == &other)
    {
        return *this;
    }

    for (BaseAction *action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();

    for (Settlement *sett : settlements)
    {
        delete sett;
    }
    settlements.clear();

    isRunning = other.isRunning;
    planCounter = other.planCounter;
    plans = vector<Plan>(other.plans);
    facilitiesOptions = vector<FacilityType>(other.facilitiesOptions);

    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }

    for (Settlement *sett : other.settlements)
    {
        settlements.push_back(new Settlement(*sett));
    }

    return *this;
}

const Simulation &Simulation::operator=(Simulation &&other)
{
    if (this == &other)
    {
        return *this;
    }

    for (BaseAction *action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();

    for (Settlement *sett : settlements)
    {
        delete sett;
    }
    settlements.clear();

    isRunning = other.isRunning;
    planCounter = other.planCounter;
    actionsLog = move(other.actionsLog);
    plans = move(other.plans);
    settlements = move(other.settlements);
    facilitiesOptions = move(other.facilitiesOptions);

    return *this;
}

void Simulation::initSimulation(const string &configFilePath)
{
    ifstream file(configFilePath);
    string line;

    while (getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        vector<string> lineArgs = Auxiliary::parseArguments(line);
        if (lineArgs[0] == "settlement")
        {
            Settlement *sett = new Settlement(lineArgs[1], static_cast<SettlementType>(stoi(lineArgs[2])));
            settlements.push_back(sett);
        }
        else if (lineArgs[0] == "facility")
        {
            FacilityType fac(lineArgs[1], static_cast<FacilityCategory>(stoi(lineArgs[2])), stoi(lineArgs[3]), stoi(lineArgs[4]), stoi(lineArgs[5]), stoi(lineArgs[6]));
            facilitiesOptions.push_back(fac);
        }
        else if (lineArgs[0] == "plan")
        {
            Settlement &settlement = getSettlement(lineArgs[1]);
            SelectionPolicy *selectionPolicy = createPolicyByName(lineArgs[1]);

            plans.push_back(Plan(planCounter++, settlement, selectionPolicy, facilitiesOptions));
        }
    }
}

void Simulation::start()
{
    open();
    while (isRunning)
    {
        string userInput;
        getline(cin, userInput);
        vector<string> parsedAction = Auxiliary::parseArguments(userInput);
        ActionType actionType = getActionType(parsedAction[0]);
        BaseAction *action;

        switch (actionType)
        {
        case ActionType::STEP:
            action = new SimulateStep(stoi(parsedAction[1]));
            break;

        case ActionType::PLAN:
            action = new AddPlan(parsedAction[1], parsedAction[2]);
            break;

        case ActionType::FACILITY:
            action = new AddFacility(parsedAction[1], static_cast<FacilityCategory>(stoi(parsedAction[2])), stoi(parsedAction[3]), stoi(parsedAction[4]), stoi(parsedAction[5]), stoi(parsedAction[6]));
            break;

        case ActionType::SETTLEMENT:
            action = new AddSettlement(parsedAction[1], static_cast<SettlementType>(stoi(parsedAction[2])));
            break;

        case ActionType::CHANGE_POLICY:
            action = new ChangePlanPolicy(stoi(parsedAction[1]), parsedAction[2]);
            break;

        case ActionType::PLAN_STATUS:
            action = new PrintPlanStatus(stoi(parsedAction[1]));
            break;

        case ActionType::LOG:
            action = new PrintActionsLog();
            break;

        case ActionType::BACKUP:
            action = new BackupSimulation();
            break;

        case ActionType::RESTORE:
            action = new RestoreSimulation();
            break;

        case ActionType::CLOSE:
            action = new Close();
            break;

        default:
            break;
        }

        action->act(*this);
        addAction(action);
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    Plan newPlan(planCounter++, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action->clone());
}

bool Simulation::addSettlement(Settlement *settlement)
{
    if (isSettlementExists(settlement->getName())) //? check if this to handle error here or inside the action
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
            return true;
        }
    }
    return false;
}

bool Simulation::isFacilityExists(const string &facilityName)
{
    for (FacilityType fac : facilitiesOptions)
    {
        if (fac.getName() == facilityName)
        {
            return true;
        }
    }
    return false;
}

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

bool Simulation::isPlanExists(const int &planId) const
{
    for (Plan plan : plans)
    {
        if (plan.getID() == planId)
        {
            return true;
        }
    }
    return false;
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

void Simulation::open()
{
    isRunning = true;
    cout << "The simulation has started";
}

void Simulation::close()
{
    isRunning = false;
}
