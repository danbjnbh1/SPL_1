#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include <map>
#include "SelectionPolicy.h"
using std::vector;
using namespace std; 

enum class PlanStatus
{
    AVALIABLE,
    BUSY,
};
//map<PlanStatus, string> planStatusToString; // it did problem of override in the Plan.cpp now all compile without that 

class Plan
{
public:
    Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
    Plan(const Plan &other);
    const int getlifeQualityScore() const;
    const int getEconomyScore() const;
    const int getEnvironmentScore() const;
    void setSelectionPolicy(SelectionPolicy *selectionPolicy);
    void step();
    void printStatus();
    const vector<Facility *> &getFacilities() const;
    const vector<Facility *> &getUnderConstruction() const;
    void addFacility(Facility *facility);
    const string toString() const;
    const string toStringFinish() const;
    const SelectionPolicy& getSelectionPolicy() const;
    const int getID() const;
    const std::string printFacilities() const;
    const std::string printunfinishedFacilities() const;
    const Settlement getSettlement() const;
    void updateStatus();

private:
    int plan_id;
    const Settlement &settlement;
    SelectionPolicy *selectionPolicy; // What happens if we change this to a reference?
    PlanStatus status;
    vector<Facility *> facilities;
    vector<Facility *> underConstruction;
    const vector<FacilityType> &facilityOptions;
    int life_quality_score, economy_score, environment_score;
};