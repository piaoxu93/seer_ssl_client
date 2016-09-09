#ifndef VISIONTHREAD_H
#define VISIONTHREAD_H

#include <QObject>
#include "singleton.hpp"
#include "visionmodule.h"
class CVisionThread : public QObject
{
    Q_OBJECT
public:
    explicit CVisionThread(QObject *parent = 0);

signals:
    void finished();
    void error(QString err);
public slots:
    void process(){
//        VisionModule::instance()->process();
    }
    void resume(){
//        VisionModule::instance()->resume();
    }
    void pause(){
//        VisionModule::instance()->pause();
    }
};
typedef Singleton<CVisionThread> VisionThread;
#endif // VISIONTHREAD_H
