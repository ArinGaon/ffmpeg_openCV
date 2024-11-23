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
    , videoProcessor(new VideoProcessor()) // videoProcessor 객체
{
    ui->setupUi(this);

    // OpenCV VideoCapture 초기화 (웹캠 or 스트리밍 소스)
    videoCapture.open(0); // 0은 기본 웹캠. 스트리밍 URL을 사용할 수도 있음.
    if (!videoCapture.isOpened()) {
        qDebug() << "Failed to open video source!";
        return;
    }

    // QTimer로 프레임 업데이트
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(30); // 30ms 간격으로 업데이트 (약 33fps)

}

MainWindow::~MainWindow()
{
    delete ui;
    delete videoProcessor;
}

void MainWindow::on_startRecordButton_clicked() 
{
    videoProcessor->ProcessVideo();
    // VideoProcessor의 녹화 시작 메서드 호출
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
    // VideoProcessor의 녹화 중지 메서드 호출
    videoProcessor->StopFullRecording();
    QMessageBox::information(this, "Record", "Recording stopped!");

}

void MainWindow::on_startStreamButton_clicked()
{
    // 스트리밍 URL을 가져와 VideoProcessor 메서드에 전달
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

    // OpenCV의 BGR 이미지를 Qt의 RGB 이미지로 변환
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage qImg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    // QLabel에 이미지 표시
    ui->videoLabel->setPixmap(QPixmap::fromImage(qImg).scaled(
        ui->videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}