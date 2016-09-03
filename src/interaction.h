#ifndef INTERACTION_H
#define INTERACTION_H

#include <QObject>
#include "params.h"
class Interaction : public QObject
{
    Q_OBJECT
public:
    explicit Interaction(QObject *parent = 0);
public:
    Q_INVOKABLE int getCameraNumber() { return PARAM::CAMERA; }
signals:
    void fieldChange(bool);
public slots:
};

#endif // INTERACTION_H
