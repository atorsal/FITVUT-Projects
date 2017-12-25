#ifndef QMLTIMETRACKER_H
#define QMLTIMETRACKER_H
#include <QObject>
#include "timetracker.h"

extern TimeTracker *w;
class QMLTimeTracker:public QObject{
      Q_OBJECT

public:
    QMLTimeTracker(QObject *parent = nullptr);

    Q_INVOKABLE void test(int layout);


};

#endif // QMLTIMETRACKER_H
