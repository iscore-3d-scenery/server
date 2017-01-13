#include "server.h"

server::server(QObject *parent) : QObject(parent)
{
    _S = new QTcpServer(this);
    _port = 3594;
    _address = QHostAddress("127.0.0.1");

    if(!_S->listen(_address, _port))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started";
        connect(_S, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
        _window = new QWidget();
        _layout = new QGridLayout();
        _q = new QPushButton("Quit");
        setWindow();
        QObject::connect(_q, SIGNAL(clicked()), qApp, SLOT(quit()));
    }
}

server::~server()
{
    _S->close();
}

void server::acceptConnection()
{
    QTcpSocket* client = _S->nextPendingConnection();
    _clientSocketList.push_back(client);
    _clientStateList.push_back(0);

    qDebug() << "New connection";

    client->write("1", 1);

    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnection()));
    setWindow();
}

void server::startRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    if(!client)
    {
        qDebug() << "Something went wrong : no client";
        return;
    }
    QByteArray buffer = client->readAll();

    qDebug() << buffer.data();

    transferString(client, QString(buffer.data()));
}

void server::clientDisconnection()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    qDebug() << "Connection ended.";
    int label =_clientSocketList.indexOf(client);
    _clientSocketList.removeAt(label);
    _clientSocketList.removeOne(client);
    client->close();
    setWindow();
}

void server::transferString(QTcpSocket* client, QString string)
{
    int label = _clientSocketList.indexOf(client);
    if(_clientStateList[label] == 0)
    {
        return;
    }

    for(int i = 0; i < _clientSocketList.size(); ++i)
    {
        if(_clientStateList[i] == 0)
        {
            quint64 bytesSent = _clientSocketList[i]->write(string.toStdString().c_str(), string.length());
            if(bytesSent == (quint64)-1)
            {
                qDebug() << "Failed to send string";
            }
            else
            {
                //qDebug() << bytesSent << "bytes sent";
            }
        }
    }
}

void server::setWindow()
{
    QLayoutItem *item;
    while((item = _layout->takeAt(0)))
    {
        delete item;
    }
    delete _layout;
    _layout = new QGridLayout();


    char info[100];
    sprintf(info, "Server : Host(%s) / Port(%d)", _address.toString().toStdString().c_str(), _port);
    QLabel* infoL = new QLabel(tr(info));
    _layout->addWidget(infoL, 0, 0, 1, 2);

    for(int i = 0; i < _clientSocketList.size(); ++i)
    {
        QPushButton* b = new QPushButton((_clientStateList[i] == 0) ? "Receiver" : "Sender");
        b->setStyleSheet((_clientStateList[i] == 0) ? "background-color: #94bece;" : "background-color: #0199d9;");
        char str[20];
        sprintf(str, "Client n°%d", i);
        QLabel* l = new QLabel(tr(str));

        _layout->addWidget(b, i+1, 0);
        QObject::connect(b, SIGNAL(clicked()), this, SLOT(changeClientState()));
        _layout->addWidget(l, i+1, 1);
    }

    _layout->addWidget(_q, _clientSocketList.size()+1, 0, 1, 2);


    _window->setLayout(_layout);
    _window->show();
}

void server::changeClientState()
{
    int label = 0;
    QPushButton* button = qobject_cast<QPushButton*>(QObject::sender());
    _layout->removeWidget(button);
    for(int i = 1; i < _layout->count(); i = i+2)
    {
        const char * name = _layout->itemAt(i)->widget()->metaObject()->className();
        if(!strcmp(name, "QLabel"))
        {
            label = (i-1) / 2;
            break;
        }
    }
    _clientStateList[label] = 1 - _clientStateList[label];
    setWindow();
}

