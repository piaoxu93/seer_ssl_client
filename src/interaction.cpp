#include "interaction.h"
#include "visionmodule.h"
#include "field.h"
Interaction::Interaction(QObject *parent) : QObject(parent) {
    connect(this,SIGNAL(fieldChange(bool)),VisionModule::instance(),SLOT(updateVisionControl(bool)));
}
