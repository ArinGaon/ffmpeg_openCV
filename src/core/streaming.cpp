#include "streaming.h"
#include <QDebug>

Streaming::Streaming(QObject* parent)
    : QObject(parent), streamingProcess(nullptr)
{
}

Streaming::~Streaming()
{
    stop(); // ��ü �Ҹ� �� ��Ʈ���� ���μ��� ����
}

bool Streaming::start(const QString& streamURL, const QString& cameraName)
{
    // FFmpeg ��ɾ� ����
    QString command = QString("ffmpeg -re -f dshow -rtbufsize 300M -i video=\"%1\" -c:v libx264 -preset veryfast -f flv %2")
        .arg(cameraName)
        .arg(streamURL);

    // ���� ���μ��� ���� ����
    if (streamingProcess) {
        qDebug() << "Streaming is already running.";
        return false;
    }

    // QProcess �ʱ�ȭ �� ����
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
        streamingProcess->kill(); // ���μ��� ���� ����
        streamingProcess->deleteLater();
        streamingProcess = nullptr;

        qDebug() << "Streaming process stopped.";
    }
    else {
        qDebug() << "No active streaming process to stop.";
    }
}
