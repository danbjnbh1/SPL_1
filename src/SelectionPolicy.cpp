#include "SelectionPolicy.h"
#include <algorithm>

using namespace std;

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