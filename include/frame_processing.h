#pragma once

#include <QObject>

#include "tracker_global.h"

class QElapsedTimer;

namespace logger
{
    class Logger;
}

class FrameProcessing : public QObject
{
    Q_OBJECT

public:
    FrameProcessing(const QJsonObject& config);
    ~FrameProcessing();
    void configure(const QJsonObject& config);

public slots:
    void onUpdate();
    void onUpdateFrame(ProcessingStruct processing);
    void onUpdateMedian(ProcessingStruct processing);
signals:
    void updateFrame(ProcessingStruct processing);
    void showImagesOpenCV(cv::Mat frame, std::string name);
private:
    qint64 mTimestamp{};
    std::unique_ptr<logger::Logger> mLogger;
    cv::Mat mDiffFrame;
    cv::Mat mDiffFrameThresh;
    cv::Mat mMedian;
    int mWidth{ 300 };
    int mHeight{ 300 };
    QElapsedTimer* mTimer;
};