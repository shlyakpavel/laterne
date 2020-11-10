#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->colorFrame->setStyleSheet(lamp_off);
    connect(&socket, &QTcpSocket::connected, this, &MainWindow::socket_connected);
    connect(&socket, &QTcpSocket::disconnected, this, &MainWindow::socket_disconnected);
    connect(&socket, &QTcpSocket::readyRead, this, &MainWindow::data_recieved);
}

MainWindow::~MainWindow()
{
    socket.disconnectFromHost();
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    if (ui->connectButton->text() == "Connect"){
        QUrl url (ui->urlEdit->text());
        socket.connectToHost(url.host(),url.port());
    }
    else{
        socket.disconnectFromHost();
    }
}

void MainWindow::socket_connected()
{
    ui->connectButton->setText("Disconnect");
    qDebug() << "Connected fine";
}

void MainWindow::socket_disconnected()
{
    ui->connectButton->setText("Connect");
    qDebug() << "Disconnected";
}

void MainWindow::data_recieved(){
    QByteArray data = socket.readAll();
    // Assume data.at(1) is always zero as we're never going to recieve more than 3 bytes of data
    // Nothing bad happens otherwise, so let's just skeep the test
    switch (data.at(0)){
    case 0x12:
        // Turn the lamp on
        if (data.at(2) != 0)
            qDebug() << "Unknown command extention detected: lamp_on. Perhaps you should update this app.";
        ui->colorFrame->setStyleSheet(lamp_on_no_color);
        break;
    case 0x13:
        // Turn the lamp off
        if (data.at(2) != 0)
            qDebug() << "Unknown command extention detected: lamp_off. Perhaps you should update this app.";
        ui->colorFrame->setStyleSheet(lamp_off);
        break;
    case 0x20:
        if (data.at(2) != 3){
            qDebug() << "Malformed command: color switch. Nothing done. Perhaps you should update this app.";
        }
        else {
        QString red = QString::number((unsigned char) data.at(3));
        QString green = QString::number((unsigned char) data.at(4));
        QString blue = QString::number((unsigned char) data.at(5));
        QString QSS = lamp_on_colored.arg(red).arg(green).arg(blue);
        ui->colorFrame->setStyleSheet(QSS);
        }
        break;
    default:
        qDebug() << "Unknown command detected. Update the app.";
    }
}
