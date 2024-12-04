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
    cout << "PlanID: " + this->plan_id << endl;
    cout << "SettlementName: " + this->getSettlement().getName();
    cout << "PlanStatus: " + planStatusToString[this->status];
    cout << "SelectionPolicy: " + this->getSelectionPolicy().toString();
    cout << "LifeQualityScore: " + this->getlifeQualityScore();
    cout << "EconomyScore: " + this->getEconomyScore();
    cout << "EnvrionmentScore: " + this->getEnvironmentScore();

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
    // check if the facility is already exists
    for (const Facility *builtFacility : this->facilities)
    {
        if (builtFacility && builtFacility->getName() == facility->getName())
        { // Check if the facility exists and names match
            throw std::runtime_error("Facility already exists");
        }
    }
    for (const Facility *isbuiltFacility : this->underConstruction)
    {
        if (isbuiltFacility && isbuiltFacility->getName() == facility->getName())
        { // Check if the facility exists and names match
            throw std::runtime_error("Facility is being built exists");
        }
    }
    if (this->status == PlanStatus::AVALIABLE)
    {
        underConstruction.push_back(facility);
        int sizeOfUnderConstruction = underConstruction.size();
        int maxOfUnderConstruction = static_cast<int>(this->settlement.getType());
        if (sizeOfUnderConstruction == maxOfUnderConstruction)
        {
            this->status == PlanStatus::BUSY;
        }
    }
    else
    {
        cout << "The plan is busy now, can not build a new facility" << endl;
    }
}

void Plan::step()
{

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

    // Get the settlement type of the current plan
    const Settlement &sett = this->settlement;
    SettlementType type = sett.getType();

    updateStatus();
}

void Plan::updateStatus()
{
    const int maxUnderConstruction = static_cast<int>(this->settlement.getType()) + 1;
    if (underConstruction.size() == maxUnderConstruction)
    {
        status = PlanStatus::BUSY;
    }
    else
    {
        status = PlanStatus::AVALIABLE;
    }
}
