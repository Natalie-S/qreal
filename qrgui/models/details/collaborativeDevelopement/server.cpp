#include "server.h"

using namespace qReal;
using namespace models;
using namespace details;
using namespace collaborativeDevelopment;

Server::Server(QObject *parent) :
    QObject(parent)
{
    mServer = new QTcpServer(this);
    connect(mServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    mTmpStorage = "";
    mLastMsg = new QStringList();
}

//Networking
void Server::listen() {
    mServer->listen(QHostAddress::LocalHost, 1234);
}

void Server::close() {
    mServer->close();
}

void Server::onNewConnection() {
    mSocket = mServer->nextPendingConnection();
    if(mSocket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "Smth's happening";
    }

    connect(mSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    emit connectedToClient();
}

void Server::onReadyRead()
{
    if (mSocket->canReadLine()) {
        QByteArray ba = mSocket->readLine();
//        qDebug() << ba.constData();
    }
    QByteArray msg = mSocket->readAll();
//    qDebug() << "onReadyread:  " << msg.constData();
    processSocketMsg(msg);
}

void Server::onDisconnected() {
    qDebug() <<"Connection disconnected.";
    disconnect(mSocket, SIGNAL(disconnected()));
    disconnect(mSocket, SIGNAL(readyRead()));
    mSocket->deleteLater();
}

//The most important function
void Server::processSocketMsg(QByteArray const &msg)
{
    std::vector<QStringList*> paramsVector = divideMsg(msg);
    for(unsigned int i = 0; i < paramsVector.size(); i++) {
        QStringList* params = (QStringList*)paramsVector[i];
        if(params->join("|") != mLastMsg->join("|")) {
//            qDebug() << "params" << params->join("|");
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
        } else {qDebug() << "SAME!";}
    }
}

//Auxiliary methods
void Server::emitAddElem(QStringList* params)
{
    Id const parent = ValuesSerializer::deserializeId(params->at(2));
    qDebug() << "parent" << params->at(2);
    Id const id = ValuesSerializer::deserializeId(params->at(3));
    qDebug() << "id" << params->at(3);
    Id const logicalId = ValuesSerializer::deserializeId(params->at(4));
    qDebug() << "logicalId" << params->at(4);
    QString const name = params->at(5);
    qDebug() << "name" << params->at(5);
    QPointF position = ValuesSerializer::deserializeQPointF(params->at(6));
    qDebug() << "parent" << params->at(6);
    if(params->at(1) == "l") {
        emit logicalModelElementAdded(parent, id, logicalId, name, position);
    } else {
        emit graphicalModelElementAdded(parent, id, logicalId, name, position);
    }
}

void Server::emitSetData(QStringList* params)
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

void Server::emitEdgeAdded(QStringList* params)
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

void Server::emitSignals(QStringList* params)
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
        qDebug() << "id" << params->at(1);
        qDebug() << "graphics" << params->at(2);
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
    }
}

std::vector<QStringList*> Server::divideMsg(QByteArray const &msg)
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
               ) {
           QStringList* list = new QStringList();
           list->append(splittedParams.at(i));
           paramsVector.push_back(list);
       } else {
            size = paramsVector.size() - 1;
            ((QStringList*)paramsVector[size])->append(splittedParams.at(i));
            }
        }
    return paramsVector;
}


