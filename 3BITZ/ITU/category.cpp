#include <algorithm>
#include "category.h"

Category::Category(std::string name) : name{name} {}

std::string Category::getName() {
    return name;
}

void Category::setName(std::string new_name) {
    name = new_name;
}

int Category::getActivitiesCount() {
    return activities.size();
}

void Category::addActivity(Activity & act) {
    return activities.push_back(act);
}

void Category::removeActivity(std::string name) {
    activities.erase(std::remove_if(activities.begin(), activities.end(), [&](Activity a) { return a.getName() == name; }), activities.end());
}

Activity * Category::getActivity(std::string name) {
    for (auto & act: activities) {
        if (act.getName() == name) {
            return &act;
        }
    }

    return nullptr;
}

std::vector<Activity> Category::getAllActivities(){
    return this->activities;
}
