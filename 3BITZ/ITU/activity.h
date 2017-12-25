#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <iostream>

class Activity {
    std::string name;
    int pid;
    int hours;
    int minutes;
    int seconds;
    int runs;

public:
    Activity(std::string name, int pid, int hours, int minutes,int seconds,int runs) : name{name}, pid{pid}, hours{hours}, minutes{minutes} ,seconds{seconds},runs{runs}{}
    std::string getName();
    int getPid();
    int getRunningHours();
    int getRunningMinutes();
    int getRunningSeconds();
    void setRunningHours(int hours);
    void setRunningMinutes(int minutes);
    void setRunningSeconds(int seconds);
    int getNumberOfRuns();

};

#endif // ACTIVITY_H
