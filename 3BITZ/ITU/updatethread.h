#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H
#include <QThread>
#include "activity.h"
#include "timetracker.h"
using namespace std;

extern bool is_stopped;
class UpdateThread : public QThread
{
    Q_OBJECT
    typedef std::map<std::string,std::pair<int,bool>>  ActivePrograms;
    ActivePrograms running_programs;
    bool stopped;
public:
    UpdateThread();
    void run();
signals:
    void progressChanged(std::map<std::string, std::pair<int, bool> >);

};

#endif // UPDATETHREAD_H
