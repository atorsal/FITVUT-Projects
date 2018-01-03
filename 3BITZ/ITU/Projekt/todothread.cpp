#include "todothread.h"
#include "activity.h"
#include <time.h>
#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <ctime>

Todothread::Todothread(std::string name):list_name(name)
{

}

int Todothread::timeToSeconds(std::string time_string){

    int h, m,s= 0;
    int secs = 0;

    if (sscanf(time_string.c_str(), "%d:%d:%d", &h, &m,&s) >= 2)
    {
      int secs = h *3600 + m*60 + s;

      return secs;
    }

    return secs;
}

std::string Todothread::secondsToTimeString(int seconds){

    time_t sec_time(seconds);
    tm *p = gmtime(&sec_time);
    QString activityTime = QString::number(p->tm_hour)+":"+QString::number(p->tm_min)+":"+QString::number(p->tm_sec);

    return activityTime.toStdString();

}

void Todothread::run(){

    static int runs = 1;
    this->todo_list = w->getTodoListByName(this->list_name);
    std::vector<Activity> list_activities = todo_list->getActivities();
    size_t todo_list_done = 0;
    bool on_start = true;
    while(1){


        for(size_t i=0; i < runs; i++){

            int hours = list_activities[i].getRunningHours();
            int minutes = list_activities[i].getRunningMinutes();
            int sec = list_activities[i].getRunningSeconds();

            if((hours == 0) && (minutes == 0) && (sec == 0)){
                todo_list_done++;

                if(i == runs - 1){
                    emit activity_done();
                    sleep(1);
                    runs++;
                }
            }

            QString t_string = QString::number(hours) + ":" + QString::number(minutes)+":"+QString::number(sec);
            int seconds = timeToSeconds(t_string.toStdString());
            if(on_start){

            }
            else{
                seconds -= 1 ;
                if(seconds <=0 ){
                    seconds = 0;
                }
            }

            std::string t = secondsToTimeString(seconds);

            int hour, min, s= 0;

            if(sscanf(t.c_str(), "%d:%d:%d", &hour, &min, &s) >= 2)
            {
              list_activities[i].setRunningHours(hour);
              list_activities[i].setRunningMinutes(min);
              list_activities[i].setRunningSeconds(s);
            }



        }
        on_start = false;
        emit runningListItems(list_activities);
        if(todo_list_done == list_activities.size()){

            emit todoListDone();
            return;
        }
        todo_list_done = 0;
        sleep(1);
        //seconds -= 20;


    }
}














