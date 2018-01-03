#ifndef PROCESSTRACKER_H
#define PROCESSTRACKER_H

#include <iostream>
#include <vector>
#include <map>
#include "activity.h"

#define BUF_SIZE 1024

class ProcessTracker {
    FILE *shell;
    std::map<std::string, std::string> progs;
    public:
        ProcessTracker();
        ~ProcessTracker();
        std::vector<Activity> getProcesses(bool system = false);
        void getInstalledPrograms();
        std::map<std::string,std::pair<int,bool>> running_programs;
};


#endif // PROCESSTRACKER_H
