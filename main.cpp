#include <QCoreApplication>
#include "controlImage.h"
#include "controls.h"
#include "camerawindow.h"
#include <pthread.h>

using namespace cv;
using namespace std;

void* TReadCommands(void* arg);
void* TDnnControl(void* arg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pthread_t tReadCommands;
    pthread_attr_t attrReadCommands;
    cpu_set_t cpuset0;

    pthread_attr_init(&attrReadCommands);
    CPU_ZERO(&cpuset0);
    CPU_SET(0, &cpuset0); // set affinity to CPU 0
    pthread_attr_setaffinity_np(&attrReadCommands, sizeof(cpu_set_t), &cpuset0);

    pthread_create(&tReadCommands, &attrReadCommands, TReadCommands, nullptr);

    pthread_t tDnnControl;
    pthread_attr_t attrDnnControl;
    cpu_set_t cpuset1;

    pthread_attr_init(&attrDnnControl);
    CPU_ZERO(&cpuset1);
    CPU_SET(1, &cpuset1); // set affinity to CPU 1
    pthread_attr_setaffinity_np(&attrDnnControl, sizeof(cpu_set_t), &cpuset1);

    pthread_create(&tDnnControl, &attrDnnControl, TDnnControl, nullptr);

    CameraWindow cameraWindow;
    cameraWindow.show();
    return a.exec();
}

void* TReadCommands(void* arg)
{
    (void)arg;
    OControls.readControls();
    return nullptr;
}

void* TDnnControl(void* arg)
{
        (void)arg;
        OFrame.dnnControl();
        return nullptr;
}
