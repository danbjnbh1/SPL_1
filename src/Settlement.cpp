#pragma once
#include <string>
#include <vector>
#include <iostream>
using std::string;
using std::vector;

using namespace std;

class Facility; 

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    
    private:
            const string name;
            SettlementType type;

    public:
        Settlement(const string &name, SettlementType type)
        : name(name), type(type) {}

        const string &getName() const
        {
            return name;
        }
        SettlementType getType() const
        {
            return type;
        }
        const string toString() const
        {
            cout << "Settlement Details:" << endl;
            cout << "Name of Settlement: " << name << endl;
            cout << "Type of Settlement: " << type << endl;
        }

        
};