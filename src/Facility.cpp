#include "Facility.h"

using namespace std;
#include <map>

map<FacilityCategory, string> categoryToString = {{FacilityCategory::ECONOMY, "ECONOMY"},
                                                  {FacilityCategory::ENVIRONMENT, "ENVIRONMENT"},
                                                  {FacilityCategory::LIFE_QUALITY, "LIFE_QUALITY"}};

map<FacilityStatus, string> statusToString = {{FacilityStatus::OPERATIONAL, "OPERATIONAL"},
                                              {FacilityStatus::UNDER_CONSTRUCTIONS, "Environment"}};

FacilityType::FacilityType(
    const string &name,
    const FacilityCategory category,
    const int price,
    const int lifeQuality_score,
    const int economy_score,
    const int environment_score) : name(name),
                                   category(category),
                                   price(price),
                                   lifeQuality_score(lifeQuality_score),
                                   economy_score(economy_score),
                                   environment_score(environment_score)
{
}

int FacilityType::getCost() const {
    return price;
}

const string &FacilityType::getName() const {
    return name;
}

Facility::Facility(
    const string &name,
    const string &settlementName,
    const FacilityCategory category,
    const int price,
    const int lifeQuality_score,
    const int economy_score,
    const int environment_score) : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
                                   settlementName(settlementName)
{
    status = FacilityStatus::UNDER_CONSTRUCTIONS;
    timeLeft = price;
}
Facility::
    Facility(const FacilityType &type, const string &settlementName) : FacilityType(type), settlementName(settlementName)
{
    status = FacilityStatus::UNDER_CONSTRUCTIONS;
    timeLeft = type.getCost();
}

const string &Facility::getSettlementName() const
{
    return settlementName;
}

const int Facility::getTimeLeft() const
{
    return timeLeft;
}

FacilityStatus Facility::step()
{
    timeLeft--;
    if (timeLeft == 0)
    {
        setStatus(FacilityStatus::OPERATIONAL);
    }
}

void Facility::setStatus(FacilityStatus status)
{
    this->status = status;
}

const FacilityStatus &Facility::getStatus() const
{
    return status;
}

const string Facility::toString() const
{
    return "Name: " + name + "\n" +
           "Category: " + categoryToString[category] + "\n" +
           "Price: " + to_string(price) + "\n" +
           "Life Quality Score: " + to_string(lifeQuality_score) + "\n" +
           "Economy Score: " + to_string(economy_score) + "\n" +
           "Environment Score: " + to_string(environment_score) + "\n" +
           "Status: " + statusToString[status] + "\n" +
           "Time Left" + to_string(timeLeft) + "\n" +
           "Settlement Name" + settlementName;
}