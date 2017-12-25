#ifndef TODO_H
#define TODO_H

#include "category.h"
#include "activity.h"

class TODO{

public:
    std::string list_name;
    std::vector<Activity> todo_activities;
    int hours;
    int seconds;

public:
    TODO(std::string list_name);
    std::string getName();
    void setName(std::string new_name);
    int getActivitiesCount();
    void addActivity(Activity & act);
    void removeActivity(std::string name);
    Activity * getActivity(std::string name);
    std::string getDuration();
    std::vector<Activity> getActivities();

};

#endif // TODO_H
