#include "client.h"


Client::Client(QObject *parent) :
    QObject(parent)
{
    mSocket = new QTcpSocket(this);
    connect(mSocket, SIGNAL(connected()), this, SLOT(onConnected()));
}

void Client::onConnected() {
    //printf("Connection established.\n");
    QMessageBox msgBox;
    msgBox.setText("Connection established.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    int t = msgBox.exec();
}

void Client::onDataChanged() {
    char buffer[32];
    buffer[0] = 'd';
    buffer[1] = 'c';
    buffer[2] = '\n';
    buffer[3] = '\0';
//    mSocket->write("Data changed\n");
//    mSocket->write('\0');
    mSocket->write(buffer);
    mSocket->flush();
}

void Client::connectToServer() {
    mSocket->connectToHost(QHostAddress::LocalHost, 1234);
}
