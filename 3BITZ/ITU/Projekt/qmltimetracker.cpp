#include "qmltimetracker.h"
#include <QObject>
#include <iostream>

QMLTimeTracker::QMLTimeTracker(QObject *parent):QObject(parent)
{

}


void QMLTimeTracker::test(int layout){
    w->change_actual_layout(layout);
}
