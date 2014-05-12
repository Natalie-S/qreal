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
    mTmpStorage = new QStringList();
//    qDebug() << "tmp   " << mTmpStorage->join("|");
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
        qDebug() << "Smth's happening";
    }

    connect(mSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    emit connectedToClient();
}

void Server::onReadyRead() {
    if (mSocket->canReadLine()) {
        QByteArray ba = mSocket->readLine();
//        qDebug() << ba.constData();
    }
    QByteArray gf = mSocket->readAll();
//    qDebug() << gf.constData();
    parseSocketMsg(gf);
}

void Server::emitAddElem(QStringList* params)
{
    Id const parent = ValuesSerializer::deserializeId(params->at(2));
    Id const id = ValuesSerializer::deserializeId(params->at(3));
    Id const logicalId = ValuesSerializer::deserializeId(params->at(4));
    QString const name = params->at(5);
    QPointF position = ValuesSerializer::deserializeQPointF(params->at(6));
    if(params->at(1) == "g") {
        emit logicalModelElementAdded(parent, id, logicalId, name, position);
    } else {
        emit graphicalModelElementAdded(parent, id, logicalId, name, position);
    }
}

void Server::parseSocketMsg(QByteArray const &msg)
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
               ) {
           QStringList* list = new QStringList();
           list->append(splittedParams.at(i));
           paramsVector.push_back(list);
       } else {
            size = paramsVector.size() - 1;
            ((QStringList*)paramsVector[size])->append(splittedParams.at(i));
            }
        }
        qDebug() << "tmp 0  " << mTmpStorage->join("|");

        for(unsigned int i = 0; i < paramsVector.size(); i++) {
            QStringList* params = (QStringList*)paramsVector[i];
            qDebug() << params->join("|");
            qDebug() << "tmp 1  " << mTmpStorage->join("|");
            if((!mTmpStorage->isEmpty()) && params->at(0) == "addElem" && params->at(1) == "g")
            {
                qDebug() << "tmp   " << mTmpStorage->join("|");
                emitAddElem(params);
                mTmpStorage->clear();
                qDebug() << params->join("|");
            } else {
//                if(!mTmpStorage->isEmpty())
//                {
//                    qDebug() << "tmp 2   " << mTmpStorage->join("|");
//                    qDebug() << mTmpStorage->at(2);
//                    emitAddElem(mTmpStorage);
//                    mTmpStorage->clear();
//                    qDebug() << params->join("|");
//                } else {
                    if(params->at(0) == "setData")
                    {
                        Id const id = ValuesSerializer::deserializeId(params->at(2));
                        QVariant const value = ValuesSerializer::deserializeQVariant("QString", params->at(3));
                        int const role = params->at(4).toInt();
                        if (params->at(1) == "l") {          
                            emit logicalModelChanged(id, value, role);
                        } else if (params->at(1) == "g"){                            
                            emit graphicalModelChanged(id, value, role);
                        }
                    } else if (params->at(0) == "addElem")
                    {
                        if (params->at(1) == "l") {
                            if(i == paramsVector.size() - 1)
                            {
                                emitAddElem(params);
                            } else {
                                mTmpStorage = params;
                                qDebug() << "tmp !!!  " << mTmpStorage->join("|");
                            }
                        }
                    } else if (params->at(0) == "delProp")
                    {
                        qDebug() << "prop Deleted!!!";
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
                        emit edgeAdded(ValuesSerializer::deserializeId(params->at(1)), params->at(2), params->at(3)
                                       , params->at(4), params->at(5), params->at(6), params->at(7)
                                       , ValuesSerializer::deserializeId(params->at(8)),ValuesSerializer::deserializeId(params->at(9)));

                    } else if (params->at(0) == "addProp")
                    {
                        emit propAdded(ValuesSerializer::deserializeId(params->at(1)), params->at(2));

                    } else if (params->at(0) == "delElem")
                    {
                        emit elementDeleted(ValuesSerializer::deserializeId(params->at(1)));

                    } else if (params->at(0) == "diagrCr")
                    {
                        emit diagramCreated(params->at(1));
                    }
//                }
            }
        }
}

void Server::onDisconnected() {
    qDebug() <<"Connection disconnected.";
    disconnect(mSocket, SIGNAL(disconnected()));
    disconnect(mSocket, SIGNAL(readyRead()));
    mSocket->deleteLater();
}

