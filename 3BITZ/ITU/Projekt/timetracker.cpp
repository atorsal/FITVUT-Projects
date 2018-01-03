#include "timetracker.h"
#include "timetracker_ui.h"
#include "todothread.h"
#include "processtracker.h"
#include "activity.h"
#include "updatethread.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include <QTimer>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QQuickView>
#include <QTableWidget>
#include <QPalette>
#include <QString>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QtCharts>
#include <QMediaPlayer>
#include <QShortcut>
#include <unistd.h>
using namespace QtCharts;

TimeTracker::TimeTracker(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TimeTracker)
{

   ui->setupUi(this);
   startThread();
   initCatagories();
   initCostValues();
   initActualCategories();
   initShortcuts();
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateChart()));
    timer->start(100); //time specified in ms
    QTimer *timer_stat = new QTimer(this);
    QObject::connect(timer_stat, SIGNAL(timeout()), this, SLOT(showActivitiesStatistics()));
    timer_stat->start(1000); //time specified in ms
    QTimer *timer2 = new QTimer(this);
    QObject::connect(timer2, SIGNAL(timeout()), this, SLOT(appendCostActivities()));
    timer2->start(6000);

   QObject::connect(ui->start_tracking_button, SIGNAL(clicked()),this, SLOT(tracking_button_clicked()));
   QObject::connect(ui->add_new_list,SIGNAL(clicked()),this,SLOT(on_add_new_list_button_clicked()));
   QObject::connect(ui->add_activity,SIGNAL(clicked()),this,SLOT(on_add_activity_button_clicked()));
   QObject::connect(ui->todo_list_view,SIGNAL(itemSelectionChanged()),this,SLOT(on_todo_lists_item_selected()));
   QObject::connect(ui->delete_list,SIGNAL(clicked()),this,SLOT(on_delete_list_clicked()));
   QObject::connect(ui->remove_activity,SIGNAL(clicked()),this,SLOT(on_delete_activity_button_clicked()));
   QObject::connect(ui->rename_list,SIGNAL(clicked()),this,SLOT(on_rename_list()));

   QObject::connect(ui->start_todo_list,SIGNAL(clicked()),this,SLOT(start_todo_list_clicked()));
   QObject::connect(ui->add_new_category,SIGNAL(clicked()),this,SLOT(new_category_button_clicked()));
   QObject::connect(ui->remove_category,SIGNAL(clicked()),this,SLOT(remove_category_button_clicked()));
   QObject::connect(ui->add_category_item,SIGNAL(clicked()),this,SLOT(new_category_item_clicked()));
   QObject::connect(ui->remove_category_item,SIGNAL(clicked()),this,SLOT(remove_category_item_clicked()));
   QObject::connect(ui->categories_list,SIGNAL(itemSelectionChanged()),this,SLOT(category_selected()));
   QObject::connect(ui->rename_category,SIGNAL(clicked()),this,SLOT(rename_category()));
   QObject::connect(ui->search_activity, SIGNAL(textChanged(const QString &)),this,SLOT(search_activated()));
   QObject::connect(ui->compute,SIGNAL(clicked()),this,SLOT(compute_activity_cost()));
   QObject::connect(ui->trackBtn,SIGNAL(clicked()),this,SLOT(automatic_tracking()));
   QObject::connect(ui->calcBtn,SIGNAL(clicked()),this,SLOT(hide_calculator()));
   QObject::connect(ui->todo_notif_btn,SIGNAL(clicked()),this,SLOT(todo_notification()));



}


TimeTracker::~TimeTracker()
{
    delete ui;
}


void TimeTracker::setRunningTime(){
    this->running_time++;
}

int TimeTracker::getRunningTime(){

    return this->running_time;
}

void TimeTracker::table_update(){
    ui->activities_table->viewport()->repaint();
}

void TimeTracker::search_activated(){

    for(int i =0; i < ui->activities_table->rowCount(); i++){
        ui->activities_table->item(i,0)->setBackgroundColor("#3a3a3a");
        ui->activities_table->item(i,1)->setBackgroundColor("#3a3a3a");
        ui->activities_table->item(i,2)->setBackgroundColor("#3a3a3a");
    }

    if(!(ui->search_activity->text().isEmpty())){
        QString searched_item = ui->search_activity->text();
        QList<QTableWidgetItem *> LTempTable;



        LTempTable = ui->activities_table->findItems(searched_item,Qt::MatchContains);
        QString c;
        QString search_color = ui->search_colors->currentText();

        if(search_color == "yellow") {
            c = "#EECC66";
        }
        else if(search_color == "orange"){
            c = "#fe8c00";
        }
        else if(search_color == "green"){
            c = "#7fbf7f";
        }
        else{
            c = "#6666ff";
        }


        for(int i=0; i < LTempTable.size(); i++){
            int row = LTempTable[i]->row();

            if(LTempTable[i]->column() != 2){
                ui->activities_table->item(row,0)->setBackgroundColor(c);
                ui->activities_table->item(row,1)->setBackgroundColor(c);
                ui->activities_table->item(row,2)->setBackgroundColor(c);
            }
        }
    }


}


void TimeTracker::change_actual_layout(int layout_index){
    this->ui->stackedWidget->setCurrentIndex(layout_index);
}



