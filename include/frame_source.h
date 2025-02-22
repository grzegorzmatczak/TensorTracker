#pragma once

#include <QObject>

#include "global.h"

namespace logger
{
    class Logger;
}

class FrameSource : public QObject
{
    Q_OBJECT

public:
    FrameSource(const QJsonObject& config);
    virtual ~FrameSource();
    void configure(const QJsonObject& config);
private:

public slots:
    virtual void onUpdate();
signals:
    void updateFrame(ProcessingStruct processing);
protected:
    std::unique_ptr<logger::Logger> mLogger;
    cv::Mat mFrame;
};
