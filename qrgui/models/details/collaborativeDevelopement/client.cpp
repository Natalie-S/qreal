#include "client.h"
#include "qabstractitemmodel.h"

using namespace qReal;
using namespace models;
using namespace details;
using namespace collaborativeDevelopment;

Client::Client(QObject *parent) :
    QObject(parent)
{
    mSocket = new QTcpSocket(this);

    connect(mSocket, SIGNAL(connected()), this, SLOT(onConnected()));
}

///Communication with Server
void Client::onConnected() {
    QMessageBox msgBox;
    msgBox.setText("Connection established.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    emit connectedToServer();
}

void Client::connectToServer(QString addr) {
    qDebug() << "trying to connect to " << addr << " on port " << 1234;
    QHostAddress hostAddress = QHostAddress(addr);
    mSocket->connectToHost(hostAddress, 1234);

}

void Client::sendMsg(QString buf)
{
    qDebug() << buf;
    QByteArray buffer;
    buffer = buf.toAscii();
    mSocket->write(buffer);
    mSocket->flush();
}

void Client::disconnectFromServer() {
    mSocket->disconnectFromHost();
}

///MetaModel Events
void Client::onMetaModelChanged(QString const &params)
{
    sendMsg(params);
}


///Creating diagram
void Client::onDiagramCreated(QString const &name)
{
    sendMsg("diagrCr|" + name + "|");
}

///Adding elements to diaram
void Client::onElementAdded(QString const &modelIdentifier, QString const &parent, QString const &id,QString const &logicalId, QString const &name
    , QPointF const &position)
{
    QString buf = modelIdentifier;
//    qDebug() <<  "modIdent: " << modelIdentifier;
    buf += parent;
//    qDebug() <<  "parent: " << parent;
    buf += "|";    
    buf += id;
//    qDebug() <<  "id: " << id;
    buf += "|";
    buf += logicalId;
//    qDebug() <<  "logId: " << logicalId;
    buf += "|";
    buf += name;
//    qDebug() <<  "name: " << name;
    buf += "|";
    buf += ValuesSerializer::serializeQPointF(position);
//    qDebug() <<  "pos: " << ValuesSerializer::serializeQPointF(position);
    buf += "|";
    sendMsg(buf);
}

void Client::onDataChanged(QString modelIdentifier,QString id, const QVariant &value, int role)
{
    QString buf = modelIdentifier;
//    qDebug() <<  "modIdent: " << modelIdentifier;
    buf += id;
//    qDebug() <<  "id: " << id;
    buf += "|";
    buf += ValuesSerializer::serializeQVariant(value);
//    qDebug() <<  "val" << ValuesSerializer::serializeQVariant(value);
    buf += "|";
    buf += QString::number(role);
//    qDebug() <<  "role: " << role;
    buf += "|";
    sendMsg(buf);
}
