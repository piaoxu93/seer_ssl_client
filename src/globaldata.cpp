#include "globaldata.h"

CGlobalData::CGlobalData()
    :camera{DataQueue<ReceiveVisionMessage>(100),DataQueue<ReceiveVisionMessage>(100)
            ,DataQueue<ReceiveVisionMessage>(100),DataQueue<ReceiveVisionMessage>(100)}
    ,vision(DataQueue<VisionMessage>(100)){
    for(int i=0;i<PARAM::CAMERA;i++)
        cameraControl[i] = true;
}
