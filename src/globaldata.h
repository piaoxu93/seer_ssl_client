#ifndef GLOBALDATA_H
#define GLOBALDATA_H
#include "singleton.hpp"
#include "visionmodule.h"
#include "dataqueue.hpp"
#include "params.h"
class CGlobalData
{
public:
    CGlobalData();
    DataQueue<ReceiveVisionMessage> camera[PARAM::CAMERA];
    DataQueue<VisionMessage> vision;
    bool cameraControl[PARAM::CAMERA];
};
typedef Singleton<CGlobalData> GlobalData;
#endif // GLOBALDATA_H
