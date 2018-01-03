#ifndef TODOTHREAD_H
#define TODOTHREAD_H

#include <todo.h>
#include <QThread>
#include "timetracker.h"

extern TimeTracker *w;
class Todothread:public QThread
{
    Q_OBJECT
    TODO *todo_list;
    std::string list_name;

public:
    Todothread(std::string name);
    int timeToSeconds(std::string time_string);
    std::string secondsToTimeString(int seconds);
    void run();
signals:
    void runningListItems(std::vector<Activity>);
    void todoListDone();
    void activity_done();
};

#endif // TODOTHREAD_H
