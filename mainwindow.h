#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void socket_connected();
    void socket_disconnected();
    void data_recieved();

private:
    Ui::MainWindow *ui;
    QTcpSocket socket;
    const QString lamp_off = "background-color: rgb(0, 0, 0);border-radius: 15px;";
    const QString lamp_on_no_color = "background-color: rgb(255, 255, 255);border-radius: 15px;";
    const QString lamp_on_colored = "background-color: rgb(%1, %2, %3);border-radius: 15px;";


};
#endif // MAINWINDOW_H
