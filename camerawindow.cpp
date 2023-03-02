#include "camerawindow.h"

static dnn_superres::DnnSuperResImpl dnnSuperRes;

CameraWindow::CameraWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(800,480);
    dnnSuperRes.readModel("LapSRN_x8.pb");
    dnnSuperRes.setModel("lapsrn",8);
    m_timer.start(16, this);
}

enum xState {normal, superRes};
xState lastX = normal;
Mat lastSuper;

void CameraWindow::paintGL()
{
    OFrame.cap >> OFrame.frame;
    if (!OFrame.frame.empty())
    {
        Mat tempImage = OFrame.frame;
        if(OControls.getJoystickX() > 300)
        {
            double percent = OControls.getJoystickX() * 0.000747863247863 - 0.124358974359;
            if(percent > 0.45)
                percent = 0.45;
            if(percent < 0.1)
                percent = 0.1;

            Rect roi(OFrame.frame.cols*percent, OFrame.frame.rows*percent, OFrame.frame.cols*(1-2*percent), OFrame.frame.rows*(1-2*percent));
            tempImage = OFrame.frame(roi);
            cv::resize(tempImage, tempImage, Size(800,480),cv::INTER_LANCZOS4);
         }
        if(OControls.getJoystickX() < 10 && lastX == normal)
        {
            Rect roi(OFrame.frame.cols*0.45, OFrame.frame.rows*0.45, OFrame.frame.cols*(1-2*0.45), OFrame.frame.rows*(1-2*0.45));
            tempImage = OFrame.frame(roi);
            dnnSuperRes.upsample(tempImage,tempImage);
            cv::resize(tempImage, tempImage, Size(800,480),cv::INTER_LANCZOS4);
            lastSuper = tempImage;
            lastX = superRes;
        }
        else if(OControls.getJoystickX() < 10 && lastX == superRes)
        {
            lastX = normal;
            tempImage = lastSuper;
            cv::cvtColor(tempImage, tempImage, cv::COLOR_BGR2RGB);
        }
        else if(OControls.getJoystickX() > 10)
            lastX = normal;

        cv::cvtColor(tempImage, tempImage, cv::COLOR_BGR2RGB);
        QImage image(tempImage.data, tempImage.cols, tempImage.rows, QImage::Format_RGB888);
        QPainter painter(this);  
        painter.drawImage(QPoint(0, 0), image);
    }
}

void CameraWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timer.timerId()) {
        update();
    } else {
        QOpenGLWidget::timerEvent(event);
    }
}
