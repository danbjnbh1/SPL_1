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

const FacilityType& EconomySelection :: selectFacility(const vector<FacilityType>& facilitiesOptions) 
    {   
        int size = sizeof(facilitiesOptions) / sizeof(facilitiesOptions[0]);

        for(int i = lastSelectedIndex; i < size; (i++)%size)
        {
            if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT)
            {
                return facilitiesOptions[i];
            }
        }
    }

const string EconomySelection :: toString() const 
    {
        return "The EconomySelection was selected and the last index is: " + lastSelectedIndex;
    }

EconomySelection *EconomySelection :: clone() const 
{
    return new EconomySelection(*this);
}


const FacilityType& SustainabilitySelection :: selectFacility(const vector<FacilityType>& facilitiesOptions) 
    {   
        int size = sizeof(facilitiesOptions) / sizeof(facilitiesOptions[0]);

        for(int i = lastSelectedIndex; i < size; (i++)%size)
        {
            if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT)
            {
                return facilitiesOptions[i];
            }
        }
    }

const string SustainabilitySelection :: toString() const 
    {
        return "The SustainabilitySelection was selected and the last index is: " + lastSelectedIndex;
    }

SustainabilitySelection *SustainabilitySelection :: clone() const 
{
    return new SustainabilitySelection(*this);
}

NaiveSelection::NaiveSelection()
{
    lastSelectedIndex = 0;
}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return "Naive Selection: " + to_string(lastSelectedIndex);
}

NaiveSelection *NaiveSelection::clone() const
{
    NaiveSelection *clonedObject = new NaiveSelection();
    clonedObject->lastSelectedIndex = this->lastSelectedIndex;
    return clonedObject;
}

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

    const FacilityType& choosenFacility = facilitiesOptions[resultInx];

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
