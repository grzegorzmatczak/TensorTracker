#include "frame_source.h"

#include "logger.hpp"

#include <QJsonObject>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>

FrameSource::FrameSource(const QJsonObject& config)
{
    configure(config);
}

FrameSource::~FrameSource()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
}

void FrameSource::configure(const QJsonObject& config)
{
    mLogger = std::make_unique<logger::Logger>(logger::LogType::FRAME_SOURCE, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
    mLogger->showThreadId(true);
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
}

void FrameSource::onUpdate()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
}
