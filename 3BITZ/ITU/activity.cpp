#include "activity.h"

std::string Activity::getName() {
    return name;
}

int Activity::getPid() {
    return pid;
}

int Activity::getRunningHours() {
    return hours;
}

int Activity::getRunningMinutes() {
    return minutes;
}

int Activity::getRunningSeconds() {
    return seconds;
}

void Activity::setRunningHours(int hours){
    this->hours = hours;
}

void Activity::setRunningMinutes(int minutes){
    this->minutes = minutes;
}

void Activity::setRunningSeconds(int seconds){
    this->seconds = seconds;
}

int Activity::getNumberOfRuns(){
    return runs;
}
