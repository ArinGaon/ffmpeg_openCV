#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include "video_processor.h"
#include "streaming.h"
#include "opencv2/opencv.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// 버튼 클릭시 호출된 함수 (private)
private slots:
    void updateFrame();
    void on_startRecordButton_clicked();
    void on_stopRecordButton_clicked();
    void on_startStreamButton_clicked();
    void on_stopStreamButton_clicked();
    // 클릭 함수기에 clicked로 선언하고 Qt Creater에 만들어놓은 블럭명과 일치시킴
    void on_setEventPathButton_clicked(); // event 슬롯 선언


private:
    Ui::MainWindow *ui;
    VideoProcessor *videoProcessor; // 객체 선언
    cv::VideoCapture videoCapture;
    QTimer* timer;
    Streaming* streaming; // streaming 객체
    QString eventVideoPath; // 이벤트 저장 경로
};
#endif // MAINWINDOW_H
