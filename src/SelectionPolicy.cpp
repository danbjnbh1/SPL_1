#pragma once
#include <vector>
#include "Facility.h"
#include <map>
#include "SelectionPolicy.h"
#include "SelectionPolicy.h"
#include <algorithm>
#include <climits>

using std::vector;
using namespace std;

EconomySelection::EconomySelection() : lastSelectedIndex(-1)
{
}

const FacilityType &EconomySelection ::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int size = sizeof(facilitiesOptions) / sizeof(facilitiesOptions[0]);

    for (int i = lastSelectedIndex; i < size; (++i) % size)
    {
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT)
        {
            return facilitiesOptions[i];
        }
    }
}

const string EconomySelection ::toString() const
{
    return "The EconomySelection was selected and the last index is: " + lastSelectedIndex;
}

EconomySelection *EconomySelection ::clone() const
{
    return new EconomySelection(*this);
}

SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1)
{
}

const FacilityType &SustainabilitySelection ::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int size = sizeof(facilitiesOptions) / sizeof(facilitiesOptions[0]);

    for (int i = lastSelectedIndex; i < size; (++i) % size)
    {
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT)
        {
            return facilitiesOptions[i];
        }
    }
}

const string SustainabilitySelection ::toString() const
{
    return "The SustainabilitySelection was selected and the last index is: " + lastSelectedIndex;
}

SustainabilitySelection *SustainabilitySelection ::clone() const
{
    return new SustainabilitySelection(*this);
}

NaiveSelection::NaiveSelection() : lastSelectedIndex(-1)
{
}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    return facilitiesOptions[++lastSelectedIndex % facilitiesOptions.size()];
}

const string NaiveSelection::toString() const
{
    return "Naive Selection: last selected index is " + to_string(lastSelectedIndex);
}

NaiveSelection *NaiveSelection::clone() const
{
    NaiveSelection *clonedObject = new NaiveSelection();
    clonedObject->lastSelectedIndex = this->lastSelectedIndex;
    return clonedObject;
}

BalancedSelection::BalancedSelection() : LifeQualityScore(0), EconomyScore(0), EnvironmentScore(0) {}

BalancedSelection::BalancedSelection(
    int LifeQualityScore,
    int EconomyScore,
    int EnvironmentScore) : LifeQualityScore(LifeQualityScore),
                            EconomyScore(EconomyScore),
                            EnvironmentScore(EnvironmentScore)
{
}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int minBalanceScore = INT_MAX;
    int resultInx = 0;
    for (size_t i = 0; i < facilitiesOptions.size(); ++i)
    {
        const FacilityType &facility = facilitiesOptions[i];

        // Calculate balance score for each facility
        int balanceScore = max(LifeQualityScore + facility.getLifeQualityScore(), max(EconomyScore + facility.getEconomyScore(),
                                                                                      EnvironmentScore + facility.getEnvironmentScore())) -
                           min(LifeQualityScore + facility.getLifeQualityScore(),
                               min(EconomyScore + facility.getEconomyScore(),
                                   EnvironmentScore + facility.getEnvironmentScore()));

        if (balanceScore < minBalanceScore)
        {
            minBalanceScore = balanceScore;
            resultInx = i;
        }
    }

    const FacilityType &choosenFacility = facilitiesOptions[resultInx];

    EnvironmentScore = EnvironmentScore + choosenFacility.getEconomyScore();
    EconomyScore = EconomyScore + choosenFacility.getEconomyScore();
    LifeQualityScore = LifeQualityScore + choosenFacility.getLifeQualityScore();

    return choosenFacility;
}

const string BalancedSelection::toString() const
{
    return "Balanced Selection: "
           "\nLife Quality Score - " +
           to_string(LifeQualityScore) +
           "\nEconomy Score - " + to_string(EconomyScore) +
           "\nEnvironment Score - " + to_string(EnvironmentScore);
}

BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(LifeQualityScore,
                                 EconomyScore,
                                 EnvironmentScore);
}

SelectionPolicy *createPolicyByName(string policyName)
{
    if (policyName == "nve")
    {
        return new NaiveSelection();
    }
    if (policyName == "bal")
    {
        return new BalancedSelection();
    }
    if (policyName == "eco")
    {
        return new EconomySelection();
    }
    if (policyName == "env")
    {
        return new SustainabilitySelection();
    }

    return nullptr;
}