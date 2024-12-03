#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Plan.h"
using std::vector;
using namespace std;
/*
enum class PlanStatus
{
    AVALIABLE,
    BUSY,
};
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

*/

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

const int Plan::getID() const
{
    return plan_id;
}

void Plan::printStatus()
{
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

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    this->selectionPolicy = selectionPolicy;
}

const SelectionPolicy &Plan::getSelectionPolicy() const
{
    return *selectionPolicy;
}

void Plan::step()
{
    // Implementation
}

/*
class Plan
{
public:

    void addFacility(Facility *facility);

    void step();

*/
