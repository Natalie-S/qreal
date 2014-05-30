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

    mTmpStorage = "";
    mLastMsg = new QStringList();
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
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Client::onDisconnected() {
    mSocket->disconnectFromHost();
    qDebug() << "Connection disconnected.";
    disconnect(mSocket, 0, 0, 0);
    mSocket->deleteLater();
}

quint16 Client::getFirsMessageSize() {
    if(mBuffer.size() >= 2) {
        return (mBuffer.at(0) - 1) * 127 + (mBuffer.at(1) - 1) + 2;
    } else {
        return 0;
    }
}

void Client::onReadyRead()
{
    mBuffer.append(mSocket->readAll());
    quint16 firstMessageSize = getFirsMessageSize();
    while (mBuffer.size() >= 2 && mBuffer.size() >= firstMessageSize) {
        processSocketMsg((mBuffer.left(firstMessageSize)).remove(0, 2));
        mBuffer = mBuffer.remove(0, firstMessageSize);
        firstMessageSize = getFirsMessageSize();
    }
//    qDebug() << "Recieved message:" << msg;
//    processSocketMsg(msg);
}

///Creating and sending msgs
void Client::sendMsg(QString buf)
{
    qDebug() << "Sending message:" <<  buf;
    QByteArray localBuffer;
    localBuffer.append((char)(1 + buf.size() / 127));
    localBuffer.append((char)(1 + buf.size() % 127));
    localBuffer.append(buf.toAscii());
    qDebug() << "Message content:" <<  localBuffer;
    mSocket->write(localBuffer);
    mSocket->flush();
}

void Client::onMetaModelChanged(QString const &params)
{
    sendMsg(params);
}

void Client::onElementBlocked(QString const &userName, Id const &id, bool state)
{
    QStringList params;
    params << "elemLocked" << userName << id.toString() << (state ? "t" : "f");
    sendMsg(params.join("|") + "|");
}

void Client::onElementAdded(QString const &modelIdentifier, QString const &parent, QString const &id,QString const &logicalId, QString const &name
    , QPointF const &position)
{
        QStringList params;
        params << modelIdentifier << parent << id << logicalId << name << ValuesSerializer::serializeQPointF(position);
        sendMsg(params.join("|") + "|");
}

void Client::onDataChanged(QString modelIdentifier, QString id, const QVariant &value, int role)
{
        QStringList params;
        params << modelIdentifier << id << ValuesSerializer::serializeQVariant(value) << QString::number(role);
        sendMsg(params.join("|") + "|");
}

///Creating and sending msgs ends

///Processing incoming msgs
void Client::processSocketMsg(QByteArray const &msg)
{
    qDebug() << "Recieved message:" << msg;
    std::vector<QStringList*> paramsVector = divideMsg(msg);
    for(unsigned int i = 0; i < paramsVector.size(); i++) {
        QStringList* params = (QStringList*)paramsVector[i];
        if(params->join("|") != mLastMsg->join("|")) {
//            qDebug() << "params" << params->join("|");
//            *mStream << params->join("|") << "\n\n";
            if(mTmpStorage != "")
            {
                if(params->at(0) == "addElem" && params->at(1) == "g") {
                    params->replace(4, mTmpStorage);
                    emitAddElem(params);
                } else {
                    emitSignals(params);
                }
                mTmpStorage = "";
            } else {
                emitSignals(params);
            }
            mLastMsg = params;
        } /*else {qDebug() << "SAME!";}*/
    }
}

