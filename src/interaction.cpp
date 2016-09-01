#include "interaction.h"
#include "visionmodule.h"
Interaction::Interaction(QObject *parent) : QObject(parent) {
    //connect(this,SIGNAL(visionControlChanged(int)),VisionModule::Instance(),SLOT(updateVisionControl(int)));
}
