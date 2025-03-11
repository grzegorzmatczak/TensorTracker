#pragma once

#include <QObject>

#include "tracker_global.h"

namespace logger
{
    class Logger;
}

class CameraThread : public QObject
{
    Q_OBJECT

public:
    CameraThread(const QJsonObject& config);
    ~CameraThread();
    void configure(const QJsonObject& config);

public slots:
    void onUpdate();
    void onUpdateFrame(ProcessingStruct processing);
    void initProcessing();
signals:
    void updateFrame(ProcessingStruct processing);
private:
    qint64 mTimestamp{};
    std::unique_ptr<logger::Logger> mLogger;
    cv::Mat mGrayFrame;
    int mWidth{ 300 };
    int mHeight{ 300 };
};