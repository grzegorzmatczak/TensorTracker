#pragma once

#include "tracker_global.h"
#include "frame_source.h"

class FrameReader : public FrameSource
{
    Q_OBJECT
public:
    FrameReader(const QJsonObject& config);
    ~FrameReader();
    void configure(const QJsonObject& config);
private:
    void loadDataFromStreamWindows(const QString& path, std::vector<cv::Mat>& data, int framesNumber);
    std::vector<QString> scanAllImages(const QString& path);
    std::vector<QString> scanAllVideo(const QString& path);
public slots:
    void onUpdate() override;
//signals:
    //void updateFrame(ProcessingStruct processing);
private:
    QString mSplit{};
    QString mInputType{};
    std::vector<cv::Mat> mFrames;
    int iteration{ 0 };
};
