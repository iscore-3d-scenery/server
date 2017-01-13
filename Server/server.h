#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QHostAddress>
#include <QApplication>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

class server : public QObject
{
    Q_OBJECT
public:
    server(QObject *parent = 0);
    ~server();

signals:

public slots:
    void acceptConnection();
    void startRead();
    void clientDisconnection();
    void changeClientState();

private:
    void setWindow();
    void transferString(QTcpSocket* client, QString string);

    QWidget* _window;
    QGridLayout* _layout;
    QTcpServer* _S;
    QPushButton* _q;
    QList <QPushButton*> _clientButton;
    quint16 _port;
    QHostAddress _address;
    QList <quint8> _clientStateList;
    QList <QTcpSocket*> _clientSocketList;
};

#endif // SERVER_H