void TimeTracker::tracking_button_clicked()
{

    if(ui->start_tracking_button->text() == QObject::tr("Start tracking")){
        is_stopped = false;
        ui->start_tracking_button->setText(QObject::tr("Stop tracking"));
        ui->start_tracking_button->setStyleSheet("QPushButton {background-color: red ;color:#ffffff;}");
        this->autoTracking = true;
    }
    else{
         is_stopped = true;
         ui->start_tracking_button->setText(QObject::tr("Start tracking"));
         ui->start_tracking_button->setStyleSheet("QPushButton {background-color: green; color:#ffffff;}");
    }
}



QString TimeTracker::createTimeString(int seconds){
    time_t sec_time(seconds);
    tm *p = gmtime(&sec_time);
    QString activityTime;
    if (p->tm_hour > 0) {
        activityTime += QString::number(p->tm_hour) + " h ";
    }

    if (p->tm_min > 0) {
        activityTime += QString::number(p->tm_min) + " m ";
    }

    activityTime += QString::number(p->tm_sec) + " s";
    return activityTime;

}



void TimeTracker::onProgressChanged(std::map<std::string, std::pair<int, bool> > running_activities){


    QString activity_name;
    QString active_time;
    this->running_acts = running_activities;
    ui->activities_table->setRowCount(running_activities.size());

    int i=0;
    this->setRunningTime();
    QString runtime = this->createTimeString(this->getRunningTime());
    ui->runtime->setText(runtime);

    if(this->autoTracking){



        for(ActivePrograms::const_iterator ptr=running_activities.begin();ptr!=running_activities.end(); ptr++){

            activity_name = QString::fromStdString(ptr->first);

            std::string act_name = ptr->first;

            std::pair<int,bool> it = ptr->second;
            activities[ptr->first] = it.first;

            active_time = this->createTimeString(it.first);

            std::string act_category = "";

            for(CategoriesActs::const_iterator acts = categories_acts.begin(); acts != categories_acts.end();acts++){
                if(act_name == acts->first){
                    act_category = categories_acts[act_name];
                }
            }
            QString category = QString::fromStdString(act_category);

            ui->activities_table->setItem(i,0,new QTableWidgetItem(activity_name));
            ui->activities_table->setItem(i,1,new QTableWidgetItem(category));
            ui->activities_table->setItem(i,2,new QTableWidgetItem(active_time));
            ui->activities_table->item(i,0)->setFlags(ui->activities_table->item(i,0)->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
            ui->activities_table->item(i,1)->setFlags(ui->activities_table->item(i,1)->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
            ui->activities_table->item(i,2)->setFlags(ui->activities_table->item(i,2)->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);

            int font_size = ui->table_fonts->currentText().toInt();

            QFont serifFont("Helvetica",font_size, QFont::Light);
            ui->activities_table->item(i,0)->setFont(serifFont);
            ui->activities_table->item(i,1)->setFont(serifFont);
            ui->activities_table->item(i,2)->setFont(serifFont);
            ui->activities_table->item(i,0)->setTextColor("#ffffff");
            ui->activities_table->item(i,1)->setTextColor("#ffffff");
            ui->activities_table->item(i,2)->setTextColor("#ffffff");


            i++;

        }

     }


    //vyhladavanie v tabulke a zvyrazneni vyhladanych prvkov
    if(!(ui->search_activity->text().isEmpty())){
        QString searched_item = ui->search_activity->text();

        QList<QTableWidgetItem *> LTempTable = ui->activities_table->findItems(searched_item,Qt::MatchContains);

        QString c;
        QString search_color = ui->search_colors->currentText();
        if(search_color == QObject::tr("yellow")) {
            c = "#EECC66";
        }
        else if(search_color == QObject::tr("orange")){
            c = "#fe8c00";
        }
        else if(search_color == QObject::tr("green")){
            c = "#7fbf7f";
        }
        else{
            c = "#6666ff";
        }

        for(int i=0; i < LTempTable.size(); i++){
            int row = LTempTable[i]->row();
            std::cout << LTempTable[i]->column() << std::endl;

            if(LTempTable[i]->column() != 2){
                ui->activities_table->item(row,0)->setBackgroundColor(c);
                ui->activities_table->item(row,1)->setBackgroundColor(c);
                ui->activities_table->item(row,2)->setBackgroundColor(c);
            }
        }

    }

    //ui->activities_table->viewport()->repaint();
}

void TimeTracker::startThread(){

    UpdateThread *workThread = new UpdateThread();

    QObject::connect(workThread,SIGNAL(progressChanged(std::map<std::string, std::pair<int, bool> >)),SLOT(onProgressChanged(std::map<std::string, std::pair<int, bool> >)));
    QObject::connect(workThread, SIGNAL(finished()),workThread, SLOT(deleteLater()));
    workThread->start();

}


bool TimeTracker::getTodoList(std::string name){
    if(todo_lists.size() == 0){
        return false;
    }
    else{
        for(size_t i=0; i < todo_lists.size(); i++){
            if(todo_lists[i].getName() == name){
                return true;
            }
        }
        return false;
    }
}

TODO *TimeTracker::getTodoListByName(std::string name){
    if(todo_lists.size() == 0){
        return nullptr;
    }
    else{
        for(size_t i=0; i < todo_lists.size(); i++){
            if(todo_lists[i].getName() == name){
                return &todo_lists[i];
            }
        }
        return nullptr;
    }
}



void TimeTracker::showListItems(QTableWidget *widget, std::string name){


    widget->setRowCount(0);
    TODO *list = getTodoListByName(name);
    std::vector<Activity> list_activities = list->getActivities();
    widget->setRowCount(list_activities.size());
    int hours;
    int minutes;
    QString duration;
    QString s_hours;
    QString s_minutes;
    QString act_name;

    for(size_t i=0; i < list_activities.size(); i++){
        act_name = QString::fromStdString(list_activities[i].getName());
        hours = list_activities[i].getRunningHours();
        minutes = list_activities[i].getRunningMinutes();
        s_hours = QString::number(hours);
        s_minutes = QString::number(minutes);
        duration = s_hours + " h " + s_minutes + " min";


        widget->setItem(i,0,new QTableWidgetItem(act_name));

        widget->item(i,0)->setTextColor("#000000");
        widget->item(i,0)->setFlags(widget->item(i,0)->flags() ^ Qt::ItemIsEditable);
        widget->setItem(i,1,new QTableWidgetItem(duration));
        widget->item(i,1)->setFlags(widget->item(i,1)->flags() ^ Qt::ItemIsEditable);
        widget->item(i,1)->setTextColor("#000000");

    }
    widget->viewport()->repaint();

}


/*add new todo list */
void TimeTracker::on_add_new_list_button_clicked()
{
    if (ui->new_todo_list_name->text().isEmpty()) {
        QMessageBox::warning(this, tr("TimeTracker"), tr("Unable to create new list. Name of the new TODO list is empty. Please type name of the list."), QMessageBox::Ok);
    }
     else{
        /*find name of new todo list*/
        std::string list_name = ui->new_todo_list_name->text().toStdString();
        /*check if list alredy exists or not*/
        if(getTodoList(list_name)){
        }
        else{
            TODO *todo_list= new TODO(list_name);
            todo_lists.push_back(*todo_list);
        }
        /*list name*/
        QString l_name = QString::fromStdString(list_name);


        /*add new list*/

        QList<QListWidgetItem *> items = ui->todo_list_view->findItems(l_name, Qt::MatchExactly);
        if(items.count() == 0){
           ui->todo_list_view->addItem(l_name);
           ui->todo_list_view->item(ui->todo_list_view->count()-1)->setSelected(true);
        }
        else{
            QMessageBox::warning(this, tr("TimeTracker"), tr("Unable to create new list. List with this name alredy exists."), QMessageBox::Ok);
        }

        ui->listName->setText(l_name);

        showListItems(ui->activityList,list_name);
        ui->new_activity_name->clear();
        QTime time(0,00);
        ui->activity_time->setTime(time);
        ui->new_todo_list_name->clear();
    }

}

void TimeTracker::onRunnigListReceived(std::vector<Activity> act){

    ui->actual_list_activities->setRowCount(act.size());

    for(size_t i=0; i < act.size(); i++){
        size_t all_activities_done = 0;
        QString act_name = QString::fromStdString(act[i].getName());
        int h,m,s;
        h = act[i].getRunningHours();
        m = act[i].getRunningMinutes();
        s = act[i].getRunningSeconds();

        QString act_time = QString::number(h)+" h "+QString::number(m)+" min "+QString::number(s)+" sec";
        ui->actual_list_activities->setItem(i,0,new QTableWidgetItem(act_name));
        ui->actual_list_activities->setItem(i,1,new QTableWidgetItem(act_time));

    }
    ui->actual_list_activities->viewport()->repaint();
}
void TimeTracker::act_done(){

    if(ui->notification_options->currentText() == "notification and sound"){

        notification_sound();
        QMessageBox *done_msg = new QMessageBox(QMessageBox::Information,tr("TimeTracker")," Activity completed",QMessageBox::Ok | QMessageBox::Cancel,this);

        done_msg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        done_msg->setStyleSheet("QLabel{ font-size:24px;qproperty-alignment: AlignCenter;}\
                                 QPushButton{width:200;font-size:18px;}\
                               ");
        done_msg->exec();


    }else if(ui->notification_options->currentText() == "notification"){
        QMessageBox *done_msg = new QMessageBox(QMessageBox::Information,tr("TimeTracker"),"Activity completed",QMessageBox::Ok | QMessageBox::Cancel,this);

        done_msg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        done_msg->setStyleSheet("QLabel{ font-size:24px;qproperty-alignment: AlignCenter;}\
                                 QPushButton{width:200;font-size:18px;}\
                               ");
        done_msg->exec();


    }else{
         notification_sound();

    }
}


void TimeTracker::todoThreadDone(){

    QString list_name = ui->todo_list_view->currentItem()->text();

    if(ui->notification_options->currentText() == "notification and sound"){

        notification_sound();
        QMessageBox *done_msg = new QMessageBox(QMessageBox::Information,tr("TimeTracker"),"TODO list " +list_name+ " completed",QMessageBox::Ok | QMessageBox::Cancel,this);

        done_msg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        done_msg->setStyleSheet("QLabel{ font-size:24px;qproperty-alignment: AlignCenter;}\
                                 QPushButton{width:200;font-size:18px;}\
                               ");
        done_msg->exec();

        ui->actual_todo_list->hide();
    }else if(ui->notification_options->currentText() == "notification"){
        QMessageBox *done_msg = new QMessageBox(QMessageBox::Information,tr("TimeTracker"),"TODO list " +list_name+ " completed",QMessageBox::Ok | QMessageBox::Cancel,this);

        done_msg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        done_msg->setStyleSheet("QLabel{ font-size:24px;qproperty-alignment: AlignCenter;}\
                                 QPushButton{width:200;font-size:18px;}\
                               ");
        done_msg->exec();

        ui->actual_todo_list->hide();
    }else{
         notification_sound();
         ui->actual_todo_list->hide();
    }
    delete this->todothread;
}


void TimeTracker::start_todo_list_clicked(){


    if(ui->todo_list_view->currentItem() != nullptr){
        ui->actual_todo_list->show();
        QListWidgetItem *current = ui->todo_list_view->currentItem();
        std::string current_item_name = current->text().toStdString();
            this->todothread = new Todothread(current_item_name);
            QObject::connect(todothread,SIGNAL(runningListItems(std::vector<Activity>)),SLOT(onRunnigListReceived(std::vector<Activity>)));
            QObject::connect(todothread,SIGNAL(activity_done()),SLOT(act_done()));
            QObject::connect(todothread,SIGNAL(todoListDone()),SLOT(todoThreadDone()));
            todothread->start();
    }
    else{
        QMessageBox::warning(this, tr("TimeTracker"), tr("No list selected."), QMessageBox::Ok);
    }

}





void TimeTracker::on_add_activity_button_clicked()
{

    if (ui->listName->text().isEmpty()) {
         QMessageBox::warning(this, tr("TimeTracker"), tr("Unable to save. Name of the new TODO list is empty. Please type name of the list."), QMessageBox::Ok);
     }
     else if(ui->new_activity_name->text().isEmpty()){
         QMessageBox::warning(this, tr("TimeTracker"), tr("Unable to save. Name of the item in list is empty. Please type activity name."), QMessageBox::Ok);

     }
     else{
         //create new activity and add it to current TODO list
         std::string list_name = ui->listName->text().toStdString();
         std::string activity_name = ui->new_activity_name->text().toStdString();
         int hours = ui->activity_time->time().hour();
         int minutes = ui->activity_time->time().minute();
         if(hours == 0 && minutes == 0){
             QMessageBox::warning(this, tr("TimeTracker"), tr("Please enter time greater than 0."), QMessageBox::Ok);
         }
             else{
             //check if activity already exists
             TODO *new_list = getTodoListByName(list_name);

             if(new_list->getActivity(activity_name) == nullptr){
                 Activity *todo_activity = new Activity(activity_name,0,hours,minutes,0,0);
                 new_list->addActivity(*todo_activity);
             }
             else{
                 QMessageBox::warning(this, tr("TimeTracker"), tr("List already contains activity with this name."), QMessageBox::Ok);
             }
         }
         showListItems(ui->activityList,list_name);
         QTime time(0,00);
         ui->new_activity_name->clear();
         ui->activity_time->setTime(time);
    }

}



void TimeTracker::on_todo_lists_item_selected()
{

    if(ui->todo_list_view->currentItem() != nullptr){
        QListWidgetItem *current = ui->todo_list_view->currentItem();
        std::string current_item_name = current->text().toStdString();

        ui->listName->setText(current->text());
        showListItems(ui->activityList,current_item_name);
        ui->new_activity_name->clear();
        QTime time(0,00);
        ui->activity_time->setTime(time);
    }

}

/*remove todo list from vector of lists*/
void TimeTracker::removeTodoList(string name){

    for(size_t i=0; i < this->todo_lists.size(); i++){
        if(this->todo_lists[i].getName() == name){
            this->todo_lists.erase(this->todo_lists.begin()+i);
        }
    }

}


/*delete selected list from listview*/
void TimeTracker::on_delete_list_clicked(){

    if(ui->todo_list_view->currentItem() != nullptr){

        QListWidgetItem *current = ui->todo_list_view->currentItem();
        std::string current_item_name = current->text().toStdString();

        ui->todo_list_view->takeItem(ui->todo_list_view->row(current));
        this->removeTodoList(current_item_name);

        //TODO odstranit veci z tabulky

        ui->activityList->clear();
        QTableWidgetItem* col1 = new QTableWidgetItem(QString("Activity"),QTableWidgetItem::Type);
        ui->activityList->setHorizontalHeaderItem(0,col1);
        QTableWidgetItem* col2 = new QTableWidgetItem(QString("Activity duration"),QTableWidgetItem::Type);
        ui->activityList->setHorizontalHeaderItem(1,col2);
        //ui->activityList->setRowCount(0);
        //ui->listName->clear();
        int r = ui->todo_list_view->count();
        if(r != 0){
            std::cout << r << std::endl;
            QListWidgetItem *t = ui->todo_list_view->item(r-1);
            //showListItems(ui->activityList,t->text().toStdString());
            ui->todo_list_view->viewport()->repaint();
            ui->listName->setText(t->text());
            showListItems(ui->activityList,t->text().toStdString());
         }
        else{
            ui->todo_list_view->viewport()->repaint();
            ui->listName->clear();
            ui->activityList->setRowCount(0);
        }
    }
}

void TimeTracker::on_rename_list(){
    QListWidgetItem *current = ui->todo_list_view->currentItem();
    if (!current) return;
    bool ok;
    QString new_name = QInputDialog::getText(this, tr("Rename TODO list"),
                                              tr("New TODO list name:"), QLineEdit::Normal,
                                              "", &ok);
    if (!ok) return;

    if (!new_name.isEmpty()){
        std::string current_item_name = current->text().toStdString();
        TODO *cur_list = this->getTodoListByName(current_item_name);
        cur_list->setName(new_name.toStdString());
        current->setText(new_name);
        ui->listName->setText(new_name);

    } else {
        QMessageBox::warning(this, tr("TimeTracker"), tr("List name cannot by empty."), QMessageBox::Ok);
    }

}

void TimeTracker::on_delete_activity_button_clicked(){
    if(!ui->listName->text().isEmpty()){
        std::string list_name = ui->listName->text().toStdString();
        QList<QTableWidgetItem *> selected_activity = ui->activityList->selectedItems();
        std::cout << selected_activity.size() << std::endl;
        for(int i=0; i < selected_activity.size(); i++){

            int row =  ui->activityList->row(selected_activity[i]);

            TODO *current = getTodoListByName(ui->listName->text().toStdString());

            current->removeActivity(selected_activity[i]->text().toStdString());
            ui->activityList->removeRow(row);
            break;

        }
        showListItems(ui->activityList,list_name);
     }
    ui->activityList->viewport()->repaint();


}

Category* TimeTracker::getCategoryByName(std::string name){
    if(categories.size() == 0){
        return nullptr;
    }
    else{
        for(size_t i=0; i < categories.size(); i++){
            if(categories[i].getName() == name){
                return &categories[i];
            }
        }
        return nullptr;
    }
}

bool TimeTracker::getCategory(std::string name){
    if(categories.size() == 0){
        return false;
    }
    else{
        for(size_t i=0; i < categories.size(); i++){
            if(categories[i].getName() == name){
                return true;

            }
        }
        return false;
    }
}


void TimeTracker::showCategoryItems(std::string current_category){

    while(ui->category_items->count()>0)
    {
      ui->category_items->takeItem(0);

    }

    std::vector<Activity> current_category_activities = this->getCategoryByName(current_category)->getAllActivities();

    for(size_t i=0; i < current_category_activities.size(); i++){
        QString name = QString::fromStdString(current_category_activities[i].getName());
        ui->category_items->addItem(name);
    }
    ui->category_items->viewport()->repaint();

}

void TimeTracker::category_selected(){

    if(ui->categories_list->currentItem() != nullptr){
        QListWidgetItem *current = ui->categories_list->currentItem();
        std::string current_item_name = current->text().toStdString();

        ui->selected_category->setText(current->text());
        showCategoryItems(current_item_name);
        ui->new_category_item->clear();
    }

}


void TimeTracker::new_category_button_clicked(){

    if (ui->new_category_name->text().isEmpty()) {
         QMessageBox::warning(this, tr("TimeTracker"), tr("Category name is empty. Please enter category name."), QMessageBox::Ok);
     }
     else{
        /*find new category name*/
        std::string category_name = ui->new_category_name->text().toStdString();


        /*check if list alredy exists or not*/
        if(getCategory(category_name)){

        }
        else{

            Category *category = new Category(category_name);
            categories.push_back(*category);
        }
        /*category name*/
        QString cat_name = QString::fromStdString(category_name);


        /*add new list*/
        QList<QListWidgetItem *> items = ui->categories_list->findItems(cat_name, Qt::MatchExactly);
        if(items.count() == 0){
           ui->categories_list->addItem(cat_name);
           ui->categories_list->item(ui->categories_list->count()-1)->setSelected(true);
           ui->selected_category->setText(ui->new_category_name->text());
        }
        else{
            QMessageBox::warning(this, tr("TimeTracker"), tr("Category already exists."), QMessageBox::Ok);
        }

        ui->new_category_name->clear();

        this->showCategoryItems(category_name);
    }

}

void TimeTracker::removeCategory(std::string name){

    for(size_t i=0; i < categories.size(); i++){
        if(categories[i].getName() == name){
            categories.erase(this->categories.begin()+i);
        }
    }

}


void TimeTracker::remove_category_button_clicked(){

    if(ui->categories_list->currentItem() != nullptr){

        QListWidgetItem *current = ui->categories_list->currentItem();
        std::string current_item_name = current->text().toStdString();
        ui->categories_list->takeItem(ui->categories_list->row(current));
        this->removeCategory(current_item_name);
        std::vector<string> curr_category = category_items[current_item_name];

        for(int i=0; i < curr_category.size(); i++){
            categories_acts[curr_category[i]] = "";
        }

        while(ui->category_items->count()>0)
        {


          ui->category_items->takeItem(0);

        }


        int r = ui->categories_list->count();
        if(r != 0){
            QListWidgetItem *t = ui->categories_list->item(r-1);
            ui->categories_list->viewport()->repaint();
            ui->selected_category->setText(t->text());
            showCategoryItems(t->text().toStdString());
         }
        else{
            ui->categories_list->viewport()->repaint();
            ui->selected_category->clear();
            ui->category_items->clear();
        }


        ui->selected_category->clear();
        ui->categories_list->viewport()->repaint();


    }

}

void TimeTracker::new_category_item_clicked(){
    if(ui->new_category_item->text().isEmpty()){
         QMessageBox::warning(this, tr("TimeTracker"), tr("Category activity name is empty."), QMessageBox::Ok);
    }
    else if(ui->selected_category->text().isEmpty()){
        QMessageBox::warning(this, tr("TimeTracker"), tr("Category name is empty."), QMessageBox::Ok);
    }
    else{
        std::string act_category_name = ui->selected_category->text().toStdString();
        Category *act_category = this->getCategoryByName(act_category_name);
        std::string cat_item_name = ui->new_category_item->text().toStdString();

        QList<QListWidgetItem *> items = ui->category_items->findItems(ui->new_category_item->text(), Qt::MatchExactly);
        if(items.count() == 0){


            categories_acts[cat_item_name] = act_category_name;
            category_items[act_category_name].push_back(cat_item_name);
            Activity *cat_activity = new Activity(cat_item_name,0,0,0,0,0);
            act_category->addActivity(*cat_activity);
            ui->new_category_item->clear();
            showCategoryItems(act_category_name);



        }
        else{
            ui->new_category_item->clear();
            QMessageBox::warning(this, tr("TimeTracker"), tr("Item already exists."), QMessageBox::Ok);
        }

    }

}

void TimeTracker::remove_category_item_clicked(){

    if(ui->category_items->currentItem() != nullptr){

        QListWidgetItem *current = ui->category_items->currentItem();
        std::string current_item_name = current->text().toStdString();

        ui->category_items->takeItem(ui->category_items->row(current));
        Category *current_cat = this->getCategoryByName(ui->selected_category->text().toStdString());
        current_cat->removeActivity(current_item_name);
        categories_acts[current_item_name] = "";

    }


}

void TimeTracker::rename_category(){
    bool ok;
    QListWidgetItem *current = ui->categories_list->currentItem();
    if (!current) return;

    QString new_name = QInputDialog::getText(this, tr("Rename category"),
                                              tr("New category name:"), QLineEdit::Normal,
                                              "", &ok);
    if (!ok) return;

    if (!new_name.isEmpty()){
        std::string current_item_name = current->text().toStdString();

        Category *cur_category = this->getCategoryByName(current_item_name);
        cur_category->setName(new_name.toStdString());
        current->setText(new_name);
        ui->selected_category->setText(new_name);
    } else {
        QMessageBox::warning(this, tr("TimeTracker"), tr("Category name cannot by empty."), QMessageBox::Ok);
    }

}

void TimeTracker::initCatagories(){

    ifstream categories_file("./categories.csv");
    string line = "";
    vector<string> all_words;
    while (getline(categories_file, line)){
        stringstream strstr(line);
        string token = "";
        while (getline(strstr,token, ',')){
            all_words.push_back(token);
        }

    }
    vector<string> prog_names;
    for(size_t i = 0, j=1; i < all_words.size(), j < all_words.size();i+=2,j+=2){
        prog_names.emplace_back(all_words[i]);

        if(!getCategory(all_words[j])){
            Category *progCategory = new Category(all_words[j]);
            categories.emplace_back(*progCategory);

        }
        Activity *act = new Activity(all_words[i],0,0,0,0,0);
        categories_acts[act->getName()] = all_words[j];
        getCategoryByName(all_words[j])->addActivity(*act);

    }


    for(size_t i = 0; i < categories.size();i++){
        QString name = QString::fromStdString(categories[i].getName());
        ui->categories_list->addItem(name);
    }


}

void TimeTracker::showActivitiesStatistics(){

    QString showForTime = ui->choose_displayed_time->text();
    QString showForPeriod = ui->display_time->currentText();

    int tSeconds = showForTime.toInt();;

    if(showForPeriod == "minutes"){

        tSeconds *= 60;
        showStatisticsItems(tSeconds);


    }
    else if(showForPeriod == "hours"){
        tSeconds *= 3600;
        showStatisticsItems(tSeconds);
    }
    else{
        tSeconds *= (3600 * 60);
        showStatisticsItems(tSeconds);
    }

}


void TimeTracker::showStatisticsItems(int showTime){


    while(ui->overview_table->rowCount() > 0){
        ui->overview_table->removeRow(0);
    }

    int i = 0;
    for(ActivePrograms::const_iterator ptr=running_acts.begin();ptr!=running_acts.end(); ptr++){



        QString activity_name = QString::fromStdString(ptr->first);
        std::string act_name = ptr->first;
        std::pair<int,bool> it = ptr->second;
        int act_time = it.first;
        QString act_category = QString::fromStdString(categories_acts[ptr->first]);

        if(act_time > showTime){

            ui->overview_table->setRowCount(i+1);
            ui->overview_table->setItem(i,0,new QTableWidgetItem(activity_name));
            ui->overview_table->setItem(i,1,new QTableWidgetItem(act_category));
            ui->overview_table->setItem(i,2,new QTableWidgetItem(act_time));
            ui->overview_table->setItem(i,3,new QTableWidgetItem("1"));
            ui->overview_table->item(i,0)->setFlags(ui->overview_table->item(i,0)->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
            ui->overview_table->item(i,1)->setFlags(ui->overview_table->item(i,1)->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
            int r = getRunningTime();
            setCellProgressBar(i,2,act_time,r);
            ui->overview_table->item(i,3)->setFlags(ui->overview_table->item(i,3)->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
            QFont serifFont("Helvetica", 12, QFont::Light);
            ui->overview_table->item(i,0)->setFont(serifFont);
            ui->overview_table->item(i,1)->setFont(serifFont);
            ui->overview_table->item(i,3)->setFont(serifFont);
            ui->overview_table->item(i,0)->setTextColor("#ffffff");
            ui->overview_table->item(i,1)->setTextColor("#ffffff");
            ui->overview_table->item(i,3)->setTextColor("#ffffff");
            i++;
        }

    }

}
void TimeTracker::setCellProgressBar(int i,int j,int value,int maxVal){

    QProgressBar *pgbar = new QProgressBar();
    QString barSheet = "QProgressBar {\
            text-align: left;\
            color: #000000;\
            padding: 1px;\
            border-bottom-right-radius: 0px;\
            border-bottom-left-radius: 0px;\
            background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,\
            stop: 0 #fff,\
            stop: 0.4999 #eee,\
            stop: 0.5 #ddd,\
            stop: 1 #eee );\
            width: 15px;\
            }\
            QProgressBar::chunk {\
            background:QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,\
            stop: 0 #78d,\
            stop: 0.4999 #46a,\
            stop: 0.5 #45a,\
            stop: 1 #238 );\
            border-radius: 2px;\
            border: 1px solid black;\
            }";
    pgbar->setStyleSheet(barSheet);
    pgbar->setMinimum(0);
    pgbar->setMaximum(maxVal);
    pgbar->setValue(value);
    QString s = secondsToTimeString(value);
    pgbar->setFormat(s);
    ui->overview_table->setCellWidget(i,j,pgbar);

 }

QString TimeTracker::secondsToTimeString(int seconds){

        time_t sec_time(seconds);
        tm *p = gmtime(&sec_time);
        QString activityTime = QString::number(p->tm_hour)+" h "+QString::number(p->tm_min)+" min";

        return activityTime;

}

void TimeTracker::initCostActivities(){
    for(std::map<string,string>::const_iterator ptr=categories_acts.begin();ptr!=categories_acts.end(); ptr++){
        QString itemName = QString::fromStdString(ptr->first);
        ui->calculatorActivities->addItem(itemName);
    }

}

void TimeTracker::initActualCategories(){
    for(std::map<string,string>::const_iterator ptr=categories_acts.begin();ptr!=categories_acts.end(); ptr++){
       if(ptr->second != ""){
           category_items[ptr->second].push_back(ptr->first);
       }
    }

}

void TimeTracker::initCostValues(){
        for(int i=0; i < 10; i++){
            int coeff = rand() % 5+1;
            this->cost_coeffs.emplace_back(coeff);
        }
}

void TimeTracker::compute_activity_cost(){

    int val = 0;
    if(ui->calculatorActivities->currentText().isEmpty()){
         QMessageBox::warning(this, tr("TimeTracker"), tr("Name of program is empty."), QMessageBox::Ok);
    }
    else{
        std::string computedActivity = ui->calculatorActivities->currentText().toStdString();

        for(std::map<string,int>::const_iterator it= activities.begin();it!=activities.end();it++){
            if(computedActivity == it->first){
                val = it->second;
            }

        }
        if(val == -1){
            QMessageBox::warning(this, tr("TimeTracker"), tr("Entered program does not exist."), QMessageBox::Ok);
        }
        else{
            if(ui->calculatorAppValue->text().isEmpty()){
                QMessageBox::warning(this, tr("TimeTracker"), tr("NO value entered."), QMessageBox::Ok);
            }
            else{
                double hourVal = ui->calculatorAppValue->text().toDouble();
                double eurVal = (val/60) * hourVal;

                QString eurString = QString::number(eurVal);
                eurString = eurString + " €";
                ui->computedValue->setText(eurString);
            }

        }
    }

}

void TimeTracker::updateChart(){

       QString currentChart = ui->graphs->currentText();
       if(currentChart == QObject::tr("activities")){
             ui->charts->setCurrentIndex(0);

            //removes previous chart from view
            QChart* chartToDelete=NULL;
            if(ui->chartView1->chart())
            {
                chartToDelete=ui->chartView1->chart();
            }

            QChart* chart= new QChart();

            QFont f;
            f.setPointSize(15);
            chart->setTitleFont(f);
            chart->setTitle(QObject::tr("Running time of activities (min)"));

            ui->chartView1->setChart(chart);

            QBarSeries *series = new QBarSeries();

            for(std::map<string,int>::const_iterator it= activities.begin();it!=activities.end();it++){
                QString name = QString::fromStdString(it->first);
                QBarSet *new_set = new QBarSet(name);
                *new_set << (activities[it->first]/60);
                series->append(new_set);

            }

            chart->addSeries(series);
            QBarCategoryAxis *axis = new QBarCategoryAxis();

            chart->createDefaultAxes();
            chart->setAxisX(axis, series);
            chart->legend()->setAlignment(Qt::AlignRight);
            delete chartToDelete;
        }
        else{
            /*******chart 2************************/
            ui->charts->setCurrentIndex(1);
            QChart* chartToDelete2=NULL;
            if(ui->chartView2->chart())
            {
                chartToDelete2=ui->chartView2->chart();
            }

            QChart* chart2= new QChart();
            QFont f2;
            f2.setPointSize(15);
            chart2->setTitleFont(f2);
            chart2->setTitle(QObject::tr("Active categories time (min)"));

           ui->chartView2->setChart(chart2);

            QBarSeries *series2 = new QBarSeries();

            for(std::map<string,std::vector<string>>::const_iterator it= category_items.begin();it!=category_items.end();it++){
                QString name = QString::fromStdString(it->first);
                QBarSet *new_set = new QBarSet(name);
                std::vector<string> categoryActs = it->second;
                int categoryTime = 0;
                for(size_t j=0; j < categoryActs.size(); j++){
                    for(std::map<string,int>::const_iterator p = activities.begin();p!=activities.end();p++){
                        if(categoryActs[j] == p->first){
                            categoryTime += activities[categoryActs[j]];
                        }
                    }

                }
                *new_set << (categoryTime/60);
                series2->append(new_set);
            }
            chart2->addSeries(series2);
            QBarCategoryAxis *axis2 = new QBarCategoryAxis();

            chart2->createDefaultAxes();
            chart2->setAxisX(axis2, series2);
            chart2->legend()->setAlignment(Qt::AlignRight);
            chart2->legend()->setVisible(true);

            delete chartToDelete2;

        }
}

int TimeTracker::timeToSeconds(std::string time_string){

    int h, m,s= 0;
    int secs = 0;

    if (sscanf(time_string.c_str(), "%d:%d:%d", &h, &m,&s) >= 2)
    {
      int secs = h *3600 + m*60 + s;

      return secs;
    }

    return secs;
}

void TimeTracker::automatic_tracking(){
    if(ui->trackBtn->isChecked()){
        ui->trackBtn->setText(QObject::tr("OFF"));
        this->autoTracking = false;
    }
    else{
        ui->trackBtn->setText(QObject::tr("ON"));
        this->autoTracking = true;
    }
}

void TimeTracker::hide_calculator(){
    if(ui->calcBtn->isChecked()){
        ui->calcBtn->setText(QObject::tr("DISABLED"));
        ui->calculateCost->hide();
    }
    else{
        ui->calcBtn->setText(QObject::tr("ENABLED"));
        ui->calculateCost->show();
    }
}

void TimeTracker::todo_notification(){

    if(ui->todo_notif_btn->isChecked()){
        ui->todo_notif_btn->setText(QObject::tr("DISABLED"));
        this->todo_notif = false;
    }
    else{
        ui->todo_notif_btn->setText(QObject::tr("ENABLED"));
        this->todo_notif = true;
    }

}

void TimeTracker::notification_sound(){

    QMediaPlayer *player = new QMediaPlayer();
    player->setMedia(QUrl::fromLocalFile("/home/juraj/Skola/ITU/projekt/ITU-TimeTracker/Notify.mp3"));
    player->setVolume(50);
    player->setPosition(0);
    player->play();

}


void TimeTracker::initShortcuts(){

    QShortcut *start_todo_list = new QShortcut(QKeySequence("Ctrl+T"),this);
    QShortcut *rename_list = new QShortcut(QKeySequence("Ctrl+M"),this);
    QShortcut *delete_list = new QShortcut(QKeySequence("Ctrl+1"),this);
    QShortcut *remove_activity = new QShortcut(QKeySequence("Ctrl+2"),this);
    QShortcut *delete_category = new QShortcut(QKeySequence("Ctrl+3"),this);
    QShortcut *delete_category_item = new QShortcut(QKeySequence("Ctrl+4"),this);

    QObject::connect(start_todo_list,SIGNAL(activated()),this,SLOT(start_todo_list_clicked()));
    QObject::connect(rename_list,SIGNAL(activated()),this,SLOT(on_rename_list()));
    QObject::connect(delete_list,SIGNAL(activated()),this,SLOT(on_delete_list_clicked()));
    QObject::connect(remove_activity,SIGNAL(activated()),this,SLOT(on_delete_activity_button_clicked()));
    QObject::connect(delete_category,SIGNAL(activated()),this,SLOT(remove_category_button_clicked()));
    QObject::connect(delete_category_item,SIGNAL(activated()),this,SLOT(remove_category_item_clicked()));

    QObject::connect(ui->printHelp,SIGNAL(triggered()),this,SLOT(showHelp()));
    QObject::connect(ui->printAbout,SIGNAL(triggered()),this,SLOT(showAboutProgram()));

}

void TimeTracker::showHelp(){
    QMessageBox::information(this, QObject::tr("Help"), QObject::tr("For manual tracking use Start/Stop tracking button.\
                                                                    For renaming use double click on the item or key shorcut,shortcuts can be found in the Settings.\
                                                                    "), QMessageBox::Ok);

}

void TimeTracker::showAboutProgram(){
    QMessageBox::information(this, QObject::tr("About TimeTracker"), QObject::tr("TimeTracker is software for tracking applications running on PC.\
                                                                                  You can see all running programs with their runtime.\
                                                                                   Application offers creating Todo lists, defining custom \
                                                                                    categories for your programs and show statistics about\
                                                                                    running activities."), QMessageBox::Ok);

}

void TimeTracker::appendCostActivities(){


    for(ActivePrograms::const_iterator ptr=running_acts.begin();ptr!=running_acts.end(); ptr++){
     int j = 0;
        for(int i=0; i < ui->calculatorActivities->count(); i++){
            if(ptr->first != ui->calculatorActivities->itemText(i).toStdString()){
                j++;
            }
        }
        if(j == ui->calculatorActivities->count()){
            ui->calculatorActivities->addItem(QString::fromStdString(ptr->first));
        }
    }

}
