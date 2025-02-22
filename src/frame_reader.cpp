#include "frame_reader.h"

#include "global.h"
#include "logger.hpp"
#include "configreader.hpp"

#include <QMessageBox>
#include <QJsonObject>
#include <QDir>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>

FrameReader::FrameReader(const QJsonObject& config)
    : FrameSource(config)
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
    configure(config);
}

FrameReader::~FrameReader()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
}

void FrameReader::configure(const QJsonObject& config)
{
    mLogger = std::make_unique<logger::Logger>(logger::LogType::FRAME_READER, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
    mLogger->showThreadId(true);
    QJsonObject datasetConfig{};
    #ifdef _WIN32
        mSplit = "\\";
        datasetConfig = config[DATASET_WIN32].toObject();
    #endif // _WIN32
    #ifdef __linux__
        
        mSplit = "/";
    #endif // __linux__
    
    QString configName = datasetConfig[CONFIG_NAME].toString();
    QString configPath = datasetConfig[PATH_TO_DATASET].toString();
    QString configPathWithName = configPath + configName;


    std::unique_ptr<ConfigReader> configReader = std::make_unique<ConfigReader>();
    if (!configReader->readConfig(configPathWithName, datasetConfig))
    {
        mLogger->printError(QString("Config %1 not valid").arg(configPathWithName), __FUNCTION__);
    }
    mInputType = datasetConfig[INPUT_TYPE].toString();
    QString cleanFramesPrefix = datasetConfig[DIR_CLEAN].toString();
    QString cleanFramesPath = configPath + cleanFramesPrefix;
    int allFramesCounter = datasetConfig[ALL_FRAMES].toInt();

#ifdef _WIN32
    loadDataFromStreamWindows(cleanFramesPath, mFrames, allFramesCounter);
#endif // _WIN32

    if(mFrames.size() > 0)
        mLogger->print(QString("mFrame:(%1 x %2)").arg(mFrames[0].rows).arg(mFrames[0].cols), __FUNCTION__);
}

void FrameReader::onUpdate()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
    iteration++;
    if (iteration > mFrames.size())
        iteration = 0;

    mLogger->print(QString("mFrames[iteration]:(%1 x %2 x%3)")
        .arg(mFrames[iteration].rows)
        .arg(mFrames[iteration].cols)
        .arg(mFrames[iteration].channels()), __FUNCTION__);
    emit updateFrame({mFrames[iteration], QDateTime::currentMSecsSinceEpoch() });
}

void FrameReader::loadDataFromStreamWindows(const QString& path, std::vector<cv::Mat>& data, int framesNumber)
{
    std::vector<QString> m_imgList = scanAllImages(path);
    std::sort(m_imgList.begin(), m_imgList.end());

    if (m_imgList.size() > 0)
    {
        for (qint32 iteration = 0; iteration < m_imgList.size(); iteration++)
        {
            if (iteration % 100 == 0)
            {
                mLogger->print(QString("loaded frames:%1").arg(iteration), __FUNCTION__);
            }
            QString name = path + mSplit + m_imgList[iteration] + mInputType;

            cv::Mat inputMat = cv::imread((name).toStdString(), cv::IMREAD_GRAYSCALE);
            data.push_back(inputMat);

            if (iteration > framesNumber)
            {
                mLogger->print(QString("stop loading on:%1").arg(iteration), __FUNCTION__);
                break;
            }
        }
    }
    else
    {
        mLogger->printError(QString("path not valid:%1").arg(path), __FUNCTION__);
    }
}

std::vector<QString> FrameReader::scanAllImages(const QString& path)
{
    std::vector<QString> temp;
    QDir directory(path);
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.png" << "*.PNG" << "*.JPG", QDir::Files);

    foreach(QString filename, images)
    {
        QStringList sl = filename.split(".");
        temp.push_back(sl[0]);
    }
    return temp;
}

std::vector<QString> FrameReader::scanAllVideo(const QString& path)
{
    std::vector<QString> temp;
    QDir directory(path);
    QStringList images = directory.entryList(QStringList() << "*.MP4" << "*.mp4", QDir::Files);

    foreach(QString filename, images)
    {
        QStringList sl = filename.split(".");
        temp.push_back(sl[0]);
    }
    return temp;
}