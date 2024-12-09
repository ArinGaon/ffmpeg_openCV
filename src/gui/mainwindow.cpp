#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "video_processor.h"
#include <QMessageBox>
#include <QImage>
#include <Qpixmap>
#include <QFileDialog>
#include "event_processor.h"
#include "streaming.h"
#include "opencv2/opencv.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , videoProcessor(new VideoProcessor()) // videoProcessor ��ü
    , streaming(new Streaming(this)) // Streaming ��ü ����
{
    ui->setupUi(this);

    QString streamURL = "http://138.2.123.234:8080/live_stream.html";
    videoCapture.open(streamURL.toStdString());
    // OpenCV VideoCapture �ʱ�ȭ (��ķ or ��Ʈ���� �ҽ�)
    if (!videoCapture.isOpened()) {
        qDebug() << "Failed to open video source!";
         return;
    }

    // QTimer�� ������ ������Ʈ
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(30); // 30ms �������� ������Ʈ (�� 33fps)


    // �ʱ� �̺�Ʈ ��� ���� (�⺻��)
    eventVideoPath = "C:/ArinGaon/test";
    ui->eventPathLabel->setText(eventVideoPath); // UI�� �ʱⰪ ǥ��
}

MainWindow::~MainWindow()
{
    delete ui;
    delete videoProcessor;
    delete streaming;
}

void MainWindow::on_startRecordButton_clicked() 
{
    // videoProcessor->ProcessVideo();
    // VideoProcessor�� ��ȭ ���� �޼��� ȣ��

    if (videoProcessor->StartFullRecording("output.mp4")) {
        QMessageBox::information(this, "Record", "Recording started!");
    }
    else {
        QMessageBox::critical(this, "Record", "Failed to start recording!");
    }
    
}

void MainWindow::on_stopRecordButton_clicked()
{
    // VideoProcessor�� ��ȭ ���� �޼��� ȣ��
    videoProcessor->StopFullRecording();
    QMessageBox::information(this, "Record", "Recording stopped!");

}

void MainWindow::on_startStreamButton_clicked()
{
    QString streamURL = ui->streamURLLineEdit->text();
    if (streamURL.isEmpty()) {
        QMessageBox::warning(this, "Error", "Stream URL is empty!");
        return;
    }

    if (streaming->start(streamURL)) {
        ui->statusLabel->setText("Streaming started.");
        QMessageBox::information(this, "Stream", "Streaming started!");
    }
    else {
        QMessageBox::critical(this, "Stream", "Failed to start streaming!");
    }
}

void MainWindow::on_stopStreamButton_clicked()
{
    streaming->stop();
    ui->statusLabel->setText("Streaming stopped.");
    QMessageBox::information(this, "Stream", "Streaming stopped!");
}


void MainWindow::updateFrame()
{
    cv::Mat frame;
    videoCapture >> frame;

    if (frame.empty()) return;

    // OpenCV�� BGR �̹����� Qt�� RGB �̹����� ��ȯ
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage qImg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    // QLabel�� �̹��� ǥ��
    ui->videoLabel->setPixmap(QPixmap::fromImage(qImg).scaled(
        ui->videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// ���� ����
void MainWindow::on_setEventPathButton_clicked()
{
    // QFileDialog�� ���丮 ����
    QString selectedDir = QFileDialog::getExistingDirectory(this, "Select Event Video Directory", eventVideoPath);
    if (!selectedDir.isEmpty()) {
        eventVideoPath = selectedDir + "/"; // ���ο� ��� ����
        ui->eventPathLabel->setText(eventVideoPath); // UI ������Ʈ
    }
    else {
        QMessageBox::information(this, "Info", "Event video path not changed.");
    }
}