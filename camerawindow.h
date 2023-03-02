#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QApplication>
#include <QOpenGLWidget>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn_superres.hpp>
#include <QBasicTimer>
#include <QPainter>
#include <QEvent>
#include "controls.h"
#include "controlImage.h"

class CameraWindow : public QOpenGLWidget
{
public:
    CameraWindow(QWidget *parent = nullptr);

protected:
    void paintGL() override;

    void timerEvent(QTimerEvent *event) override;

private:
    QBasicTimer m_timer;
};

#endif // CAMERAWINDOW_H
