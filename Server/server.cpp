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
        char info[100];
        sprintf(info, "Server : Host(%s) / Port(%d)", _address.toString().toStdString().c_str(), _port);
        QLabel* infoL = new QLabel(tr(info));
        _layout->addWidget(infoL, 0, 0, 1, 2);
        _q = new QPushButton("Quit");
        _layout->addWidget(_q, 1, 0, 1, 2);
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

    QPushButton* button = new QPushButton("Receiver");
    button->setStyleSheet("background-color: #94bece;");
    _clientButton.push_back(button);
    char str[20];
    sprintf(str, "Client n°%d", _clientSocketList.size());
    QLineEdit* name = new QLineEdit(str);
    _clientName.push_back(name);

    qDebug() << "New connection";

    client->write("1", 1);

    connect(button, SIGNAL(clicked()), this, SLOT(changeClientState()));
    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnection()));
    setWindow();
    qDebug() << _clientButton.size() << " | " << _clientName.size() << " | " << _clientStateList.size() << " | " << _clientSocketList.size();
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
    delete _clientButton.takeAt(label);
    delete _clientName.takeAt(label);
    _clientStateList.takeAt(label);
    _clientSocketList.takeAt(label);
    delete _layout->takeAt(3 + label * 2);
    delete _layout->takeAt(3 + label * 2 + 1);
    client->close();
    setWindow();
    qDebug() << _clientButton.size() << " | " << _clientName.size() << " | " << _clientStateList.size() << " | " << _clientSocketList.size();
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
    while((item = _layout->takeAt(3)))
    {
    }

    for(int i = 0; i < _clientSocketList.size(); ++i)
    {
        //QPushButton* b = new QPushButton((_clientStateList[i] == 0) ? "Receiver" : "Sender");
        //b->setStyleSheet((_clientStateList[i] == 0) ? "background-color: #94bece;" : "background-color: #0199d9;");
        char str[20];
        sprintf(str, "Client n°%d", i);
        //QLabel* l = new QLabel(tr(str));

        _layout->addWidget(_clientButton[i], i+2, 0);
        _layout->addWidget(_clientName[i], i+2, 1);
    }

    _window->setLayout(_layout);
    _window->show();
}

void server::changeClientState()
{
    QPushButton* button = qobject_cast<QPushButton*>(QObject::sender());
    int label =_clientButton.indexOf(button);

    _clientStateList[label] = 1 - _clientStateList[label];
    _clientButton[label]->setText((_clientStateList[label] == 0) ? "Receiver" : "Sender");
    _clientButton[label]->setStyleSheet((_clientStateList[label] == 0) ? "background-color: #94bece;" : "background-color: #0199d9;");
    setWindow();
    qDebug() << _clientButton.size() << " | " << _clientName.size() << " | " << _clientStateList.size() << " | " << _clientSocketList.size();
}

