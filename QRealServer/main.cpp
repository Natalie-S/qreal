#include <QCoreApplication>
#include <QDebug>
#include <cstdio>

#include "server.h"

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);
    qDebug() << "Server started";
    Server *server = new Server();
    server->listen();
    return a.exec();
}
