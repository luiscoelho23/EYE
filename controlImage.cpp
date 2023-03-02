#include "controlImage.h"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include "controls.h"

using namespace std;
using namespace cv;

CControlImage OFrame;

CControlImage::CControlImage()
        :cap(0,CAP_V4L2)
{
        cap.set(CAP_PROP_FRAME_WIDTH, 800);
        cap.set(CAP_PROP_FRAME_HEIGHT, 480);
        cap.set(CAP_PROP_FPS, 60);

        if(!cap.isOpened())
        {
                cout << "error opening camera\n";
                exit(1);
        }
}

Ptr<FaceDetectorYN> detector = FaceDetectorYN::create("facedetection.onnx", "", Size(320, 320), 0.96, 0.96, 5000);

void CControlImage::dnnControl(void)
{
        while(1)
        {
                if(!frame.empty() && OControls.getMode() == automatic)
                {
                        detector->setInputSize(frame.size());
                        Mat faces1;
                        detector->detect(frame, faces1);

                        for (int i = 0; i < faces1.rows; i++)
                        {
                                if(OControls.getMode() == automatic)
                                        rotateMotor((int)(faces1.at<float>(0,0) - 350) * 0.08857);
                        }

                }
        }
}
