#include "Action.h"
#include "SelectionPolicy.h"
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

map<ActionStatus, string> actionStatusToString = {{ActionStatus::COMPLETED, "COMPLETED"}, {ActionStatus::ERROR, "ERROR"}};

// BaseAction start --------------------------------------
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::ERROR) {}

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
    cout << "Error: " + errorMsg << endl;
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
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const
{
    return "SimulationStep " + to_string(numOfSteps) + " " + actionStatusToString[getStatus()];
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
    return "AddFacility " +
           facilityName +
           facilityCategoryToString[facilityCategory] + " " +
           to_string(price) + " " +
           to_string(lifeQualityScore) + " " +
           to_string(economyScore) + " " +
           to_string(environmentScore) + " " +
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
    return "AddPlan " +
           settlementName + " " +
           selectionPolicy + " " +
           actionStatusToString[getStatus()];
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}
// AddPlan end ----------------------------------------

// BackupSimulation start --------------------------------------
BackupSimulation::BackupSimulation()
{
    // Implementation
}

void BackupSimulation::act(Simulation &simulation)
{
    backup = new Simulation(simulation);
    complete();
}

const string BackupSimulation::toString() const
{
    return "backup " + actionStatusToString[getStatus()];
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}
// BackupSimulation end ----------------------------------------

// BackupSimulation start --------------------------------------
RestoreSimulation::RestoreSimulation()
{
    // Implementation
}

void RestoreSimulation::act(Simulation &simulation)
{
    simulation = *backup;
    complete();
}

const string RestoreSimulation::toString() const
{
    return "restore " + actionStatusToString[getStatus()];
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}
// BackupSimulation end ----------------------------------------

// AddSettlement start ----------------------------------------
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName), settlementType(settlementType) {};
const string AddSettlement::toString() const
{
    return "AddSettlement " + settlementName + " " + to_string(static_cast<int>(settlementType)) + " " + actionStatusToString[getStatus()];
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
    newSelectionPolicy = createPolicyByName(newPolicy, plan.getlifeQualityScore(), plan.getEconomyScore(), plan.getEnvironmentScore());

    if (newSelectionPolicy == nullptr)
    {
        error("Cannot change selection policy");
        return;
    }

    plan.setSelectionPolicy(newSelectionPolicy);
    complete();
}

const string ChangePlanPolicy::toString() const
{
    return "ChangePlanPolicy " + to_string(planId) + " " +  newPolicy + " " + actionStatusToString[getStatus()];
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}
// ChangePlanPolicy end ----------------------------------------

// PrintPlanStatus start --------------------------------------

PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {};

void PrintPlanStatus::act(Simulation &simulation)
{
    if (!simulation.isPlanExists(planId))
    {
        error("Plan doesn't exist");
        return;
    }

    Plan planToPrint = simulation.getPlan(this->planId);
    planToPrint.printStatus();
    complete();
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const
{
    return "PrintPlanStatus " + to_string(planId) + " " + actionStatusToString[getStatus()];
}

// PrintPlanStatus end --------------------------------------

// PrintActionsLog start ------------------------------------

PrintActionsLog::PrintActionsLog() {};

void PrintActionsLog::act(Simulation &simulation)
{
    vector<BaseAction *> logs = simulation.getActionsLog();
    for (const auto &log : logs)
    {
        cout << log->toString() << endl;
    }
    complete();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const
{
    return "PrintActionsLog " +  actionStatusToString[getStatus()];
}

// PrintActionsLog end ------------------------------------

// Close start ---------------------------------------------

Close::Close() {};

void Close::act(Simulation &simulation)
{
    vector<Plan> plans = simulation.getPlans();
    for (const Plan &plan : plans)
    {
        cout << plan.toString() << endl;
    }
    simulation.close();
    complete();
}

Close *Close::clone() const
{
    return new Close(*this);
}

const string Close::toString() const
{

    return "close " + actionStatusToString[getStatus()];
}
// Close end -----------------------------------------------