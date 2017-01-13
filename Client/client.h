#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QHostAddress>
#include <QFont>
#include <QPushButton>
#include <QWidget>
#include <QApplication>
#include <QLineEdit>
#include <QLabel>
#include <QThread>

class client : public QObject
{
    Q_OBJECT
public:
    client(QObject *parent = 0);
    ~client();

    void start();
signals:

public slots:
    void sendMessage();
    void connect();
    void disconnect();
    void startRead();

private:
    QTcpSocket* _C;
    QString _address;
    quint16 _port;

    QWidget* _window;
    QLineEdit* _portLineEdit;
    QLineEdit* _hostLineEdit;
    QLineEdit* _messageLineEdit;
};

#endif // CLIENT_H
