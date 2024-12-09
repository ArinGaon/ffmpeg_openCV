#ifndef STREAMING_H
#define STREAMING_H

#include <QObject>
#include <QProcess>
#include <QString>

class Streaming : public QObject
{
    Q_OBJECT

public:
    explicit Streaming(QObject* parent = nullptr);
    ~Streaming();

    bool start(const QString& streamURL, const QString& cameraName = "Microsoft® LifeCam HD-3000");
    void stop();

private:
    QProcess* streamingProcess; // FFmpeg 프로세스 관리
};

#endif // STREAMING_H
