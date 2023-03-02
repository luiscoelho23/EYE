#ifndef __CONTROLIMAGE_H__
#define __CONTROLIMAGE_H__

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#define WINDOWNAME "Main Window"

using namespace cv;



class CControlImage
{
        public:
                VideoCapture cap;
                Mat frame;
                CControlImage();
                void dnnControl(void);
};

extern CControlImage OFrame;

#endif
