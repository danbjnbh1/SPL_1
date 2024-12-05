#include <vector>
#include "Facility.h"
#include <map>
#include "SelectionPolicy.h"
#include <algorithm>
#include <climits>

using namespace std;

EconomySelection::EconomySelection() : lastSelectedIndex(-1)
{
}

const FacilityType &EconomySelection ::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int size = facilitiesOptions.size();

    for (int i = lastSelectedIndex + 1; i < size; i = (i + 1) % size)
    {
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ECONOMY)
        {
            return facilitiesOptions[i];
        }
    }

    throw runtime_error("No facility found for the given policy");
}

const string EconomySelection ::toString() const
{
    return "eco";
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
    int size = facilitiesOptions.size();

    for (int i = lastSelectedIndex + 1; i < size; i = (i + 1) % size)
    {
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT)
        {
            return facilitiesOptions[i];
        }
    }

    throw runtime_error("No facility found for the given policy");
}

const string SustainabilitySelection ::toString() const
{
    return "env";
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
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return "nve";
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
    for (size_t i = 0; i < facilitiesOptions.size(); i++)
    {
        const FacilityType &facility = facilitiesOptions[i];

        // Calculate balance score for each facility
        int balanceScore = max(LifeQualityScore + facility.getLifeQualityScore(),
                               max(EconomyScore + facility.getEconomyScore(),
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

    EnvironmentScore = EnvironmentScore + choosenFacility.getEnvironmentScore();
    EconomyScore = EconomyScore + choosenFacility.getEconomyScore();
    LifeQualityScore = LifeQualityScore + choosenFacility.getLifeQualityScore();

    return choosenFacility;
}

const string BalancedSelection::toString() const
{
    return "bal";
}

BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(LifeQualityScore,
                                 EconomyScore,
                                 EnvironmentScore);
}

SelectionPolicy *createPolicyByName(string policyName,
                                    int lifeQualityScore,
                                    int economyScore,
                                    int environmentScore)
{
    if (policyName == "nve")
    {
        return new NaiveSelection();
    }
    if (policyName == "bal")
    {
        return new BalancedSelection(lifeQualityScore, economyScore, environmentScore);
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