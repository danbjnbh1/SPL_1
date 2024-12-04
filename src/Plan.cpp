#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <string>
// #include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Plan.h"
using std::vector;
using namespace std;

map<PlanStatus, string> planStatusToString = {{PlanStatus::AVALIABLE, "AVALIABLE"}, {PlanStatus::BUSY, "BUSY"}};

Plan::Plan(
    const int planId,
    const Settlement &settlement,
    SelectionPolicy *selectionPolicy,
    const vector<FacilityType> &facilityOptions) : plan_id(planId),
                                                   settlement(settlement),
                                                   selectionPolicy(selectionPolicy),
                                                   facilityOptions(facilityOptions) {};

Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy->clone()),
      status(other.status),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    for (Facility *facility : other.facilities)
    {
        facilities.push_back(new Facility(*facility));
    }

    for (Facility *facility : other.underConstruction)
    {
        underConstruction.push_back(new Facility(*facility));
    }
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
    cout << "PlanID: " + plan_id << endl;
    cout << "SettlementName: " + getSettlement().getName();
    cout << "PlanStatus: " + planStatusToString[this->status];
    cout << "SelectionPolicy: " + selectionPolicy->toString();
    cout << "LifeQualityScore: " + to_string(life_quality_score);
    cout << "EconomyScore: " + to_string(economy_score);
    cout << "EnvrionmentScore: " + to_string(environment_score);

    // Loop through the facilities vector
    for (const auto &facility : this->getFacilities())
    {
        cout << "FacilityName: " << facility->getName() + "/n";
        cout << "FacilityStatus: " << "OPERATIONAL" << std::endl;
    }

    // Loop through the underConstruction vector
    std::cout << "Under Construction:" << std::endl;
    for (const auto &facility : this->getUnderConstruction())
    {
        std::cout << "Name: " << facility->getName()
                  << ", Status: " << "UNDER_CONSTRUCTIONS" << std::endl;
    }
    switch (status)
    {
    case PlanStatus::AVALIABLE:
        std::cout << "Status: AVAILABLE" << std::endl;
        break;
    case PlanStatus::BUSY:
        std::cout << "Status: BUSY" << std::endl;
        break;
    default:
        std::cout << "Unknown Status" << std::endl;
        break;
    }
}

const string Plan::toString() const
{
    return "Plan ID " + to_string(plan_id) + "\n"
                                             "Settlement name" +
           settlement.getName() + "\n" +
           "The plan's status is " + planStatusToString[status] + "\n" +
           "The life quality score is: " + to_string(life_quality_score) + "\n" +
           "The economy_score is: " + to_string(economy_score) + "\n" +
           "The environment score is: " + to_string(environment_score) + "\n" +
           "The finished facilities are: " + printFacilities() + "\n" +
           "The unfinished are: " + printunfinishedFacilities() + "\n";
}

const string Plan::toStringFinish() const
{
    return "PlanID: " + to_string(plan_id) + "\n"
                                             "SettlementName: " +
           settlement.getName() + "\n" +
           "LifeQuality_Score:  " + to_string(life_quality_score) + "\n" +
           "Economy_Score: " + to_string(economy_score) + "\n" +
           "Environment _Score: " + to_string(environment_score);
}

const std::string Plan::printFacilities() const
{
    std::string result;
    for (const Facility *facility : facilities)
    {
        if (facility)
        {
            result += facility->toString() + ", ";
        }
    }
    // Remove the trailing ", " if the result is not empty
    if (!result.empty())
    {
        result.erase(result.size() - 2); // Remove last two characters
    }
    return result;
}

const std::string Plan::printunfinishedFacilities() const
{
    std::string result;
    for (const Facility *facility : underConstruction)
    {
        if (facility)
        {
            result += facility->toString() + ", ";
        }
    }
    // Remove the trailing ", " if the result is not empty
    if (!result.empty())
    {
        result.erase(result.size() - 2); // Remove last two characters
    }
    return result;
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
    if(this->status == PlanStatus::AVALIABLE)
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

    for (auto it = underConstruction.begin(); it != underConstruction.end();)
    {
        Facility *facility = *it;

        // Advance the facility's construction status
        FacilityStatus status = facility->step();

        // If the facility is now operational, move it to the facilities list
        if (status == FacilityStatus::OPERATIONAL)
        {
            facilities.push_back(facility);   // Add to operational facilities
            it = underConstruction.erase(it); // Remove from underConstruction list
        }
        else
        {
            ++it; // Move to the next facility
        }
    }

    updateStatus();
}

void Plan::updateStatus()
{
    const int maxUnderConstruction = static_cast<int>(settlement.getType()) + 1;
    if (underConstruction.size() == maxUnderConstruction)
    {
        status = PlanStatus::BUSY;
    }
    else
    {
        status = PlanStatus::AVALIABLE;
    }
}
