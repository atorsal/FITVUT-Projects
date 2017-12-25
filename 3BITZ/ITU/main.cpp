//#include "timetracker.h"
#include "activity.h"
#include "qmltimetracker.h"
#include <QApplication>
#include <QMetaType>
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QQuickView>
#include <QTableWidget>
#include <QQmlApplicationEngine>
#include <QQmlContext>

TimeTracker *w;
bool is_stopped = false;
int main(int argc, char *argv[])
{
    qRegisterMetaType<std::map<std::string,std::pair<int,bool>>>("std::map<std::string,std::pair<int,bool>>");
    qRegisterMetaType<std::vector<Activity>>("std::vector<Activity>");
    qmlRegisterType<QMLTimeTracker>( "timetracker", 1, 0, "QMLTimeTracker" );

    QApplication a(argc, argv);
    QTranslator qtTranslator;
    qtTranslator.load("en",
                QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);
    w = new TimeTracker();
    w->show();
    return a.exec();
}
