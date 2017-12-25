#ifndef TIMETRACKER_H
#define TIMETRACKER_H
#include <QListWidgetItem>
#include "tracker.h"
#include "todo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QQuickView>
#include <QTableWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QWidget>
#include <QLineEdit>
#include <QTimeEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QObject>
#include <cstdlib>
#include <QtCharts>

extern bool is_stopped;
using namespace QtCharts;

namespace Ui {
class TimeTracker;
}

class TimeTracker : public QMainWindow{
    Q_OBJECT
    Tracker tracker;
    QString current_category_name;
    std::vector<TODO> todo_lists;
    std::vector<Category> categories;
    std::map<std::string,int> activities;
    std::map<std::string,std::string> categories_acts;
    std::map<std::string,std::vector<std::string>> category_items;
    std::vector<int> cost_coeffs;
    bool f = true;
    bool autoTracking = true;
    bool todo_notif = true;
    typedef std::map<std::string,std::string> CategoriesActs;
    QThread *todothread;
    typedef std::map<std::string,std::pair<int,bool>> ActivePrograms;
    ActivePrograms running_acts;
    int running_time = rand() % 2000;

public:
    explicit TimeTracker(QWidget *parent = 0);
    ~TimeTracker();


public slots:
    void onProgressChanged(std::map<std::string,std::pair<int,bool>> running_activities);
    void onRunnigListReceived(std::vector<Activity> act);
    void todoThreadDone();
    void act_done();
    void startThread();
    void tracking_button_clicked();
    void change_actual_layout(int layout_index);
    TODO *getTodoListByName(std::string name);
    void setRunningTime();
private slots:

    void showHelp();

    void showAboutProgram();

    int getRunningTime();

    void search_activated();

    QString createTimeString(int seconds);

    void start_todo_list_clicked();

    void on_add_new_list_button_clicked();

    void on_delete_list_clicked();

    void on_rename_list();

    void on_delete_activity_button_clicked();

    void removeTodoList(std::string name);

    bool getTodoList(std::string name);

    void showListItems(QTableWidget *widget,std::string name);

    void on_add_activity_button_clicked();

    void on_todo_lists_item_selected();

    void new_category_button_clicked();

    void remove_category_button_clicked();

    void new_category_item_clicked();

    void remove_category_item_clicked();

    Category *getCategoryByName(std::string name);

    bool getCategory(std::string name);

    void removeCategory(std::string name);

    void showCategoryItems(std::string current_category);

    void category_selected();

    void rename_category();

    void initCatagories();

    void showActivitiesStatistics();

    void showStatisticsItems(int showTime);

    void setCellProgressBar(int i, int j, int value, int maxVal);

    QString secondsToTimeString(int seconds);

    void initCostActivities();

    void initCostValues();

    void compute_activity_cost();

    void updateChart();

    int timeToSeconds(std::string time_string);

    void initActualCategories();

    void automatic_tracking();

    void hide_calculator();

    void todo_notification();

    void notification_sound();

    void initShortcuts();

    void appendCostActivities();

    void table_update();

public:
    Ui::TimeTracker *ui;
    QTimer *timer;
    std::map<std::string,int> number_of_runs;


signals:
    void tracking_stopped(bool is_stopped);
};

#endif // TIMETRACKER_H
