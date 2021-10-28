#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket> //通信套接字//对方的(客户端的)套接字(通信套接字)
#include <QMessageBox>
#include <QLabel>
#include <unistd.h> // sleep func
#include <QTimer>
#include <QFile>
#include <fstream>
#include <iostream>
#include <QUdpSocket>

#define RASPBERRY_PHOTO "../RecvFile/Photo.jpg"
#define TEXT_TXT "../RecvFile/Test.txt"
#define RECV_BUFF_SIZE 1024

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //receive from server


private slots:
    void on_ConnectButton_clicked();

    void on_DisconnectButton_clicked();

    void on_SendButton_clicked();

    void on_OpenTestPhotoButton_clicked();

    void on_OpenTimerButton_clicked();

    void mShowPicture();

    void on_TcpRecvDataFunc();

    void on_UdpRecvDataFunc();

    void on_OpenRecvPhoto_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket *mTcpSocket;
    QUdpSocket *mUdpSocket;
    QTimer *mTimer;
    QFile *mPicFile;
    bool mbTimerBtnClicked;
    QString mPName[2];
};

#endif // MAINWINDOW_H
