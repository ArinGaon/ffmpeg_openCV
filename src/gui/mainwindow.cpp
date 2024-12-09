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
    , videoProcessor(new VideoProcessor()) // videoProcessor 객체
    , streaming(new Streaming(this)) // Streaming 객체 생성
{
    ui->setupUi(this);

    QString streamURL = "http://138.2.123.234:8080/live_stream.html";
    videoCapture.open(streamURL.toStdString());
    // OpenCV VideoCapture 초기화 (웹캠 or 스트리밍 소스)
    if (!videoCapture.isOpened()) {
        qDebug() << "Failed to open video source!";
         return;
    }

    // QTimer로 프레임 업데이트
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(30); // 30ms 간격으로 업데이트 (약 33fps)


    // 초기 이벤트 경로 설정 (기본값)
    eventVideoPath = "C:/ArinGaon/test";
    ui->eventPathLabel->setText(eventVideoPath); // UI에 초기값 표시
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
    // VideoProcessor의 녹화 시작 메서드 호출

    if (videoProcessor->StartFullRecording("output.mp4")) {
        QMessageBox::information(this, "Record", "Recording started!");
    }
    else {
        QMessageBox::critical(this, "Record", "Failed to start recording!");
    }
    
}

void MainWindow::on_stopRecordButton_clicked()
{
    // VideoProcessor의 녹화 중지 메서드 호출
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

    // OpenCV의 BGR 이미지를 Qt의 RGB 이미지로 변환
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage qImg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    // QLabel에 이미지 표시
    ui->videoLabel->setPixmap(QPixmap::fromImage(qImg).scaled(
        ui->videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// 슬롯 구현
void MainWindow::on_setEventPathButton_clicked()
{
    // QFileDialog로 디렉토리 선택
    QString selectedDir = QFileDialog::getExistingDirectory(this, "Select Event Video Directory", eventVideoPath);
    if (!selectedDir.isEmpty()) {
        eventVideoPath = selectedDir + "/"; // 새로운 경로 설정
        ui->eventPathLabel->setText(eventVideoPath); // UI 업데이트
    }
    else {
        QMessageBox::information(this, "Info", "Event video path not changed.");
    }
}