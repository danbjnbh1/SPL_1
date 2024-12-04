#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation
{
public:
    Simulation(const string &configFilePath);
    Simulation(const Simulation& other);
    Simulation(Simulation &&other);
    ~Simulation();
    const Simulation& operator=(const Simulation& other);
    const Simulation& operator=(Simulation&& other);

    void start();
    void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
    void addAction(BaseAction *action);
    bool addSettlement(Settlement *settlement);
    bool addFacility(FacilityType facility);
    bool isSettlementExists(const string &settlementName);
    bool isFacilityExists(const string &facilityName);
    bool isPlanExists(const int &planId) const;
    vector<BaseAction *> getActionsLog() const;
    vector<Plan> getPlans() const;
    Settlement &getSettlement(const string &settlementName);
    Plan &getPlan(const int planID);
    void step();
    void close();
    void open();
    void initSimulation(const string &configFilePath);

private:
    bool isRunning;
    int planCounter; // For assigning unique plan IDs
    vector<BaseAction *> actionsLog;
    vector<Plan> plans;
    vector<Settlement *> settlements;
    vector<FacilityType> facilitiesOptions;
};