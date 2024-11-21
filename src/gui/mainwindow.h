#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_startRecordButton_clicked();
    void on_stopRecordButton_clicked();
    void on_startStreamButton_clicked();
    void on_stopStreambutton_clicked();

    // Ŭ�� �Լ��⿡ clicked�� �����ϰ� Qt Creater�� �������� ����� ��ġ��Ŵ


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
