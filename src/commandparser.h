#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QObject>

class CommandParser : public QObject
{
    Q_OBJECT
public:
    CommandParser(QObject *parent = 0);
    Q_INVOKABLE bool sendCommand(const QString& command);
signals:

public slots:
};

#endif // COMMANDPARSER_H
