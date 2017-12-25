#include "todo.h"
#include <algorithm>
#include "category.h"

TODO::TODO(std::string list_name) : list_name{list_name} {}

std::string TODO::getName() {
    return list_name;
}

void TODO::setName(std::string new_name) {
    list_name = new_name;
}


int TODO::getActivitiesCount() {
    return todo_activities.size();
}

void TODO::addActivity(Activity & act) {
    return todo_activities.push_back(act);
}

void TODO::removeActivity(std::string name) {
    todo_activities.erase(std::remove_if(todo_activities.begin(), todo_activities.end(), [&](Activity a) { return a.getName() == name; }), todo_activities.end());
}

Activity * TODO::getActivity(std::string name) {
    for (auto & act: todo_activities) {
        if (act.getName() == name) {
            return &act;
        }
    }

    return nullptr;
}

std::vector<Activity> TODO::getActivities(){
    return todo_activities;
}
