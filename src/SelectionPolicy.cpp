#pragma once
#include <vector>
#include "Facility.h"
#include <map>
#include "SelectionPolicy.h"
using std::vector;
using namespace std;


map<FacilityCategory, string> categoryToString = {{FacilityCategory::ECONOMY, "ECONOMY"},
                                                  {FacilityCategory::ENVIRONMENT, "ENVIRONMENT"},
                                                  {FacilityCategory::LIFE_QUALITY, "LIFE_QUALITY"}};

const FacilityType& EconomySelection :: selectFacility(const vector<FacilityType>& facilitiesOptions) 
    {   
        int size = sizeof(facilitiesOptions) / sizeof(facilitiesOptions[0]);

        for(int i = lastSelectedIndex; i < size; (i++)%size)
        {
            if (categoryToString[facilitiesOptions[i].getCategory()] == "ECONOMY")
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
            if (categoryToString[facilitiesOptions[i].getCategory()] == "ENVIRONMENT")
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
