#include "controls.h"

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 1000000;
static uint16_t delay;
int fd;

CControls OControls;

CControls::CControls()
        :joystickX(0), joystickY(0), controlMode(manual)
{
}

CControls::~CControls()
{
        close(fd);
}

void CControls::readControls()
{
    int ret = 0;

    fd = open(device, O_RDWR);
    if (fd < 0)
        pabort("can't open device");

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
        pabort("can't get spi mode");

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't get bits per word");

    /*
     * max speed
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't get max speed hz");

    //printf("spi mode: %d\n", mode);
    //printf("bits per word: %d\n", bits);
    //printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

    while(1)
    {
        transfer(fd);
        usleep(100);
    }
}

enum mode CControls::getMode(){ return controlMode; }

static uint64_t lastButton = 0;

uint64_t CControls::getJoystickX(void){ return joystickX; }

void CControls::transfer(int fd)
{
    int ret;
    uint8_t tx[] = {0b00000001, 0b10000000, 0b00000000};
    uint8_t rx[ARRAY_SIZE(tx)] = {0, };
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = ARRAY_SIZE(tx),
        .speed_hz = speed,
        .delay_usecs = delay,
        .bits_per_word = bits,
    };

    tx[1] = 0b10000000;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message");

    joystickY = (rx[1] << 8 ) | (rx[0] & 0xff);
    //printf("%d", joystickY);
    if(controlMode == manual)
        if(joystickY > 300)
                rotateMotor(-25);
        else if(joystickY < 200)
                rotateMotor(25);

   // puts("\n");

    // Read channel 1
    tx[1] = 0b10010000;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message");
    joystickX = (rx[1] << 8 ) | (rx[0] & 0xff);
    //puts("\n");

    // Read channel 2
    tx[1] = 0b10100000;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message");
    if(((rx[1] << 8) | (rx[0] & 0xff)) < 256 && lastButton == 1)
    {
        if(controlMode == automatic)
                controlMode = manual;
        else
                controlMode = automatic;
    }
    if(((rx[1] << 8) | (rx[0] & 0xff)) > 256)
        lastButton = 1;
    else
        lastButton = 0;

   // printf("%d\n", ((rx[1] << 8) | (rx[0] & 0xff)));
    //puts("\n");
    //printf("%d", controlMode);
    //puts("\n");
}

static void pabort(const char *s)
{
    perror(s);
    abort();
}

void rotateMotor(int value)
{
        char buffer[4];

        sprintf(buffer, "%d", value);
        int fp = open("/dev/led0", O_WRONLY);
        write(fp, buffer, sizeof(buffer));
        close(fp);
}
