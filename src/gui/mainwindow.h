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

// ��ư Ŭ���� ȣ��� �Լ� (private)
private slots:
    void updateFrame();
    void on_startRecordButton_clicked();
    void on_stopRecordButton_clicked();
    void on_startStreamButton_clicked();
    void on_stopStreamButton_clicked();
    // Ŭ�� �Լ��⿡ clicked�� �����ϰ� Qt Creater�� �������� ����� ��ġ��Ŵ
    void on_setEventPathButton_clicked(); // event ���� ����


private:
    Ui::MainWindow *ui;
    VideoProcessor *videoProcessor; // ��ü ����
    cv::VideoCapture videoCapture;
    QTimer* timer;
    Streaming* streaming; // streaming ��ü
    QString eventVideoPath; // �̺�Ʈ ���� ���
};
#endif // MAINWINDOW_H
