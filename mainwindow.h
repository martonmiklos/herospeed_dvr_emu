#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>

namespace Ui {
class MainWindow;
}

class Client {
public:
    int id;
    QTcpSocket *socket;
    QByteArray buffer;
    int state;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void newConnection();
    void socketDataReady(int id);
    void socketClosed(int id);

private:
    Ui::MainWindow *ui;
    QTcpServer *m_server;

    QSignalMapper m_disconnectMapper, m_dataReadyMapper;

    QList<Client*> m_clients;

    void logString(QString str);
};

#endif // MAINWINDOW_H
