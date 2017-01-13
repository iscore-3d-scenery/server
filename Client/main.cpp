#include <QApplication>
#include <iostream>
#include <QFile>
#include "client.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    client c;
    c.start();

    return a.exec();
}

