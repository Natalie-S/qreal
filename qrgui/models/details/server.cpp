#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
    mServer = new QTcpServer(this);
    connect(mServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}
void Server::listen() {
    mServer->listen(QHostAddress::LocalHost, 1234);
}

void Server::close() {
    mServer->close();
}

void Server::onNewConnection() {
    mSocket = mServer->nextPendingConnection();
    if(mSocket->state() == QTcpSocket::ConnectedState) {
        //printf("New connection established.\n");
        /*QMessageBox msgBox;
        msgBox.setText("New connection established.\n");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.show();*/
        qDebug() << "Smth's happening";
    }

    connect(mSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Server::onReadyRead() {
    while(mSocket->canReadLine()) {
        /*QByteArray ba = mSocket->readLine();
        if(strcmp(ba.constData(), "!exit\n") == 0) {
            mSocket->disconnectFromHost();
            break;
        }
        QMessageBox msgBox;
        //msgBox.setText(">> %s", ba.constData());
        msgBox.setText("DC");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.show();*/

        //printf(">> %s", ba.constData());
        qDebug() << "DC";
    }
}

void Server::onDisconnected() {
    printf("Connection disconnected.\n");
    disconnect(mSocket, SIGNAL(disconnected()));
    disconnect(mSocket, SIGNAL(readyRead()));
    mSocket->deleteLater();
}

