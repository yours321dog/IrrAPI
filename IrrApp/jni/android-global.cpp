#include <sys/time.h>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device = 0;
IVideoDriver* driver = 0;
ISceneManager* smgr = 0;
ISceneCollisionManager *collMgr = 0;

int gWindowWidth = 640;
int gWindowHeight = 480;
stringc gSdCardPath = "/sdcard/";
SColor backColor = SColor(255,180,180,255);

bool _isInit = false;

long _getTime()
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}