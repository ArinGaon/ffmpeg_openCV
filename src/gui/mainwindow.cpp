#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "video_processor.h"
#include <QMessageBox>
#include <QImage>
#include <Qpixmap>

#include "opencv2/opencv.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , videoProcessor(new VideoProcessor()) // videoProcessor ��ü
{
    ui->setupUi(this);

    // OpenCV VideoCapture �ʱ�ȭ (��ķ or ��Ʈ���� �ҽ�)
    videoCapture.open(0); // 0�� �⺻ ��ķ. ��Ʈ���� URL�� ����� ���� ����.
    if (!videoCapture.isOpened()) {
        qDebug() << "Failed to open video source!";
        return;
    }

    // QTimer�� ������ ������Ʈ
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(30); // 30ms �������� ������Ʈ (�� 33fps)

}

MainWindow::~MainWindow()
{
    delete ui;
    delete videoProcessor;
}

void MainWindow::on_startRecordButton_clicked() 
{
    videoProcessor->ProcessVideo();
    // VideoProcessor�� ��ȭ ���� �޼��� ȣ��
   /* if (videoProcessor->StartFullRecording("output.mp4", 640, 480)) {
        QMessageBox::information(this, "Record", "Recording started!");
    }
    else {
        QMessageBox::critical(this, "Record", "Failed to start recording!");
    }
    */
}

void MainWindow::on_stopRecordButton_clicked()
{
    // VideoProcessor�� ��ȭ ���� �޼��� ȣ��
    videoProcessor->StopFullRecording();
    QMessageBox::information(this, "Record", "Recording stopped!");

}

void MainWindow::on_startStreamButton_clicked()
{
    // ��Ʈ���� URL�� ������ VideoProcessor �޼��忡 ����
    QString streamURL = ui->streamURLLineEdit->text();
    if (videoProcessor->startStreaming(streamURL.toStdString())) {
        QMessageBox::information(this, "Stream", "Streaming started!");
    }
    else {
        QMessageBox::critical(this, "Stream", "Failed to start streaming!");
    }

}

void MainWindow::on_stopStreambutton_clicked()
{
    videoProcessor->stopStreaming();
    ui->statusLabel->setText("Streaming stopped.");
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