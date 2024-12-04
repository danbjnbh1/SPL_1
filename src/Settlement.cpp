#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "Settlement.h"

using namespace std;

class Facility; 


map<SettlementType, string> settlementToString = {{SettlementType::VILLAGE, "VILLAGE"},
                                                  {SettlementType::CITY, "CITY"},
                                                  {SettlementType::METROPOLIS, "METROPOLIS"}};

Settlement::Settlement
    (const string &name, SettlementType type) : name(name), type(type) {}

const string& Settlement :: getName() const
{
    return name;
}

SettlementType Settlement :: getType() const
{
    return type;
} 

const string Settlement :: toString() const
{
    
    return "Settlement Details: \n" 
            "Name of the Settlement: " + name + "\n" +
            "Settlement type is " + settlementToString[type];
    
}
