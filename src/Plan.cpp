#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Plan.h"
using namespace std;

map<PlanStatus, string> planStatusToString = {{PlanStatus::AVALIABLE, "AVALIABLE"}, {PlanStatus::BUSY, "BUSY"}};

Plan::Plan(
    const int planId,
    const Settlement &settlement,
    SelectionPolicy *selectionPolicy,
    const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilities(),
      underConstruction(),
      facilityOptions(facilityOptions),
      life_quality_score(0),
      economy_score(0),
      environment_score(0) {};

// copy Constructor
Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    selectionPolicy = other.selectionPolicy->clone();
    // Deep copy of facilities
    for (Facility *facility : other.facilities)
    {
        facilities.push_back(new Facility(*facility));
    }

    // Deep copy of underConstruction
    for (Facility *facility : other.underConstruction)
    {
        underConstruction.push_back(new Facility(*facility));
    }
}

// copy Constructor with settlement reference
Plan::Plan(const Plan &other, Settlement &settlement)
    : plan_id(other.plan_id),
      settlement(settlement),
      selectionPolicy(),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    selectionPolicy = other.selectionPolicy->clone();

    for (Facility *facility : other.facilities)
    {
        facilities.push_back(new Facility(*facility));
    }

    for (Facility *facility : other.underConstruction)
    {
        underConstruction.push_back(new Facility(*facility));
    }
}

// destructor
Plan::~Plan()
{
    delete selectionPolicy;

    for (Facility *facility : facilities)
    {
        delete facility;
    }
    for (Facility *facility : underConstruction)
    {
        delete facility;
    }
}

// Move Operator
Plan::Plan(Plan &&other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy),
      status(other.status),
      facilities(std::move(other.facilities)),
      underConstruction(std::move(other.underConstruction)),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    other.selectionPolicy = nullptr;
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

const vector<Facility *> &Plan::getUnderConstruction() const
{
    return underConstruction;
}

const int Plan::getID() const
{
    return plan_id;
}

void Plan::printStatus()
{
    cout << toString() << endl;

    // Print finished facilities
    for (Facility *facility : facilities)
    {
        cout << facility->toString() << endl;
    }

    // Print underConstruction facilities
    for (Facility *facility : underConstruction)
    {
        cout << facility->toString() << endl;
    }
}

const string Plan::toString() const
{
    return "PlanID: " + to_string(plan_id) + "\n" +
           "SettlementName: " + getSettlement().getName() + "\n" +
           "PlanStatus: " + planStatusToString[this->status] + "\n" +
           "SelectionPolicy: " + selectionPolicy->toString() + "\n" +
           "LifeQualityScore: " + to_string(life_quality_score) + "\n" +
           "EconomyScore: " + to_string(economy_score) + "\n" +
           "EnvironmentScore: " + to_string(environment_score);
}

const string Plan::finalStatus() const
{
    return "PlanID: " + to_string(plan_id) + "\n" +
           "SettlementName: " + getSettlement().getName() + "\n" +
           "LifeQualityScore: " + to_string(life_quality_score) + "\n" +
           "EconomyScore: " + to_string(economy_score) + "\n" +
           "EnvironmentScore: " + to_string(environment_score);
}

const Settlement Plan::getSettlement() const
{
    return this->settlement;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    this->selectionPolicy = selectionPolicy;
}

const SelectionPolicy &Plan::getSelectionPolicy() const
{
    return *selectionPolicy;
}

void Plan::addFacility(Facility *facility)
{
    if (this->status == PlanStatus::AVALIABLE)
    {
        underConstruction.push_back(facility);
        updateStatus();
    }
}

void Plan::step()
{
    while (status == PlanStatus::AVALIABLE)
    {
        FacilityType newFacilityType = selectionPolicy->selectFacility(facilityOptions);
        Facility *newFacility = new Facility(newFacilityType, settlement.getName());
        addFacility(newFacility);
    }

    for (auto iter = underConstruction.begin(); iter != underConstruction.end();)
    {
        Facility *facility = *iter;

        FacilityStatus status = facility->step();

        // If the facility is now operational, move it to the facilities list
        if (status == FacilityStatus::OPERATIONAL)
        {
            life_quality_score += facility->getLifeQualityScore();
            economy_score += facility->getEconomyScore();
            environment_score += facility->getEnvironmentScore();
            facilities.push_back(facility);
            iter = underConstruction.erase(iter);
        }
        else
        {
            ++iter; // Move to the next facility
        }
    }

    updateStatus();
}

void Plan::updateStatus()
{
    const size_t maxUnderConstruction = static_cast<size_t>(settlement.getType()) + 1;

    if (underConstruction.size() == maxUnderConstruction)
    {
        status = PlanStatus::BUSY;
    }
    else
    {
        status = PlanStatus::AVALIABLE;
    }
}
