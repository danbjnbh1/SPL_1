#include "Simulation.h"
#include <iostream>
#include <Facility.h>
#include <Settlement.h>

//-------------------------
// tests includes
#include "Facility.h"
//-------------------------

using namespace std;

Simulation *backup = nullptr;

void facilityTest()
{
    Facility *fac1 = new Facility("Fac1", "Set1", FacilityCategory::ENVIRONMENT, 2, 2, 2, 2);
    string s = fac1->toString();
    cout << s << endl;
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        cout << "usage: simulation <config_path>" << endl;
        return 0;
    }
    
    string configurationFile = argv[1];
    cout << configurationFile + " this is the file path" << endl;
    Simulation simulation(configurationFile);
    simulation.start();
/*  
    if (backup != nullptr)
    {
        delete backup;
        backup = nullptr;
    }

    facilityTest();
    cout << "Test print" << endl;
    Settlement *s = new Settlement("dadush", SettlementType::METROPOLIS);
    cout << s->toString() << endl;


    return 0;
    */
}