#include "Action.h"
#include "Facility.h"
#include <map>
#include <iostream> 
extern Simulation *backup;

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

const map<ActionStatus, string> actionStatusToString = {{ActionStatus::COMPLETED, "COMPLETED"}, {ActionStatus::ERROR, "ERROR"}};

// BaseAction start --------------------------------------
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
    cout << errorMsg << endl;
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}
// BaseAction end ----------------------------------------

// SimulateStep start --------------------------------------
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation)
{
    simulation.step();
    complete();
}

const string SimulateStep::toString() const
{
    return "step" + to_string(numOfSteps) + actionStatusToString[getStatus()];
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}
// SimulateStep end ----------------------------------------

// AddFacility start --------------------------------------
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) : facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore)
{
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

const string AddFacility::toString() const
{
    return "facility" +
           facilityName +
           facilityCategoryToString[facilityCategory] +
           to_string(price) +
           to_string(lifeQualityScore) +
           to_string(economyScore) +
           to_string(environmentScore) +
           actionStatusToString[getStatus()];
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}
// AddFacility end ----------------------------------------

// AddPlan start --------------------------------------
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation)
{
    if (!simulation.isSettlementExists(settlementName))
    {
        error("Cannot create this plan");
        return;
    }

    Settlement &settlement = simulation.getSettlement(settlementName);
    SelectionPolicy *selectionPolicy = createPolicyByName(this->selectionPolicy);

    if (selectionPolicy == nullptr)
    {
        error("Cannot create this plan");
        return;
    }
    simulation.addPlan(settlement, selectionPolicy);
    complete();
}

const string AddPlan::toString() const
{
    return "plan" +
           settlementName +
           selectionPolicy +
           actionStatusToString[getStatus()];
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}
// AddPlan end ----------------------------------------

// BackupSimulation start --------------------------------------
void BackupSimulation::act(Simulation &simulation)
{
    backup = new Simulation(simulation);
    complete();
}

const string BackupSimulation::toString() const
{
    return "backup";
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}
// BackupSimulation end ----------------------------------------

// AddSettlement start ----------------------------------------
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName), settlementType(settlementType) {};
const string AddSettlement::toString() const
{
    return "settlement" + settlementName + to_string(static_cast<int>(settlementType)) + actionStatusToString[getStatus()];
};

void AddSettlement::act(Simulation &simulation)
{
    if (simulation.isSettlementExists(settlementName))
    {
        error("Settlement already exists");
        return;
    }

    simulation.addSettlement(new Settlement(settlementName, settlementType));
    complete();
}

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName), settlementType(settlementType) {};

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this);
}
// AddSettlement end ----------------------------------------

// ChangePlanPolicy start ----------------------------------------
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) : planId(planId), newPolicy(newPolicy)
{
}

void ChangePlanPolicy::act(Simulation &simulation)
{
    if (!simulation.isPlanExists(planId))
    {
        error("Cannot change selection policy");
        return;
    }

    Plan &plan = simulation.getPlan(planId);

    if (plan.getSelectionPolicy().toString() == newPolicy)
    {
        error("Cannot change selection policy");
        return;
    }

    SelectionPolicy *newSelectionPolicy;
    newSelectionPolicy = new BalancedSelection(plan.getlifeQualityScore(), plan.getEconomyScore(), plan.getEnvironmentScore());

    if (newSelectionPolicy == nullptr)
    {
        error("Cannot change selection policy");
        return;
    }

    plan.setSelectionPolicy(newSelectionPolicy);
}

const string ChangePlanPolicy::toString() const
{
    return "changePolicy" + to_string(planId) + newPolicy;
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}
// ChangePlanPolicy end ----------------------------------------