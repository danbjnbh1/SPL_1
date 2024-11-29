#include "Action.h"
#include <map>

using namespace std;

ActionType getActionType(string &actionTypeStr)
{
    map<string, ActionType> stringToEnum = {
        {"step", ActionType::STEP},
        {"plan", ActionType::PLAN},
        {"planStatus", ActionType::PLAN_STATUS},
        {"changePolicy", ActionType::CHANGE_POLICY},
        {"settlement", ActionType::SETTLEMENT},
        {"facility", ActionType::FACILITY},
        {"backup", ActionType::BACKUP},
        {"restore", ActionType::RESTORE},
        {"close", ActionType::CLOSE},
        {"log", ActionType::LOG},

    };

    return stringToEnum[actionTypeStr];
}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation)
{
    simulation.step();
}

void AddFacility::act(Simulation &simulation)
{
    FacilityType newFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    const bool succeeded = simulation.addFacility(newFacility);
    if (!succeeded)
    {
        error("Facility already exists");
    }
    else
    {
        complete();
    }
}