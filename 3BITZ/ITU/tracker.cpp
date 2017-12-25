#include "tracker.h"
#include <algorithm>

void Tracker::addCategory(std::string name) {
    categories.emplace_back(name);
}

Category * Tracker::getCategory(std::string name) {
    for (auto & cat : categories) {
        if (cat.getName() == name) {
            return &cat;
        }
    }

    return nullptr;
}

void Tracker::removeCategory(std::string name) {
    categories.erase(std::remove_if(categories.begin(), categories.end(), [&](Category c) { return c.getName() == name; }), categories.end());
}

void Tracker::renameCategory(std::string name, std::string new_name) {
    Category * c = getCategory(name);
    if (!c) return;
    c->setName(new_name);

}
