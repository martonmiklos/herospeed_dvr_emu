#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>

uint8_t peer1_0[] = {
0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
0xbb, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x58, 0x43, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35 };
uint8_t peer1_1[] = {
0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t peer1_2[] = {
0x00, 0x00, 0x01, 0x01, 0x06, 0x00, 0x00, 0x00,
0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t peer1_3[] = {
0x00, 0x00, 0x01, 0x01, 0x06, 0x00, 0x00, 0x00,
0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_server = new QTcpServer(this);

    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if (!m_server->listen(QHostAddress::Any, 8670)) {
        qDebug() << "Unable to listen";
    } else {
        qDebug() << "Server started";
    }
}

void MainWindow::newConnection()
{
    while (m_server->hasPendingConnections()) {
        QTcpSocket *newSocket = m_server->nextPendingConnection();

        Client *client = new Client();
        client->socket = newSocket;
        client->state = 0;

        client->id = 0;
        if (m_clients.size()) {
            client->id = m_clients.last()->id + 1;
        }

        connect(newSocket, SIGNAL(disconnected()), &m_disconnectMapper, SLOT(map()));
        m_disconnectMapper.setMapping(newSocket, client->id);
        connect(&m_disconnectMapper, SIGNAL(mapped(int)), this, SLOT(socketClosed(int)));

        connect(newSocket, SIGNAL(readyRead()), &m_dataReadyMapper, SLOT(map()));
        m_dataReadyMapper.setMapping(newSocket, client->id);
        connect(&m_dataReadyMapper, SIGNAL(mapped(int)), this, SLOT(socketDataReady(int)));

        m_clients.append(client);
        logString("New client");
    }
}

void MainWindow::socketDataReady(int id)
{
    foreach (Client *client, m_clients) {
        if (client->id == id) {
            switch (client->state) {
            case 0:
                qWarning()  << client->socket->bytesAvailable();
                if (client->socket->bytesAvailable() >= 32) {
                    client->socket->write((char*)peer1_0, sizeof(peer1_0));
                    client->state = 1;
                }
                break;
            case 1: {
                qWarning()  << client->socket->bytesAvailable();
                if (client->socket->bytesAvailable() >= 108) {
                    QByteArray data = client->socket->readAll();
                    QString userName = data.mid(0x20, 32);
                    logString(tr("Got data %1 bytes").arg(data.size()));
                    logString(tr("User %1").arg(userName));
                    logString("Hash:");
                    logString(data.mid(0x40, 8).toHex());
                    logString(data.mid(0x40 + 8, 8).toHex());
                    logString(data.mid(0x40 + 16, 8).toHex());
                    logString(data.mid(0x40 + 24, 8).toHex());
                    logString(data.mid(0x40 + 32, 8).toHex());
                    logString(data.mid(0x40 + 40, 8).toHex());
                    logString(data.mid(0x40 + 48, 8).toHex());
                    logString(data.mid(0x40 + 56, 8).toHex());
                    client->socket->write((char*)peer1_0, sizeof(peer1_1));
                    client->state++;
                    client->socket->close();
                }
            } break;
            default:
                client->socket->close();
                break;
            }
            break;
        }
    }
}

void MainWindow::socketClosed(int id)
{
    // FIXME delete connection
}

void MainWindow::logString(QString str)
{
    ui->textBrowser->append(str);
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
    qDebug() << str;
}

MainWindow::~MainWindow()
{
    delete ui;
}
