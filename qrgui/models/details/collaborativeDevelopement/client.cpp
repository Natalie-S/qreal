#include "client.h"
#include "qabstractitemmodel.h"


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
    /*int t = */msgBox.exec();
}

void Client::onElementAdded(QString parent, QString id,QString logicalId, QString const &name
    , QPointF const &position)
{
    QString buf = "parent: ";
    buf += parent;
    buf += " | id: ";
    buf += id;
    buf += " | logId: ";
    buf += logicalId;
    buf += " | name: ";
    buf += name;
    buf += " | position: ";
    buf += ValueSerializer::serializeQPointF(position);
    QByteArray buffer;
    buffer.append(buf);
    mSocket->write(buffer);
    mSocket->flush();
}

void Client::onDataChanged(QString id, const QVariant &value, int role) {
//    char buffer[32];
//    buffer[0] = 'd';
//    buffer[1] = 'c';
//    buffer[2] = '\n';
//    buffer[3] = '\0';
    QString buf = "id: ";
    buf += id;
    buf += " | value: ";
    QString v = ValueSerializer::serializeQVariant(value);
    buf += v;
    buf += " | role: ";
    buf += role;
    QByteArray buffer;
    buffer.append(buf);
    mSocket->write(buffer);
    qDebug() << "buffer: " << buffer;
    mSocket->flush();
}

void Client::connectToServer(QString addr) {
    QHostAddress hostAddress = QHostAddress(addr);
    //mSocket->connectToHost(QHostAddress::LocalHost, 1234);
    mSocket->connectToHost(hostAddress, 1234);

}

void Client::disconnectFromServer() {
    mSocket->disconnectFromHost();
}
