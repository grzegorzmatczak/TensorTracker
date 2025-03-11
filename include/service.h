#pragma once

#include "tracker_global.h"

#include <QObject>

namespace logger
{
    class Logger;
}

class FrameSource;
class CameraThread;
class FrameProcessing;
class FrameMedian;

class Service : public QObject
{
    Q_OBJECT

public:
    Service();
    Service(const QJsonObject& config);
    ~Service();
    void configure(const QJsonObject& config);
private slots:
    void onUpdate();
public slots:
    void onShowImagesOpenCV(cv::Mat frame, std::string name);
private:
    void createStartupThreads(const QJsonObject& config);
private:
    FrameSource* capture{ nullptr };
    QThread* captureThread{ nullptr };
    CameraThread* camera{ nullptr };
    QThread* cameraThread{ nullptr };
    FrameProcessing* frameProcessing{ nullptr };
    QThread* frameProcessingThread{ nullptr };
    FrameMedian* frameMedian{ nullptr };
    QThread* frameMedianThread{ nullptr };
    

    QTimer* mTimer{ nullptr };
    QTimer* mTimerCamera{ nullptr };

    std::unique_ptr<logger::Logger> mLogger;

    int mCameraTimerValue{ 100 };
    int mCameraFPS{ 25 };

};
