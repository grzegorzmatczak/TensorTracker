#pragma once

#include <QObject>

#include "global.h"

class MedianFilter;

class QElapsedTimer;

namespace logger
{
    class Logger;
}

class FrameMedian : public QObject
{
    Q_OBJECT

public:
    FrameMedian(const QJsonObject& config);
    ~FrameMedian();
    void configure(const QJsonObject& config);

public slots:
    void onUpdate();
    void onUpdateFrame(ProcessingStruct processing);
signals:
    void updateMedian(ProcessingStruct processing);
    void showImagesOpenCV(cv::Mat frame, std::string name);
private:
    qint64 mTimestamp{};
    std::unique_ptr<logger::Logger> mLogger;
    cv::Mat mMedian;
    MedianFilter* mMedianFilter;
    int mWidth{ 300 };
    int mHeight{ 300 };
    int mSizeOfMedian{ 10 };
    QElapsedTimer* mTimer;
};