#ifndef CATEGORY_H
#define CATEGORY_H

#include <iostream>
#include <vector>
#include "activity.h"

class Category {
    std::string name;
    std::vector<Activity> activities;

    public:
    Category(std::string name);
    std::string getName();
    void setName(std::string new_name);
    int getActivitiesCount();
    void addActivity(Activity & act);
    void removeActivity(std::string name);
    Activity * getActivity(std::string name);
    std::vector<Activity> getAllActivities();
};

#endif // CATEGORY_H
