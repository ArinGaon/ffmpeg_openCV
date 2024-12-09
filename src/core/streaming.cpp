#include "streaming.h"
#include <QDebug>

Streaming::Streaming(QObject* parent)
    : QObject(parent), streamingProcess(nullptr)
{
}

Streaming::~Streaming()
{
    stop(); // 객체 소멸 시 스트리밍 프로세스 종료
}

bool Streaming::start(const QString& streamURL, const QString& cameraName)
{
    // FFmpeg 명령어 구성
    QString command = QString("ffmpeg -re -f dshow -rtbufsize 300M -i video=\"%1\" -c:v libx264 -preset veryfast -f flv %2")
        .arg(cameraName)
        .arg(streamURL);

    // 기존 프로세스 종료 방지
    if (streamingProcess) {
        qDebug() << "Streaming is already running.";
        return false;
    }

    // QProcess 초기화 및 실행
    streamingProcess = new QProcess(this);
    streamingProcess->start(command);

    if (!streamingProcess->waitForStarted()) {
        qDebug() << "Failed to start streaming process!";
        delete streamingProcess;
        streamingProcess = nullptr;
        return false;
    }

    qDebug() << "Streaming started with command:" << command;
    return true;
}

void Streaming::stop()
{
    if (streamingProcess) {
        streamingProcess->kill(); // 프로세스 강제 종료
        streamingProcess->deleteLater();
        streamingProcess = nullptr;

        qDebug() << "Streaming process stopped.";
    }
    else {
        qDebug() << "No active streaming process to stop.";
    }
}
