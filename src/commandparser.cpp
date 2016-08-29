#include "commandparser.h"
#include <QTextStream>
#include <QtDebug>
#include "singleparams.h"
#include <iostream>
CommandParser::CommandParser(QObject *parent) : QObject(parent){
}
bool CommandParser::sendCommand(const QString& command){
    std::cout << std::endl << SingleParams::instance()->_(command.toStdString()) << std::endl;
    return true;
}
