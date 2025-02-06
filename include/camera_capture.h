#pragma once

#include <QObject>

class CameraCapture : public QObject
{
	Q_OBJECT

public:
	CameraCapture();
	CameraCapture(QJsonObject const& config);
	~CameraCapture() {};
	void configure(QJsonObject const& config);
private:

private slots:
	void startCamera();
	void stopCamera();
	void displayCameraError();
private:
	int mDeviceID{ 0 };
};
