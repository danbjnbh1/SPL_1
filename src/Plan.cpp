#pragma once
#include <vector>
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

Plan::Plan(
    const int planId,
    const Settlement &settlement,
    SelectionPolicy *selectionPolicy,
    const vector<FacilityType> &facilityOptions,): 
        planId(planId),
        settlement(settlement),
        selectionPolicy(selectionPolicy),
        facilityOptions(facilityOptions)
    {};

Plan::Plan(const Plan &other): other(other){};

const int Plan::getlifeQualutyScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return  economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

const vertor<Facility*> Plan::getFacilities() const
{
        return facilities;
}

const int::Plan const //there is a commen in the plan.h didn't get it
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
    return  "Plan ID " + plan_id + "\n" +
            "Settlement " + settlement + "\n" +
            "The plan's status is " + printStatus() + "\n" +
            "The life quality score is: " + life_quality_score + "\n" +
            "The economy_score is: " + economy_score + "\n" +
            "The environment score is: " + environment_score + "\n" +
            "The finished facilities are: " + printFacilities() + "\n" +
            "The unfinished are: " + printunfinishedFacilities() + "\n";
}

const std::string Plan::printFacilities()const
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
const std::string Plan::printunfinishedFacilities()const
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


/*    
class Plan
{
public:

    void addFacility(Facility *facility);

    void step();
    
*/

