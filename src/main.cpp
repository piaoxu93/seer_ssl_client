#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QtDebug>
#include <iostream>
#include "commandparser.h"
#include "field.h"
#include "singleparams.h"
#include "serialobject.h"
#include "radiopacket.h"
#include "visionmodule.h"
#include "refereebox.h"
#include "interaction.h"
#include "translation.hpp"
#include "threadmanager.h"
using namespace std;


void qmlRegister(){
    qmlRegisterType<CommandParser>("Client.Component", 1, 0, "CommandParser");
    qmlRegisterType<Field>("Client.Component", 1, 0, "Field");
    qmlRegisterType<SerialObject>("Client.Component", 1, 0, "Serial");
    qmlRegisterType<RefBoxCommand>("Client.Component", 1, 0, "RefBoxCommand");
    qmlRegisterType<GameState>("Client.Component", 1, 0, "GameState");
    qmlRegisterType<RefereeBox>("Client.Component", 1, 0, "RefereeBox");
    qmlRegisterType<Interaction>("Client.Component", 1, 0, "Interaction");
    qmlRegisterType<TranslationTest>("Client.Component", 1, 0, "Translator");
}
void threadRegister(){
    ThreadManager::instance();
}
int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    threadRegister();
    qmlRegister();
    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/src/qml/main.qml")));
    return app.exec();
}
