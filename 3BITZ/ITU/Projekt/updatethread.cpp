
#include "updatethread.h"
#include "timetracker.h"
#include "processtracker.h"
#include "activity.h"
#include <cstdlib>

#include <QtDebug>

extern TimeTracker w;
extern bool is_stopped;

UpdateThread::UpdateThread()
{


}

void UpdateThread::run() {

    bool on = true;

       while(1) {

            ProcessTracker *pTracker = new ProcessTracker();

            //get vector of running activities from ps command
            std::vector<Activity> running_activities = pTracker->getProcesses();

            if(on){
                for(size_t i=0; i < running_activities.size(); i++){
                   std::string prog_name = running_activities[i].getName();
                   this->running_programs[prog_name].first = rand() % 150;
                   this->running_programs[prog_name].second = false;

                }
                on = false;
            }
            else{

                std::vector<string> programs;
                for(size_t i =0; i < running_activities.size(); i++){
                    if (std::find(programs.begin(), programs.end(), running_activities[i].getName()) != programs.end()){
                        continue;
                    }
                    else{
                        programs.push_back(running_activities[i].getName());

                    }
                }


                for(ActivePrograms::const_iterator ptr=this->running_programs.begin();ptr!=this->running_programs.end(); ptr++){
                    for(size_t i =0; i < programs.size(); i++){
                        if(ptr->first == programs[i]){

                            this->running_programs[ptr->first].second = true;
                            break;
                        }
                        else{

                             this->running_programs[ptr->first].second = false;
                        }

                    }

                }

                for(size_t i =0; i < programs.size(); i++){
                    bool here = false;
                    for(ActivePrograms::const_iterator ptr=this->running_programs.begin();ptr!=this->running_programs.end(); ptr++){
                        if(programs[i] != ptr->first){
                            here = false;
                        }
                        else{
                            here = true;
                            break;
                        }
                    }

                    if(!here){
                        this->running_programs[programs[i]].second = true;
                        this->running_programs[programs[i]].first = 0;

                    }

                }

                if(!is_stopped){
                    for(ActivePrograms::const_iterator ptr=this->running_programs.begin();ptr!=this->running_programs.end(); ptr++){
                       if(this->running_programs[ptr->first].second){
                           this->running_programs[ptr->first].first +=1;
                       }


                    }
                }

            }
            delete pTracker;

            //notify the main thread
            emit progressChanged(this->running_programs);
            //w.setRunningTime();
            sleep(1);


       }
}
