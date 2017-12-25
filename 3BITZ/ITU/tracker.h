#ifndef TRACKER_H
#define TRACKER_H

#include <vector>
#include "category.h"
#include "todo.h"

class Tracker {
  std::vector<Category> categories;
  std::vector<TODO> todos;

public:
  void addCategory(std::string name);
  Category * getCategory(std::string name);
  void removeCategory(std::string name);
  void renameCategory(std::string name, std::string new_name);
};

#endif // TRACKER_H
