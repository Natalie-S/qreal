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
    qDebug() << gf.constData();
    parseSocketMsg(gf);
}

void Server::parseSocketMsg(QByteArray const &msg)
{
    QString const paramsString = QString::fromAscii(msg);
    QStringList const splittedParams = paramsString.split('|');
//    if (splittedParams.size() == 1)
//    {
//        if(splittedParams.at(0) == "loadPlugins")
//        {
//            emit pluginsWereLoaded();
//        } else {
//        qDebug() << splittedParams.at(0);
//        emit diagramCreated(splittedParams.at(0));
//        }
//    } else {
        std::vector<QStringList*> paramsVector;
        unsigned int size = 0;
        for(int i = 0; i < splittedParams.length() - 1; i++) {
            if(splittedParams.at(i) == "l"
                    || splittedParams.at(i) == "g"
                    || splittedParams.at(i) == "delProp"
                    || splittedParams.at(i) == "updProp"
                    || splittedParams.at(i) == "updShape"
                    || splittedParams.at(i) == "addNode"
                    || splittedParams.at(i) == "addEdge"
                    || splittedParams.at(i) == "addProp"
                    || splittedParams.at(i) == "diagrCr"
                    || splittedParams.at(i) == "loadPlugins"
                    ) {
                QStringList* list = new QStringList();
                list->append(splittedParams.at(i));
                paramsVector.push_back(list);
            } else {
                size = paramsVector.size() - 1;
                ((QStringList*)paramsVector[size])->append(splittedParams.at(i));
            }
        }
        for(unsigned int i = 0; i < paramsVector.size(); i++) {
            QStringList* params = (QStringList*)paramsVector[i];
    //        for (int i = 0; i < params->size(); i++) {
    //            qDebug() << i << "  " << params->at(i);
    //        }
            if(params->at(0) == "l")
            {
                if (params->size() == 4) {
                    qDebug() << "obtained params l set";
                    qDebug() <<  "id" << params->at(1);
                    Id const id = ValuesSerializer::deserializeId(params->at(1));
                    qDebug() <<  "qvar" << params->at(2) << "    " <<  params->at(3);
                    QVariant const value = ValuesSerializer::deserializeQVariant("QString", params->at(2));
                    qDebug() << "role" << params->at(4);
                    int const role = params->at(3).toInt();
                    emit logicalModelChanged(id, value, role);
                } else {
                    qDebug() << "obtained params l add";
                    Id const parent = ValuesSerializer::deserializeId(params->at(1));
                    qDebug() <<  "parent" << params->at(1);
                    Id const id = ValuesSerializer::deserializeId(params->at(2));
                    qDebug() <<  "id" << params->at(2);
                    Id const logicalId = ValuesSerializer::deserializeId(params->at(3));
                    qDebug() <<  "logicalId" << params->at(3);
                    QString const name = params->at(4);
                    qDebug() <<  "name" << params->at(4);
                    QPointF position = ValuesSerializer::deserializeQPointF(params->at(5));
                    qDebug() <<  "position";
                    emit logicalModelElementAdded(parent, id, logicalId, name, position);
                }

            } else if (params->at(0) == "g")
            {
                if (params->size() == 5) {
                    qDebug() << "obtained params g set";
                    qDebug() <<  "id" << params->at(1);
                    Id const id = ValuesSerializer::deserializeId(params->at(1));
                    qDebug() <<  "qvar" << params->at(2) << "    " <<  params->at(3);
                    QVariant const value = ValuesSerializer::deserializeQVariant(params->at(2), params->at(3));
                    qDebug() << "role" << params->at(4);
                    int const role = params->at(4).toInt();
                    emit graphicalModelChanged(id, value, role);
                } else {
                    qDebug() << "obtained params g add";
                    Id const parent = ValuesSerializer::deserializeId(params->at(1));
                    qDebug() <<  "parent" << params->at(1);
                    Id const id = ValuesSerializer::deserializeId(params->at(2));
                    qDebug() <<  "id" << params->at(2);
                    Id const logicalId = ValuesSerializer::deserializeId(params->at(3));
                    qDebug() <<  "logicalId" << params->at(3);
                    QString const name = params->at(4);
                    qDebug() <<  "name" << params->at(4);
                    QPointF position = ValuesSerializer::deserializeQPointF(params->at(5));
                    qDebug() <<  "position";
                    emit graphicalModelElementAdded(parent, id, logicalId, name, position);
                }
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
                emit nodeAdded(ValuesSerializer::deserializeId(params->at(1)), params->at(2), params->at(3) == "t");

            } else if (params->at(0) == "addEdge")
            {
                emit edgeAdded(ValuesSerializer::deserializeId(params->at(1)), params->at(2), params->at(3), params->at(4), params->at(5), params->at(6), params->at(7));

            } else if (params->at(0) == "addProp")
            {
                emit propAdded(ValuesSerializer::deserializeId(params->at(1)), params->at(2));

            } else if (params->at(0) == "loadPlugins")
            {
//                emit pluginsWereLoaded();
            } else if (params->at(0) == "diagrCr")
            {
                emit diagramCreated(params->at(1));
            }

        }
//    }

}

void Server::onDisconnected() {
    qDebug() <<"Connection disconnected.";
    disconnect(mSocket, SIGNAL(disconnected()));
    disconnect(mSocket, SIGNAL(readyRead()));
    mSocket->deleteLater();
}

