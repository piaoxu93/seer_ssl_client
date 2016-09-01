#ifndef PARAMS_H
#define PARAMS_H
#include "singleparams.h"
const int PI = 3.1415926;
namespace PARAM {
    const int CAMERA = 4;
    const int ROBOTNUM = 12;
    const int BALLNUM = 20;
    const int ROBOTMAXID = 12;
    namespace FOLLOW{
        const int ERRORNUM = 9999;
        const int INITNUM = -1;
        const int RECEIVECARNUM = 12;
        const int SENDCARNUM = 6;
        const int MAXFOLLOWNUM = 12;
        const int CALIERROR = 100;
        const int BALLCALIERROR = 200;
        const int FRAMERATE = 60;
    }
}
#endif // PARAMS_H