void Client::emitSignals(QStringList* params)
{
    if(params->at(0) == "setData")
    {
        emitSetData(params);
    } else if (params->at(0) == "addElem")
    {
        if(params->at(1) == "l") {
            mTmpStorage = params->at(4);
        }
        emitAddElem(params);

    } else if (params->at(0) == "delProp")
    {
        emit propDeleted(params->at(1));

    } else if (params->at(0) == "updProp")
    {
        emit propUpdated(ValuesSerializer::deserializeId(params->at(1)), params->at(2), params->at(3), params->at(4), params->at(5));

    } else if (params->at(0) == "updShape")
    {
        emit shapeUpdated(ValuesSerializer::deserializeId(params->at(1)), params->at(2));

    } else if (params->at(0) == "addNode")
    {
        emit nodeAdded(ValuesSerializer::deserializeId(params->at(1)), params->at(2), params->at(3) == "t", ValuesSerializer::deserializeId(params->at(4)));

    } else if (params->at(0) == "addEdge")
    {
        emitEdgeAdded(params);

    } else if (params->at(0) == "addProp")
    {
        emit propAdded(ValuesSerializer::deserializeId(params->at(1)), params->at(2));

    } else if (params->at(0) == "delElem")
    {
        emit elementDeleted(ValuesSerializer::deserializeId(params->at(1)));

    } else if (params->at(0) == "diagrCr")
    {
        emit diagramCreated(params->at(1), ValuesSerializer::deserializeId(params->at(2)), ValuesSerializer::deserializeId(params->at(3)),ValuesSerializer::deserializeId(params->at(4)),ValuesSerializer::deserializeId(params->at(5)));

    } else if(params->at(0) == "elemLocked")
    {
        QString userName = params->at(1);
        Id id = ValuesSerializer::deserializeId(params->at(2));
        bool state = params->at(3) == "t";
        emit elemStateChanged(userName, id, state);
    } else if (params->at(0) == "grElemRemoved")
    {
        emit graphElemRemoved(ValuesSerializer::deserializeId(params->at(1)));
    } else if (params->at(0) == "logElemRemoved")
    {
        emit logElemRemoved(ValuesSerializer::deserializeId(params->at(1)));
    } else if (params->at(0) == "chatMsg")
    {
        emit receivedChatMsg(params->at(1), params->at(2));
    }
}

std::vector<QStringList*> Client::divideMsg(QByteArray const &msg)
{
    QString const paramsString = QString::fromAscii(msg);
    QStringList const splittedParams = paramsString.split('|');
    std::vector<QStringList*> paramsVector;
    unsigned int size = 0;
    for(int i = 0; i < splittedParams.length() - 1; i++) {
        if(splittedParams.at(i) == "setData"
                || splittedParams.at(i) == "addElem"
                || splittedParams.at(i) == "delProp"
                || splittedParams.at(i) == "updProp"
                || splittedParams.at(i) == "updShape"
                || splittedParams.at(i) == "addNode"
                || splittedParams.at(i) == "addEdge"
                || splittedParams.at(i) == "addProp"
                || splittedParams.at(i) == "diagrCr"
                || splittedParams.at(i) == "delElem"
                || splittedParams.at(i) == "elemLocked"
                || splittedParams.at(i) == "grElemRemoved"
                || splittedParams.at(i) == "logElemRemoved"
                || splittedParams.at(i) == "chatMsg"
               ) {
           QStringList* list = new QStringList();
           list->append(splittedParams.at(i));
           paramsVector.push_back(list);
       } else {
            size = paramsVector.size() - 1;
            if (size != -1) {
                ((QStringList*)paramsVector[size])->append(splittedParams.at(i));
            } /*else {
                qDebug() << "Wrong params vector";
            }*/
        }
    }
    return paramsVector;
}

void Client::emitAddElem(QStringList* params)
{
    Id const parent = ValuesSerializer::deserializeId(params->at(2));
//    qDebug() << "parent" << params->at(2);
    Id const id = ValuesSerializer::deserializeId(params->at(3));
//    qDebug() << "id" << params->at(3);
    Id const logicalId = ValuesSerializer::deserializeId(params->at(4));
//    qDebug() << "logicalId" << params->at(4);
    QString const name = params->at(5);
//    qDebug() << "name" << params->at(5);
    QPointF position = ValuesSerializer::deserializeQPointF(params->at(6));
//    qDebug() << "parent" << params->at(6);
    if(params->at(1) == "l") {
        emit logicalModelElementAdded(parent, id, logicalId, name, position);
    } else {
        emit graphicalModelElementAdded(parent, id, logicalId, name, position);
    }
}

void Client::emitSetData(QStringList* params)
{
    Id const id = ValuesSerializer::deserializeId(params->at(2));
    QVariant const value = ValuesSerializer::deserializeQVariant("QString", params->at(3));
    int const role = params->at(4).toInt();
    if (params->at(1) == "l") {
        emit logicalModelChanged(id, value, role);
    } else if (params->at(1) == "g"){
        emit graphicalModelChanged(id, value, role);
    }
}

void Client::emitEdgeAdded(QStringList* params)
{
    Id const &diagram = ValuesSerializer::deserializeId(params->at(1));
    QString const &name = params->at(2);
    QString const &labelText = params->at(3);
    QString const &labelType = params->at(4);
    QString const &lineType = params->at(5);
    QString const &beginType = params->at(6);
    QString const &endType = params->at(7);
    Id edgeId = ValuesSerializer::deserializeId(params->at(8));
    Id associationId = ValuesSerializer::deserializeId(params->at(9));
    emit edgeAdded(diagram, name, labelText, labelType, lineType, beginType, endType, edgeId, associationId);
}
