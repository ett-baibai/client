#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->DisconnectButton->setEnabled(false);
    ui->SendButton->setEnabled(false);

    mTcpSocket = new QTcpSocket();//initialise client socket

    mUdpSocket = new QUdpSocket();
    mUdpSocket->bind(QHostAddress("127.0.0.1"), 2333);
    QObject::connect(mUdpSocket, &QUdpSocket::readyRead, this, &MainWindow::on_UdpRecvDataFunc);

    //It will go to func "RecvDataFunc" when there is data from server
    QObject::connect(mTcpSocket,&QTcpSocket::readyRead,this,&MainWindow::on_TcpRecvDataFunc);
/*
    mTimer=new QTimer;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(mShowPicture()));

    mbTimerBtnClicked=false;

    mPicFile = new QFile;

    mPName[0]="../RecvFile/Qt.jpg";
    mPName[1]="../RecvFile/test.jpg";
    */
}

MainWindow::~MainWindow()
{
    delete this->mTcpSocket;
    delete this->mTimer;
    delete this->mPicFile;
    //delete this->mPicFileBin;
    delete ui;
}


//connect to server
void MainWindow::on_ConnectButton_clicked()
{
    mTcpSocket->abort();//cancle current conncetion
    mTcpSocket->connectToHost("127.0.0.1",8888);

    if(!mTcpSocket->waitForConnected(3000))//failed to connect in 3s
        qDebug()<<("failed to connect due to time out!");
    else
    {
        ui->ConnectButton->setEnabled(false);
        ui->DisconnectButton->setEnabled(true);
        ui->SendButton->setEnabled(true);
        statusBar()->showMessage("connect succeed",5000);
    }
}


//disconnect from server
void MainWindow::on_DisconnectButton_clicked()
{

    mTcpSocket->disconnectFromHost();
    mTcpSocket->close();
    ui->ConnectButton->setEnabled(true);
    ui->DisconnectButton->setEnabled(false);
    ui->SendButton->setEnabled(false);

    statusBar()->showMessage("close...",1500);
}


//send conmand to server
void MainWindow::on_SendButton_clicked()
{
    qint64 WriteResult = mTcpSocket->write(ui->SendContentEdit->text().toLatin1());//send command
    bool bFlush = mTcpSocket->flush();

    if(WriteResult != -1 && bFlush == 1)//judge send succeedly or not
    {
        if(WriteResult == 0)
            QMessageBox::information(this,"error",tr("WriteResult = 0"));
        else
        {
            statusBar()->showMessage("send...",1500);
        }
    }
    else
        qDebug()<<"failed to write!";
}


void MainWindow::on_TcpRecvDataFunc()
{
    qDebug()<<"tcp msg";
    //获取对方发送的内容
    QByteArray array = mTcpSocket->readAll();
    //在编辑区中显示
    ui->RecvContentEdit->append(array);
    //write data into file via bin mode
/*
    std::ofstream PicFileBin;

    QString CommandStr;
    CommandStr = ui->SendContentEdit->text();
    if(CommandStr == "photo")
    {
        qDebug("photo");
        PicFileBin.open(RASPBERRY_PHOTO,std::ios_base::out|std::ios_base::binary);
    }
     else
    {
        qDebug("text");
        PicFileBin.open(TEXT_TXT,std::ios_base::out|std::ios_base::binary);
    }

    //open by binary and just write

    QByteArray QArrRecvBuff;

    int iRecvCount=1,iBuffCount=0;

    while(1)
    {

        if(mTcpSocket->waitForReadyRead(200) == false)
        {
            qDebug()<<"Recv all";
            break;
        }
        else
        {
            QArrRecvBuff = mTcpSocket->readAll();
            //qDebug("%d,socket size = %u,recv size=%d\n",iRecvCount,(unsigned int)(mTcpSocket->size()),QArrRecvBuff.size());

            for(iBuffCount=0;iBuffCount<QArrRecvBuff.size();iBuffCount++)
                PicFileBin<<QArrRecvBuff[iBuffCount];//write in data

            iRecvCount++;
        }
    }

    mPicFile->close();
    */
}


void MainWindow::on_UdpRecvDataFunc()
{
    qDebug()<<"udp msg";
    //获取对方发送的内容
    QByteArray arr;
    while(mUdpSocket->hasPendingDatagrams())
    {
        arr.resize(mUdpSocket->pendingDatagramSize());
        mUdpSocket->readDatagram(arr.data(), arr.size());
    }
    //在编辑区中显示
    ui->RecvContentEdit->append(arr);

}
//open test photo
void MainWindow::on_OpenTestPhotoButton_clicked()
{
    ui->PictureLabel->setPixmap(mPName[0]);
    update();
}


//start or stop show photo in every interval
void MainWindow::on_OpenTimerButton_clicked()
{
    if(mbTimerBtnClicked == false)
    {
        qDebug()<<("start...");
        ui->OpenTimerButton->setText("close timer");
        mTimer->start(800);
        mbTimerBtnClicked=true;
    }

    else
    {
        qDebug()<<("stop");
        ui->OpenTimerButton->setText("open timer");
        mTimer->stop();
        mbTimerBtnClicked=false;
    }

}


//slot func of timer
void MainWindow::mShowPicture()
{
    static int count=0;
    ui->PictureLabel->setPixmap(mPName[count]);
    update();

    count++;
    if(count>1)count=0;
}


//open received photo
void MainWindow::on_OpenRecvPhoto_clicked()
{
    QString RecvPhoto(RASPBERRY_PHOTO);
    ui->PictureLabel->setPixmap(RecvPhoto);
    update();
}
