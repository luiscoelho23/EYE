#ifndef __CONTROLS_H__
#define __CONTROLS_H__

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

enum mode{ manual = 0, automatic};

class CControls
{
        private:
                uint64_t joystickX;
                uint64_t joystickY;
                enum mode controlMode;

                void transfer(int);
        public:
                CControls();
                ~CControls();
                void readControls();
                enum mode getMode();
                uint64_t getJoystickX(void);
};

static void pabort(const char *);

void rotateMotor(int);

extern CControls OControls;

#endif
