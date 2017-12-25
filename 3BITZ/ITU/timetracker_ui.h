/********************************************************************************
** Form generated from reading UI file 'timetracker.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMETRACKER_H
#define UI_TIMETRACKER_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
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
#include <QStackedWidget>
#include <QDesktopWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QProgressBar>
#include <QtCharts>
#include <QToolButton>
#include <QMenu>
#include <QObject>

QT_BEGIN_NAMESPACE
using namespace QtCharts;
class Ui_TimeTracker
{

public:
    QHBoxLayout* topLayout;
    QQuickView *view;
    QWidget* container;
    QWidget *actual_todo_list;
    QTableWidget* activities_table;
    QTableWidget *actual_list_activities;
    QStackedWidget *stackedWidget;
    QPushButton *start_tracking_button;
    QLineEdit *search_activity;
    QLineEdit *new_todo_list_name;
    QListWidget *todo_list_view;
    QListWidget *todo_list_view2;
    QPushButton *start_todo_list;
    QPushButton* add_new_list;
    QPushButton *rename_list;
    QPushButton *delete_list;
    QLineEdit *listName;
    QTableWidget *activityList;
    QLineEdit *new_activity_name;
    QTimeEdit *activity_time;
    QCheckBox *notification;
    QPushButton *add_activity;
    QPushButton *remove_activity;
    QPushButton *rename_activity;
    QLineEdit *new_category_name;
    QPushButton *add_new_category;
    QListWidget *categories_list;
    QPushButton *remove_category;
    QPushButton *rename_category;
    QLineEdit *selected_category;
    QListWidget *category_items;
    QLineEdit *new_category_item;
    QPushButton *add_category_item;
    QPushButton *remove_category_item;
    QPushButton *rename_category_item;
    QLabel *runtime;
    QSpinBox *choose_displayed_time;
    QComboBox *display_time;
    QTableWidget *overview_table;
    QTableWidget *most_used_programs;
    QPushButton *compute;
    QLineEdit *calculatorAppValue;
    QLineEdit *calculatedCost;
    QLineEdit *showComputedCost;
    QComboBox *calculatorActivities;
    QLabel *computedValue;
    QStackedWidget *charts;
    QChart *chart;
    QBarCategoryAxis *axis;
    QChartView *chartView1;
    QChart *chart2;
    QBarCategoryAxis *axis2;
    QChartView *chartView2;
    QComboBox *graphs;

    QComboBox *search_colors;
    QComboBox *table_fonts;
    QPushButton *trackBtn;
    QPushButton *todo_notif_btn;
    QComboBox *notification_options;
    QSpinBox *notif_time;
    QPushButton *calcBtn;
    QWidget *calculateCost;
    QMenuBar *menuBar;
    QMenu *help;
    QMenu *about;
    QAction *printHelp;
    QAction *printAbout;

    QWidget *main_widget;
    QVBoxLayout* main_layout;



    void setupUi(QMainWindow *TimeTracker)
    {
        menuBar = new QMenuBar();
        menuBar->setStyleSheet("QMenuBar{color: white;background-color:transparent}"
                               "QMenuBar::item:selected {\
                               background: #a8a8a8;\
                               }");
        help = new QMenu("&Help");
        printHelp = new QAction(QObject::tr("&Show Help"),TimeTracker);
        //printHelp->setShortcuts(QKeySequence::HelpContents);
        printHelp->setStatusTip(QObject::tr("Print help."));
        printHelp->setWhatsThis(QObject::tr("Click this option to create a new file."));
        printAbout = new QAction(QObject::tr("&About program"),TimeTracker);
        //printAbout->setShortcuts(QKeySequence::Print);
        printAbout->setStatusTip(QObject::tr("About program."));


        help->addAction(printHelp);
        help->addAction(printAbout);
        menuBar->addMenu(help);

        this->main_widget = new QWidget();
        this->main_widget->setMinimumWidth(1200);
        this->main_widget->setMinimumHeight(800);
        QPalette pal = QPalette();
        //"#474f5b",#767a79,#ADA6A6,#939393,#676565
        // set black background
        pal.setColor(QPalette::Background, "#424242");
        this->main_widget->setAutoFillBackground(true);
        this->main_widget->setPalette(pal);

        /*main layout with button and top layout*/
        this->main_layout =  new QVBoxLayout(this->main_widget);
        this->start_tracking_button = new QPushButton("start_tracking_button");
        this->start_tracking_button->setText(QObject::tr("Stop tracking"));
        this->start_tracking_button->setStyleSheet("QPushButton{font-size:22px;}");

        QFont font = this->start_tracking_button->font();
        font.setPointSize(15);
        this->start_tracking_button->setFont(font);
        this->start_tracking_button->setMinimumHeight(40);
        this->start_tracking_button->setStyleSheet("QPushButton {background-color: red; color:#ffffff;}");
        this->main_layout->addWidget(menuBar);
        this->main_layout->addWidget(this->start_tracking_button);

        //add layout to main layout
        this->topLayout = new QHBoxLayout();
        this->main_layout->addLayout(topLayout);
        this->view = new QQuickView(QUrl::fromLocalFile(":main1.qml"));
        view->setResizeMode(QQuickView::SizeRootObjectToView);
        view->setColor(QColor(Qt::transparent));

        this->container = QWidget::createWindowContainer(this->view,this->main_widget);
        this->container->setStyleSheet("QWidget{background-color:#424242;}");

        this->stackedWidget = new QStackedWidget;

        //menu items
        this->container->setAutoFillBackground(true);
        this->container->setPalette(pal);
        this->container->setMinimumSize(120, 500);
        this->container->setFocusPolicy(Qt::TabFocus);


        this->topLayout->addWidget(this->container,0);

        this->activities_table = new QTableWidget();
        this->activities_table->verticalHeader()->setVisible(false);
        QHeaderView* header = this->activities_table->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::Stretch);
        this->activities_table->horizontalHeader()->setStyleSheet("QHeaderView{font-size: 15pt;\
                                                                    color:#3a3a3a;\
                                                                    background-color:#d3d3d3;\
                                                                    }");



        QPalette p = this->activities_table->palette();
           p.setColor(QPalette::Base, "#3a3a3a");
           this->activities_table->setPalette(p);


        QString style1(
               "QTableWidget {"
                   "border: solid white;"
               "}"
           );
           this->activities_table->setStyleSheet(style1);

           //this->activities_table->setStyleSheet(QString("QScrollBar:horizontal { border: 2px solid grey; background: #32CC99; height: 15px; margin: 0px 20px 0 20px; }"));
        //po sortovani sa objavuju duplikovane riadky v tabulke,resp niektore tam zas nie su
        //this->activities_table->horizontalHeader()->setSortIndicatorShown(true);
        //this->activities_table->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
        //this->activities_table->setSortingEnabled(true);



        QWidget *t_widget = new QWidget();
        QVBoxLayout *tLayout = new QVBoxLayout();
        t_widget->setLayout(tLayout);
        this->search_activity = new QLineEdit();
        this->search_activity->setPlaceholderText(QObject::tr("search activity"));

        QPalette palette;
        palette.setColor(QPalette::Base,"#191919");
        palette.setColor(QPalette::Text,Qt::white);
        //this->search_activity->setPalette(palette);
        this->search_activity->setStyleSheet(" QLineEdit {\
                                               height:32px;\
                                               font-size:18px;\
                                               border:2px outset;\
                                               border-radius: 4px; \
                                               border-color: rgb(255, 255, 255);\
                                               color:rgb(0,0,0); \
                                               background-color: rgb(232, 235, 239); } "
                                              );
        QFont f = this->search_activity->font();
        font.setPointSize(15);
        this->search_activity->setFont(f);



        //layout with main table and search box
        tLayout->addWidget(this->search_activity);
        tLayout->addWidget(this->activities_table);

        //layout with todo lists
        QString style = "QPushButton{color: #ffffff;border: 1px solid #d6d6d6;\
                        font-size:17px;\
                        border-radius: 4px;\
                        padding: 6px;\
                        background: #303030;\
                        min-width: 80px;}";

        QPalette p1;
        //#f6f6f6
        p1.setColor(QPalette::Base, "#e9e9e9");



        QString listSheet = "QListWidget::item {\
            color:#000000;\
            background-color:transparent;\
        }\
        QListWidget::item:selected{\
            background: #d3d3d3;\
            };";

        QString tableSheet = "QTableWidget{\
                             selection-background-color:#6f58ed;\
                             selection-color:5f5f5f;}\
                             QTableWidget::item {\
                selection-background-color:#6f58ed;\
                color:#000000;\
                background-color:transparent;\
        }";



        QWidget *todo_main_tab = new QWidget();
        QVBoxLayout *todo_main_tab_layout = new QVBoxLayout();
        todo_main_tab->setLayout(todo_main_tab_layout);

        actual_todo_list = new QWidget();
        QVBoxLayout *actual_list_layout = new QVBoxLayout();
        actual_todo_list->setLayout(actual_list_layout);
        QLabel *actual_list_name = new QLabel(QObject::tr("Running TODO list activities"));
        actual_list_name->setFont(font);
        actual_list_name->setStyleSheet("QLabel{color: #ffffff}");

        this->actual_list_activities = new QTableWidget();
        this->actual_list_activities->verticalHeader()->setVisible(false);
        QHeaderView* header_act = this->actual_list_activities->horizontalHeader();
        header_act->setSectionResizeMode(QHeaderView::Stretch);
        this->actual_list_activities->setColumnCount(2);
        this->actual_list_activities->setRowCount(0);
        this->actual_list_activities->setPalette(p1);
        this->actual_list_activities->setStyleSheet(tableSheet);
        this->actual_list_activities->horizontalHeader()->setStyleSheet("QHeaderView{font-size: 13pt;\
                                                                      color:#000000;\
                                                                      background-color:#d3d3d3;\
                                                                      }");

        QTableWidgetItem* col_1 = new QTableWidgetItem(QString("Activity"),QTableWidgetItem::Type);
        this->actual_list_activities->setHorizontalHeaderItem(0,col_1);
        QTableWidgetItem* col_2 = new QTableWidgetItem(QString("Remaining time"),QTableWidgetItem::Type);
        this->actual_list_activities->setHorizontalHeaderItem(1,col_2);
        this->actual_list_activities->verticalHeader()->setVisible(false);
        actual_todo_list->hide();


        QPalette todo_palette;
        todo_palette.setColor(QPalette::Base,"#191919");
        todo_palette.setColor(QPalette::Text,Qt::white);
        QString todo_line_edits = " QLineEdit { \
                                   border:2px outset; \
                                   border-radius: 4px; \
                                   border-color: rgb(255, 255, 255); \
                                   color:rgb(0,0,0); \
                                   background-color: rgb(232, 235, 239); } ";


        //actual_todo_list->setMaximumHeight(250);

        actual_list_layout->addWidget(actual_list_name);
        actual_list_layout->addWidget(this->actual_list_activities);

        QWidget *todo_lists_tab = new QWidget();
        QHBoxLayout *todo_list_layout = new QHBoxLayout();
        todo_lists_tab->setLayout(todo_list_layout);
        todo_main_tab_layout->addWidget(todo_lists_tab);

        QVBoxLayout *todo1_tab = new QVBoxLayout();
        QVBoxLayout *todo2_tab = new QVBoxLayout();
        todo_list_layout->addLayout(todo1_tab);
        todo_list_layout->addLayout(todo2_tab);
        QLabel *new_todo_list = new QLabel(QObject::tr("New TODO list"));
        QFont todo_font = new_todo_list->font();
        todo_font.setPointSize(14);
        new_todo_list->setFont(todo_font);
        new_todo_list->setStyleSheet("QLabel{color: #ffffff}");

        this->todo_list_view = new QListWidget();
        this->todo_list_view->setPalette(p1);
        this->todo_list_view->setStyleSheet(listSheet);
        this->todo_list_view->setFont(todo_font);


        this->new_todo_list_name = new QLineEdit();
        this->new_todo_list_name->setFont(todo_font);
        this->new_todo_list_name->setPlaceholderText(QObject::tr("list name"));
        this->new_todo_list_name->setPalette(p1);
        this->new_todo_list_name->setStyleSheet(todo_line_edits);



        this->add_new_list = new QPushButton(QObject::tr("Add list"));
        this->add_new_list->setStyleSheet(style);
        this->add_new_list->setToolTip("Click to add new list.");

        QLabel *todo_list_showed = new QLabel(QObject::tr("TODO lists"));
        todo_list_showed->setFont(todo_font);
        todo_list_showed->setStyleSheet("QLabel{color: #ffffff}");

        QWidget *todo_lists_handle_buttons = new QWidget();
        QHBoxLayout *buttons_layout = new QHBoxLayout();
        todo_lists_handle_buttons->setLayout(buttons_layout);
        this->delete_list = new QPushButton(QObject::tr("Delete list"));
        this->delete_list->setStyleSheet(style);
        this->delete_list->setToolTip("Click to delete chosen list from the view.");
        this->rename_list = new QPushButton(QObject::tr("Rename list"));
        this->rename_list->setStyleSheet(style);
        buttons_layout->addWidget(this->delete_list);
        buttons_layout->addWidget(this->rename_list);
        this->start_todo_list = new QPushButton(QObject::tr("Start tracking list"));
        this->start_todo_list->setToolTip((QObject::tr("Click to start tracking chosen list from Todo lists.")));
        this->start_todo_list->setStyleSheet("QPushButton{color: #ffffff;border: 1px solid #d6d6d6;\
                                             font-size:20px;\
                                             border-radius: 4px;\
                                             padding: 6px;\
                                             background: #303030;\
                                             min-width: 80px;}");



        todo1_tab->addWidget(new_todo_list);
        todo1_tab->addWidget(this->new_todo_list_name);
        todo1_tab->addWidget(this->add_new_list);
        todo1_tab->addWidget(todo_list_showed);
        todo1_tab->addWidget(this->todo_list_view);
        todo1_tab->addWidget(this->start_todo_list);
        todo1_tab->addWidget(todo_lists_handle_buttons);


        QLabel *active_list = new QLabel(QObject::tr("Current list"));
        active_list->setFont(todo_font);
        active_list->setStyleSheet("QLabel{color: #ffffff}");


        this->listName = new QLineEdit();
        this->listName->setReadOnly(true);
        this->listName->setPalette(p1);
        this->listName->setFont(todo_font);
        this->listName->setPlaceholderText(QObject::tr("list name"));
        this->listName->setStyleSheet(todo_line_edits);
        QLabel *act = new QLabel(QObject::tr("List activities"));
        act->setFont(todo_font);
        act->setStyleSheet("QLabel{color: #ffffff}");

        this->activityList = new QTableWidget();
        this->activityList->setPalette(p1);
        this->activityList->setStyleSheet(tableSheet);
        this->activityList->setColumnCount(2);
        this->activityList->setRowCount(0);
        this->activityList->horizontalHeader()->setStyleSheet("QHeaderView{font-size: 13pt;\
                                                              color:#000000;\
                                                              background-color:#d3d3d3;\
                                                              }");
        //#3a3a3a

        QTableWidgetItem* col1 = new QTableWidgetItem(QString(QObject::tr("Activity")),QTableWidgetItem::Type);
        this->activityList->setHorizontalHeaderItem(0,col1);
        QTableWidgetItem* col2 = new QTableWidgetItem(QString(QObject::tr("Activity duration")),QTableWidgetItem::Type);
        this->activityList->setHorizontalHeaderItem(1,col2);
        this->activityList->verticalHeader()->setVisible(false);
        QHeaderView* header1 = this->activityList->horizontalHeader();
        header1->setSectionResizeMode(QHeaderView::Stretch);
        this->activityList->setStyleSheet("QTableWidgetItem{font-size:15px;}");


        QLabel *new_activity = new QLabel(QObject::tr("New activity"));
        new_activity->setFont(todo_font);
        new_activity->setStyleSheet("QLabel{color: #ffffff}");

        this->new_activity_name = new QLineEdit();
        this->new_activity_name->setPlaceholderText(QObject::tr("activity name"));
        this->new_activity_name->setFont(todo_font);
        this->new_activity_name->setPalette(todo_palette);
        this->new_activity_name->setStyleSheet(todo_line_edits);

        QLabel *act_time = new QLabel(QObject::tr("Duration (hh:mm)"));
        act_time->setFont(todo_font);
        act_time->setStyleSheet("QLabel{color: #ffffff}");

        QString todo_t_edits = " QTimeEdit {\
                                   border-color: rgb(255, 255, 255); \
                                   color:rgb(0,0,0); \
                                   background-color: rgb(232, 235, 239); } ";



        this->activity_time = new QTimeEdit();
        this->activity_time->setFont(todo_font);
        this->activity_time->setStyleSheet(todo_t_edits);
        this->new_activity_name->setStyleSheet(todo_line_edits);
        this->notification = new QCheckBox(QObject::tr("Notify when time exceeds"));
        this->notification->setCheckable(true);
        this->notification->setStyleSheet("QCheckBox{color: #ffffff}");


        QWidget *activities_menu = new QWidget();
        QHBoxLayout *activities_menu_layout = new QHBoxLayout();
        activities_menu->setLayout(activities_menu_layout);
        this->add_activity = new QPushButton(QObject::tr("Add activity"));
        this->add_activity->setToolTip(QObject::tr("Click to add new activity to actual list."));
        this->add_activity->setStyleSheet(style);
        this->remove_activity = new QPushButton(QObject::tr("Remove activity"));
        this->remove_activity->setToolTip(QObject::tr("Click to remove chosen activity from the table."));
        this->remove_activity->setStyleSheet(style);
        this->rename_activity = new QPushButton(QObject::tr("Rename activity"));
        this->rename_activity->setStyleSheet(style);
        activities_menu_layout->addWidget(this->add_activity);
        activities_menu_layout->addWidget(this->remove_activity);
        //activities_menu_layout->addWidget(this->rename_activity);




        todo2_tab->addWidget(active_list);
        todo2_tab->addWidget(this->listName);
        todo2_tab->addWidget(act);
        todo2_tab->addWidget(this->activityList);
        todo2_tab->addWidget(new_activity);
        todo2_tab->addWidget(this->new_activity_name);
        todo2_tab->addWidget(act_time);
        todo2_tab->addWidget(this->activity_time);
        //todo2_tab->addWidget(this->notification);
        todo2_tab->addWidget(activities_menu);

        todo_main_tab_layout->addWidget(todo_lists_tab);
        todo_main_tab_layout->addWidget(actual_todo_list);



        QWidget *categories_tab = new QWidget();
        QHBoxLayout *categories_tab_layout = new QHBoxLayout();
        categories_tab->setLayout(categories_tab_layout);
        QVBoxLayout *category_tab1 = new QVBoxLayout();
        QVBoxLayout *category_tab2 = new QVBoxLayout();
        categories_tab_layout->addLayout(category_tab1);
        categories_tab_layout->addLayout(category_tab2);

        QLabel *new_category = new QLabel();
        new_category->setText(QObject::tr("New category"));
        new_category->setFont(todo_font);
        new_category->setStyleSheet("QLabel{color: #ffffff}");

        this->categories_list = new QListWidget();
        this->categories_list->setPalette(p1);
        this->categories_list->setStyleSheet(listSheet);
        this->categories_list->setFont(todo_font);
        this->new_category_name = new QLineEdit();

        this->new_category_name->setPlaceholderText(QObject::tr("category name"));
        this->new_category_name->setPalette(p1);
        this->new_category_name->setStyleSheet(todo_line_edits);
        this->new_category_name->setFont(todo_font);
        this->add_new_category = new QPushButton(QObject::tr("Add category"));
        this->add_new_category->setStyleSheet(style);
        this->add_new_category->setToolTip(QObject::tr("Click to add new category."));

        QLabel *categories_showed = new QLabel(QObject::tr("Category list"));
        categories_showed->setFont(todo_font);
        categories_showed->setStyleSheet("QLabel{color: #ffffff}");

        QWidget *categories_handle_buttons = new QWidget();
        QHBoxLayout *category_buttons_layout = new QHBoxLayout();
        categories_handle_buttons->setLayout(category_buttons_layout);
        this->remove_category = new QPushButton(QObject::tr("Delete category"));
        this->remove_category->setToolTip(QObject::tr("Click to remove selected category."));
        this->remove_category->setStyleSheet(style);
        this->rename_category = new QPushButton(QObject::tr("Rename category"));
        this->rename_category->setStyleSheet(style);
        category_buttons_layout->addWidget(this->remove_category);
        category_buttons_layout->addWidget(this->rename_category);

        category_tab1->addWidget(new_category);
        category_tab1->addWidget(this->new_category_name);
        category_tab1->addWidget(this->add_new_category);
        category_tab1->addWidget(categories_showed);
        category_tab1->addWidget(this->categories_list);
        category_tab1->addWidget(categories_handle_buttons);


        QLabel *current_category = new QLabel(QObject::tr("Current category"));
        current_category->setFont(todo_font);
        current_category->setStyleSheet("QLabel{color: #ffffff}");

        this->selected_category = new QLineEdit();
        this->selected_category->setReadOnly(true);
        this->selected_category->setPalette(p1);
        this->selected_category->setPlaceholderText(QObject::tr("category name"));
        this->selected_category->setStyleSheet(todo_line_edits);
        this->selected_category->setFont(todo_font);
        QLabel *cat_items = new QLabel(QObject::tr("Category items"));
        cat_items->setFont(todo_font);
        cat_items->setStyleSheet("QLabel{color: #ffffff}");
        this->category_items = new QListWidget();
        this->category_items->setPalette(p1);
        this->category_items->setStyleSheet(listSheet);
        QFont listFont;
        listFont.setPointSize(13);
        this->category_items->setFont(listFont);

        QLabel *new_item = new QLabel(QObject::tr("New category item"));
        new_item->setFont(todo_font);
        new_item->setStyleSheet("QLabel{color: #ffffff}");

        this->new_category_item = new QLineEdit();
        this->new_category_item->setPlaceholderText(QObject::tr("item name"));
        this->new_category_item->setPalette(p1);
        this->new_category_item->setStyleSheet(todo_line_edits);
        this->new_category_item->setFont(todo_font);
        QWidget *categories_items_menu = new QWidget();
        QHBoxLayout *cat_menu_layout = new QHBoxLayout();
        categories_items_menu->setLayout(cat_menu_layout);
        this->add_category_item = new QPushButton(QObject::tr("Add item"));
        this->add_category_item->setToolTip(QObject::tr("Click to add new item to selected category."));
        this->add_category_item->setStyleSheet(style);
        this->remove_category_item = new QPushButton(QObject::tr("Remove item"));
        this->remove_category_item->setStyleSheet(style);
        this->remove_category_item->setToolTip(QObject::tr("Click to remove item from selected category."));
        this->rename_category_item = new QPushButton(QObject::tr("Rename activity"));
        this->rename_category_item->setStyleSheet(style);
        cat_menu_layout->addWidget(this->add_category_item);
        cat_menu_layout->addWidget(this->remove_category_item);
        //cat_menu_layout->addWidget(this->rename_category_item);

        category_tab2->addWidget(current_category);
        category_tab2->addWidget(this->selected_category);
        category_tab2->addWidget(new_item);
        category_tab2->addWidget(this->new_category_item);
        category_tab2->addWidget(cat_items);
        category_tab2->addWidget(this->category_items);
        category_tab2->addWidget(categories_items_menu);

        QString statSheet = "QLabel{\
                            color: #fe8c00;\
                            }";

        QString spin_edits = " QSpinBox {\
                                   border-color: rgb(255, 255, 255); \
                                   color:rgb(0,0,0); \
                                   background-color: rgb(232, 235, 239); } ";





        QFont statFont;
        statFont.setPointSize(20);
        /*statistics tab*/
        QWidget *statistics_tab = new QWidget();
        QVBoxLayout *statistics_layout = new QVBoxLayout();
        statistics_tab->setLayout(statistics_layout);
        QWidget *top_panel = new QWidget();
        QHBoxLayout *top_panel_layout = new QHBoxLayout();
        top_panel->setLayout(top_panel_layout);
        QLabel *total_runtime = new QLabel(QObject::tr("Total runtime:"));
        total_runtime->setFont(statFont);
        total_runtime->setStyleSheet(statSheet);
        this->runtime = new QLabel();
        this->runtime->setFont(statFont);
        this->runtime->setStyleSheet(statSheet);
        QLabel *choose_time = new QLabel(QObject::tr("Show for "));
        choose_time->setStyleSheet("QLabel{color:#ffffff;\
                                    qproperty-alignment: AlignCenter;}"
                                   );
        QFont timeFont;
        timeFont.setPointSize(15);
        choose_time->setFont(timeFont);
        this->choose_displayed_time = new QSpinBox();
        this->choose_displayed_time->setStyleSheet(spin_edits);

        this->display_time = new QComboBox();
        this->display_time->addItems({ QObject::tr("minutes"), QObject::tr("hours"), QObject::tr("days")});
        this->overview_table = new QTableWidget();
        this->overview_table->setPalette(p1);
        this->overview_table->setStyleSheet(tableSheet);
        this->overview_table->verticalHeader()->setVisible(false);
        this->overview_table->setColumnCount(4);
        this->overview_table->setRowCount(1);



        QTableWidgetItem* column0 = new QTableWidgetItem(QString(QObject::tr("Activity")),QTableWidgetItem::Type);
        this->overview_table->setHorizontalHeaderItem(0,column0);
        QTableWidgetItem* column1 = new QTableWidgetItem(QString(QObject::tr("Category")),QTableWidgetItem::Type);
        this->overview_table->setHorizontalHeaderItem(1,column1);
        QTableWidgetItem* column2 = new QTableWidgetItem(QString(QObject::tr("Runtime")),QTableWidgetItem::Type);
        this->overview_table->setHorizontalHeaderItem(2,column2);
        QTableWidgetItem* column3 = new QTableWidgetItem(QString(QObject::tr("Number of runs")),QTableWidgetItem::Type);
        this->overview_table->setHorizontalHeaderItem(3,column3);
        QHeaderView* overview_header = this->overview_table->horizontalHeader();
        overview_header->setSectionResizeMode(QHeaderView::Stretch);
        this->overview_table->horizontalHeader()->setStyleSheet("QHeaderView{font-size: 17px;\
                                                                            color:#3a3a3a;\
                                                                            background-color:#d3d3d3;\
                                                                            }");

        top_panel_layout->addWidget(total_runtime);
        top_panel_layout->addWidget(this->runtime);
        top_panel_layout->addWidget(choose_time);
        top_panel_layout->addWidget(this->choose_displayed_time);
        top_panel_layout->addWidget(this->display_time);

        QWidget *bottom_panel = new QWidget();
        QHBoxLayout *bottom_panel_layout = new QHBoxLayout();
        bottom_panel->setLayout(bottom_panel_layout);


        QPalette p2 ;
        //#f6f6f6,#e9e9e9
        p2.setColor(QPalette::Background, "#e9e9e9");


        //#424242,
        this->calculateCost = new QWidget();
        this->calculateCost->setStyleSheet("QWidget{\
                                     border-radius:4px;\
                                     background-color: #e9e9e9;\
                                     }");

        QString calculator_line_edits = " QLineEdit { \
                                   border:2px outset; \
                                   border-radius: 4px; \
                                   border-color: rgb(255, 255, 255); \
                                   color:rgb(0,0,0); \
                                   background-color: #bdbdbd; } ";

       QString calcStyle = "QPushButton{color: #ffffff;border: 1px solid #d6d6d6;\
                                    border-radius: 4px;\
                                    padding: 6px;\
                                    background: #7e7e7e;\
                                    min-width: 80px;}";




        calculateCost->setMaximumWidth(400);
        calculateCost->setMinimumWidth(350);





        QVBoxLayout *calculatorLayout = new QVBoxLayout();
        calculateCost->setLayout(calculatorLayout);

        QLabel *calculateLabel = new QLabel(QObject::tr("Cost calculator"));
        calculateLabel->setStyleSheet("QLabel{color: #000000;\
                                              qproperty-alignment: AlignCenter;\
                                              font:20px;\
                                              }");
        //nastavit vlastnosti

        //lineedit s tlacitkom calculate
        QWidget *calc = new QWidget();
        QVBoxLayout *mainCalcLayout = new QVBoxLayout();
        calc->setLayout(mainCalcLayout);
        mainCalcLayout->setMargin(0);
        mainCalcLayout->setSpacing(0);

        QWidget *labels = new QWidget();
        QHBoxLayout *labLay = new QHBoxLayout();
        labels->setLayout(labLay);
        labLay->setMargin(0);
        labLay->setSpacing(0);

        QLabel *calcName = new QLabel(QObject::tr("Program name"));
        calcName->setStyleSheet("QLabel{color: #000000; font:18px;}");
        QLabel *calcValue = new QLabel(QObject::tr("   Usage value eur/min"));
        calcValue->setStyleSheet("QLabel{color: #000000; font:18px;}");

        labLay->addWidget(calcName);
        labLay->addWidget(calcValue);

        QWidget *calculatorItems = new QWidget();
        QHBoxLayout *calcLayout = new QHBoxLayout();
        calculatorItems->setLayout(calcLayout);


        this->compute = new QPushButton(QObject::tr("Calculate"));
        compute->setStyleSheet(calcStyle);

        this->calculatorAppValue = new QLineEdit();
        this->calculatorAppValue->setMaximumWidth(180);
        this->calculatorAppValue->setPlaceholderText("Value eur/min");
        this->calculatorAppValue->setPalette(p1);
        this->calculatorAppValue->setStyleSheet(calculator_line_edits);
        this->calculatorAppValue->setFont(todo_font);

        this->calculatorActivities = new QComboBox();
        QFont fnt;
        fnt.setPointSize(20);
        this->calculatorActivities->setFont(font);

        this->calculatorActivities->setView(new QListView());
        this->calculatorActivities->setStyleSheet("QComboBox{background-color:#7e7e7e;min-height:30px;color:white}\
                                                  QComboBox QAbstractItemView { outline: 0px;}\
                                                  QComboBox QAbstractItemView::item {min-height: 35px; min-width: 50px; }\
                                                  QListView::item:selected { color: black; background-color: lightgray}");



        calcLayout->addWidget(this->calculatorActivities);
        calcLayout->addWidget(this->calculatorAppValue);

        mainCalcLayout->addWidget(labels);
        mainCalcLayout->addWidget(calculatorItems);
        mainCalcLayout->addWidget(compute);

        QLabel *computedCost = new QLabel(QObject::tr("Computed value"));
        computedCost->setStyleSheet("QLabel{color: #000000; font:18px;}");
        this->showComputedCost = new QLineEdit();
        this->showComputedCost->setPalette(p1);
        this->showComputedCost->setStyleSheet(calculator_line_edits);
        this->showComputedCost->setFont(todo_font);

       this->computedValue = new QLabel("0 €");
       this->computedValue->setStyleSheet("QLabel{color: #000000;\
                                     font:40px;\
                                     }");
        calculatorLayout->addWidget(calculateLabel);
        calculatorLayout->addWidget(calc);
        calculatorLayout->addWidget(computedCost);
        calculatorLayout->addWidget(computedValue);
        //calculatorLayout->addWidget(showComputedCost);

        this->charts = new QStackedWidget();

        /*first chart*/
        this->chart = new QChart();
        this->chart->setTitle(QObject::tr("Running activities"));
        this->chart->setAnimationOptions(QChart::SeriesAnimations);

        this->axis = new QBarCategoryAxis();
        this->chart->createDefaultAxes();

        chart->legend()->setVisible(true);
        this->chartView1 = new QChartView(this->chart);
        this->chartView1->setRenderHint(QPainter::Antialiasing);

        /*second chart*/
        this->chart2 = new QChart();
        this->chart2->setTitle(QObject::tr("Running categories"));
        this->chart2->setAnimationOptions(QChart::SeriesAnimations);

        this->axis2 = new QBarCategoryAxis();
        this->chart2->createDefaultAxes();

        chart2->legend()->setVisible(true);
        this->chartView2 = new QChartView(this->chart2);
        this->chartView2->setRenderHint(QPainter::Antialiasing);

        QWidget *chartWidget = new QWidget();
        QVBoxLayout *chartWidgetLayout = new QVBoxLayout();
        chartWidget->setLayout(chartWidgetLayout);

        QWidget *topGraphs = new QWidget();
        QHBoxLayout *gridLayout = new QHBoxLayout();
        topGraphs->setLayout(gridLayout);
        gridLayout->setMargin(0);
        gridLayout->setSpacing(0);
        QLabel *chLabel = new QLabel(QObject::tr("Show chart for"));
        chLabel->setStyleSheet("QLabel{color: #ffffff; font:18px;}");
        chLabel->setMaximumWidth(150);
        this->graphs = new QComboBox();
        this->graphs->addItems({QObject::tr("activities"),QObject::tr("categories")});

        gridLayout->addWidget(chLabel);
        gridLayout->addWidget(this->graphs);
        chartWidgetLayout->addWidget(topGraphs);
        chartWidgetLayout->addWidget(this->charts);

        this->charts->addWidget(chartView1);
        this->charts->addWidget(chartView2);

        bottom_panel_layout->addWidget(chartWidget);
        bottom_panel_layout->addWidget(calculateCost);

        statistics_layout->addWidget(top_panel);
        statistics_layout->addWidget(this->overview_table);
        statistics_layout->addWidget(bottom_panel);

        /*Settings tab*/
        QString labelStyle = "QLabel{color: #ffffff; font:18px;}";
        QString headerStyle = "QLabel{color: #fe8c00; font:22px;}";

        QWidget *settings_tab = new QWidget();

        QHBoxLayout *settingsLayout = new QHBoxLayout();
        settings_tab->setLayout(settingsLayout);

        QWidget *settings_panel1 = new QWidget();
        QVBoxLayout *settings_panel_layout = new QVBoxLayout();
        settings_panel1->setLayout(settings_panel_layout);
        settings_panel_layout->setMargin(0);
        settings_panel_layout->setSpacing(0);

        //QWidget *settings_panel2 = new QWidget();
        //QVBoxLayout *settings_panel_layout2 = new QVBoxLayout();
        //settings_panel2->setLayout(settings_panel_layout2);
        QLabel *program_settings = new QLabel("Program setttings");
        program_settings->setStyleSheet(headerStyle);

        QWidget *lang_picker = new QWidget();
        QHBoxLayout *hbLayout = new QHBoxLayout();
        lang_picker->setLayout(hbLayout);
        QLabel *acolor = new QLabel(QObject::tr("Language:"));
        acolor->setStyleSheet(labelStyle);
        QComboBox * langs = new QComboBox();
        langs->addItems({QObject::tr("english"),QObject::tr("slovak"),QObject::tr("czech")});
        hbLayout->addWidget(acolor);
        hbLayout->addWidget(langs);

        QLabel *main_tab_settings = new QLabel("Main tab setttings");
        main_tab_settings->setStyleSheet(headerStyle);

        QWidget *color_picker = new QWidget();
        QHBoxLayout *cpLayout = new QHBoxLayout();
        color_picker->setLayout(cpLayout);
        QLabel *color = new QLabel(QObject::tr("Set search color:"));
        color->setStyleSheet(labelStyle);
        this->search_colors= new QComboBox();
        this->search_colors->addItems({QObject::tr("orange"),QObject::tr("blue"),QObject::tr("green"),QObject::tr("yellow"),});
        cpLayout->addWidget(color);
        cpLayout->addWidget(this->search_colors);

        QWidget *font_picker = new QWidget();
        QHBoxLayout *fontLayout = new QHBoxLayout();
        font_picker->setLayout(fontLayout);
        QLabel *table_font = new QLabel(QObject::tr("Set main table font:"));
        table_font->setStyleSheet(labelStyle);
        this->table_fonts= new QComboBox();
        this->table_fonts->addItems({"13","15","18"});
        fontLayout->addWidget(table_font);
        fontLayout->addWidget(this->table_fonts);

        QWidget *auto_tracking = new QWidget();
        QHBoxLayout *trackingLayout = new QHBoxLayout();
        auto_tracking->setLayout(trackingLayout);
        QLabel *tracking = new QLabel(QObject::tr("Automatic tracking:"));
        tracking->setStyleSheet(labelStyle);
        this->trackBtn = new QPushButton(QObject::tr("ON"));
        this->trackBtn->setCheckable(true);
        this->trackBtn->setStyleSheet("QPushButton{background-color: #329932; color:#ffffff;}\
                               QPushButton:checked { background-color:#ff4c4c;color:#ffffff;\
                                }");
        trackingLayout->addWidget(tracking);
        trackingLayout->addWidget(this->trackBtn);


        QLabel *todo_tab_settings = new QLabel(QObject::tr("TODO lists setttings"));
        todo_tab_settings->setStyleSheet(headerStyle);

        QWidget *todo_notif = new QWidget();
        QHBoxLayout *l3 = new QHBoxLayout();
        todo_notif->setLayout(l3);
        QLabel *notif_label = new QLabel(QObject::tr("Notification:"));
        notif_label->setStyleSheet(labelStyle);
        this->todo_notif_btn = new QPushButton(QObject::tr("ENABLED"));
        todo_notif_btn->setCheckable(true);
        todo_notif_btn->setStyleSheet("QPushButton{background-color:#329932; color:#ffffff}\
                                QPushButton:checked { background-color:#ff4c4c;color:#ffffff;\
                                }");
        l3->addWidget(notif_label);
        l3->addWidget(todo_notif_btn);

        QWidget *notification_picker = new QWidget();
        QHBoxLayout *notifLayout = new QHBoxLayout();
        notification_picker->setLayout(notifLayout);
        QLabel *notify = new QLabel(QObject::tr("Choose notification option:"));
        notify->setStyleSheet(labelStyle);
        this->notification_options= new QComboBox();
        notification_options->addItems({QObject::tr("notification"),QObject::tr("sound"),QObject::tr("notification and sound")});
        notifLayout->addWidget(notify);
        notifLayout->addWidget(notification_options);

        QWidget *notif_interval = new QWidget();
        QHBoxLayout *nLayout = new QHBoxLayout();
        notif_interval->setLayout(nLayout);
        QLabel *interval = new QLabel(QObject::tr("Set notification interval (min):"));
        interval->setStyleSheet(labelStyle);
        this->notif_time = new QSpinBox();
        notif_time->setStyleSheet(spin_edits);
        nLayout->addWidget(interval);
        nLayout->addWidget(notif_time);

        QLabel *stats = new QLabel(QObject::tr("Statistics settings"));
        stats->setStyleSheet(headerStyle);

        QWidget *showCost = new QWidget();
        QHBoxLayout *costLayout = new QHBoxLayout();
        showCost->setLayout(costLayout);
        QLabel *calculator = new QLabel(QObject::tr("Displaying cost calculator:"));
        calculator->setStyleSheet(labelStyle);
        this->calcBtn = new QPushButton(QObject::tr("ENABLED"));
        calcBtn->setCheckable(true);
        calcBtn->setStyleSheet("QPushButton{background-color:#329932; color:#ffffff}\
                                QPushButton:checked { background-color:#ff4c4c;color:#ffffff;\
                                }");
        costLayout->addWidget(calculator);
        costLayout->addWidget(calcBtn);

        settings_panel_layout->addWidget(program_settings);
        settings_panel_layout->addWidget(lang_picker);
        settings_panel_layout->addWidget(main_tab_settings);
        settings_panel_layout->addWidget(color_picker);
        settings_panel_layout->addWidget(font_picker);
        settings_panel_layout->addWidget(auto_tracking);
        settings_panel_layout->addWidget(todo_tab_settings);
        settings_panel_layout->addWidget(todo_notif);
        settings_panel_layout->addWidget(notification_picker);
        settings_panel_layout->addWidget(notif_interval);
        settings_panel_layout->addWidget(stats);
        settings_panel_layout->addWidget(showCost);

        QString label2Style = "QLabel{color: #ffffff; font:20px;}";
        QWidget *settings_panel2 = new QWidget();
        //QGridLayout *set2Layout = new QGridLayout();
        //settings_panel2->setLayout(set2Layout);
        QVBoxLayout *set2Layout = new QVBoxLayout();
        settings_panel2->setLayout(set2Layout);

        QWidget *w1 = new QWidget();
        QHBoxLayout *w1l = new QHBoxLayout();
        w1->setLayout(w1l);
        QLabel *shortcuts = new QLabel(QObject::tr("TimeTracker keyboard shortcuts"));
        shortcuts->setStyleSheet(headerStyle);
        QLabel *start_list = new QLabel(QObject::tr("Start TODO list"));
        start_list->setStyleSheet(label2Style);
        QLabel *start_list_shortcut = new QLabel("Ctrl+T");
        start_list_shortcut->setStyleSheet(label2Style);
        w1l->addWidget(start_list);
        w1l->addWidget(start_list_shortcut);

        QWidget *w2 = new QWidget();
        QHBoxLayout *w2l = new QHBoxLayout();
        w2->setLayout(w2l);
        QLabel *rename_list = new QLabel(QObject::tr("Rename TODO list"));
        rename_list->setStyleSheet(label2Style);
        QLabel *rename_list_shortcut = new QLabel("Ctrl+M");
        rename_list_shortcut->setStyleSheet(label2Style);
        w2l->addWidget(rename_list);
        w2l->addWidget(rename_list_shortcut);


        QWidget *w3 = new QWidget();
        QHBoxLayout *w3l = new QHBoxLayout();
        w3->setLayout(w3l);
        QLabel *delete_list =  new QLabel(QObject::tr("Delete TODO list"));
        delete_list->setStyleSheet(label2Style);
        QLabel *delete_list_shortcut = new QLabel("Ctrl+1");
        delete_list_shortcut->setStyleSheet(label2Style);
        w3l->addWidget(delete_list);
        w3l->addWidget(delete_list_shortcut);

        QWidget *w4 = new QWidget();
        QHBoxLayout *w4l = new QHBoxLayout();
        w4->setLayout(w4l);
        QLabel *remove_list_activity = new QLabel(QObject::tr("Delete TODO list activity"));
        remove_list_activity->setStyleSheet(label2Style);
        QLabel *remove_list_activity_shortcut = new QLabel("Ctrl+2");
        remove_list_activity_shortcut->setStyleSheet(label2Style);
        w4l->addWidget(remove_list_activity);
        w4l->addWidget(remove_list_activity_shortcut);


        QWidget *w5 = new QWidget();
        QHBoxLayout *w5l = new QHBoxLayout();
        w5->setLayout(w5l);
        QLabel *delete_category = new QLabel(QObject::tr("Delete category"));
        delete_category->setStyleSheet(label2Style);
        QLabel *delete_category_shortcut = new QLabel("Ctrl+3");
        delete_category_shortcut->setStyleSheet(label2Style);
        w5l->addWidget(delete_category);
        w5l->addWidget(delete_category_shortcut);

        QWidget *w6 = new QWidget();
        QHBoxLayout *w6l = new QHBoxLayout();
        w6->setLayout(w6l);
        QLabel *delete_category_item = new QLabel(QObject::tr("Delete category activity"));
        delete_category_item->setStyleSheet(label2Style);
        QLabel *delete_category_item_shortcut = new QLabel("Ctrl+4");
        delete_category_item_shortcut->setStyleSheet(label2Style);
        w6l->addWidget(delete_category_item);
        w6l->addWidget(delete_category_item_shortcut);

        QSpacerItem *verticalSpacer = new QSpacerItem(40,100);
        QSpacerItem *horizontalSpacer = new QSpacerItem(20,0);
        set2Layout->addWidget(shortcuts);
        set2Layout->addWidget(w1);
        set2Layout->addWidget(w2);
        set2Layout->addWidget(w3);
        set2Layout->addWidget(w4);
        set2Layout->addWidget(w5);
        set2Layout->addWidget(w6);
        set2Layout->addItem(verticalSpacer);


        settingsLayout->addWidget(settings_panel1);
        settingsLayout->addItem(horizontalSpacer);
        settingsLayout->addWidget(settings_panel2);


        this->stackedWidget->addWidget(t_widget);
        this->stackedWidget->addWidget(todo_main_tab);
        this->stackedWidget->addWidget(categories_tab);
        this->stackedWidget->addWidget(statistics_tab);
        this->stackedWidget->addWidget(settings_tab);


        this->activities_table->setColumnCount(3);
        this->activities_table->setRowCount(0);


        QTableWidgetItem* column0_name = new QTableWidgetItem(QString(QObject::tr("Activity")));

        column0_name->setTextColor("#000000");

        this->activities_table->setHorizontalHeaderItem(0,column0_name);
        QTableWidgetItem* column1_name = new QTableWidgetItem(QString(QObject::tr("Category")),QTableWidgetItem::Type);
        this->activities_table->setHorizontalHeaderItem(1,column1_name);
        QTableWidgetItem* column2_name = new QTableWidgetItem(QString(QObject::tr("Time")),QTableWidgetItem::Type);
        this->activities_table->setHorizontalHeaderItem(2,column2_name);

        this->topLayout->addWidget(this->stackedWidget);

            TimeTracker->setCentralWidget(this->main_widget);
            TimeTracker->setMaximumHeight(800);
            TimeTracker->setMaximumWidth(1200);
            retranslateUi(TimeTracker);

    } // setupUi

    void retranslateUi(QMainWindow *TimeTracker)
    {
        TimeTracker->setWindowTitle(QApplication::translate("TimeTracker", "TimeTracker", Q_NULLPTR));

    }
};

namespace Ui {
    class TimeTracker: public Ui_TimeTracker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMETRACKER_H
