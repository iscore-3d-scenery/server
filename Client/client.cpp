#include "client.h"

client::client(QObject *parent) : QObject(parent)
{
    _C = new QTcpSocket(this);
}

client::~client()
{
    _C->close();
}

void client::start()
{
    _window = new QWidget();
    _window->setFixedSize(250, 200);

    _hostLineEdit = new QLineEdit(_window);
    _hostLineEdit->setGeometry(100, 0, 100, 30);
    QLabel* hostLabel = new QLabel(tr("Host:"), _window);
    hostLabel->setGeometry(0, 0, 100, 30);

    _portLineEdit = new QLineEdit(_window);
    _portLineEdit->setGeometry(100, 40, 100, 30);
    QLabel* portLabel = new QLabel(tr("Server port:"), _window);
    portLabel->setGeometry(0, 40, 100, 30);

    _messageLineEdit = new QLineEdit(_window);
    _messageLineEdit->setGeometry(100, 80, 100, 30);
    QLabel* messageLabel = new QLabel(tr("Message:"), _window);
    messageLabel->setGeometry(0, 80, 100, 30);

    QPushButton *co = new QPushButton(tr("Connect"), _window);
    co->setGeometry(0, 120, 120, 30);
    co->setFont(QFont("Times", 18, QFont::Bold));

    QPushButton *dco = new QPushButton(tr("Disconnect"), _window);
    dco->setGeometry(130, 120, 120, 30);
    dco->setFont(QFont("Times", 18, QFont::Bold));

    QPushButton *send = new QPushButton(tr("Send"), _window);
    send->setGeometry(0, 160, 120, 30);
    send->setFont(QFont("Times", 18, QFont::Bold));

    QPushButton *quit = new QPushButton(tr("Quit"), _window);
    quit->setGeometry(130, 160, 120, 30);
    quit->setFont(QFont("Times", 18, QFont::Bold));

    QObject::connect(co, SIGNAL(clicked()), this, SLOT(connect()));
    QObject::connect(dco, SIGNAL(clicked()), this, SLOT(disconnect()));
    QObject::connect(send, SIGNAL(clicked()), this, SLOT(sendMessage()));
    QObject::connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    _window->show();
}

void client::sendMessage()
{
    if(_C->state() == QTcpSocket::ConnectedState)
    {
        QString string = _messageLineEdit->text();
        qDebug() << "Starting string transfer";
        quint64 bytesSent = _C->write(string.toStdString().c_str(), string.length());
        if(bytesSent == (quint64)-1)
        {
            qDebug() << "Failed to send string";
        }
        else
        {
            qDebug() << bytesSent << "bytes sent";
        }
    }
    else
    {
        qDebug() << "Failed to send string : the socket is not connected.";
    }
}

void client::connect()
{
    if(_C->state() == QTcpSocket::UnconnectedState)
    {
        _address = _hostLineEdit->text();
        _port = _portLineEdit->text().toInt();
        QHostAddress addr(_address);
        _C->connectToHost(addr, _port);

        QObject::connect(_C, SIGNAL(readyRead()), this, SLOT(startRead()));
    }
    else
    {
        qDebug() << "Failed to connect : the socket is already connected, try to disconnect before.";
    }
}

void client::disconnect()
{
    if(_C->state() == QTcpSocket::ConnectedState)
    {
        _C->close();
    }
    else
    {
        qDebug() << "Failed to disconnect : the socket is already disconnected.";
    }
}

void client::startRead()
{
    QByteArray buffer = _C->readAll();
    qDebug() << buffer.data();
}
