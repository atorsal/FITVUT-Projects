#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <unistd.h>

#include <QString>
#include <QtDebug>
#include "processtracker.h"
#include "dirent.h"

ProcessTracker::ProcessTracker() {
    shell = popen("ps -eo uid,pid,etime,comm", "r");
    getInstalledPrograms();
}

ProcessTracker::~ProcessTracker() {
    pclose(shell);
}

std::vector<Activity> ProcessTracker::getProcesses(bool system) {
    std::vector<Activity> procs;
    if (!shell) return procs;

    fscanf(shell, "%*s\t%*s\t%*s\t%*s\n"); // skip header

    int uid, pid;
    char rtime[BUF_SIZE], progname[BUF_SIZE];
    std::string time_str;
    std::tm tm;

    std::string saved_name;

    while (fscanf(shell, "%d\t%d\t%s\t%s\n", &uid, &pid, rtime, progname) == 4) {
        if (!system && (uid == 0)) continue;

        time_str = rtime;

        if (time_str.size() == 5) {
            time_str = "00:" + time_str;
        }

        std::istringstream ss(time_str);
        ss >> std::get_time(&tm, "%H:%M:%S");
        mktime(&tm);

        bool found = false;
        std::string prog = progname;

        for (auto & item : this->progs) {
            if (prog == "sh" || prog == "cat") continue;

            if (item.first.find(prog) != std::string::npos) {
                procs.emplace_back(item.second, pid, tm.tm_hour, tm.tm_min,tm.tm_sec,1);
                found = true;
                break;
            }
        }


        if (!found) {
            continue;
        }
    }

    return procs;
}

void ProcessTracker::getInstalledPrograms() {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("/usr/share/applications")) == NULL)
        return;
    while ((ent = readdir (dir)) != NULL) {
         std::string path = std::string{"/usr/share/applications/"} + ent->d_name;

         std::ifstream myfile (path);
         if (!myfile.is_open())
             continue;

         std::string line;
         size_t index;
         std::string generic_name;
         while ( getline (myfile,line) ) {
             index = line.find('=');
             if (index != std::string::npos) {
                  std::string value = line.substr(index + 1);
                  std::string name = line.substr(0, index);

                  if (name == "Name") {
                      generic_name = value;
                  }

                  if (name == "Exec" && !generic_name.empty()) {
                      this->progs[value] = generic_name;
                      break;
                  }
                }
          }

       myfile.close();
    }

    closedir (dir);
}
