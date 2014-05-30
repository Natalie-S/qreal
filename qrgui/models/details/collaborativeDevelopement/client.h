#pragma once

#include <QtCore/QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <cstdio>

#include "models/details/collaborativeDevelopement/valuesSerializer.h"

class QTcpServer;

class QTcpSocket;

namespace qReal {
namespace models {
namespace details {
namespace collaborativeDevelopment {

class Client : public QObject
{
    Q_OBJECT
    public:
        explicit Client(QObject *parent = 0);
        void connectToServer(QString addr);
//        void disconnectFromServer();
        //ex-server
//        void listen();
//        void close();
    signals:
        void connectedToServer();
        //ex-server
        void elemStateChanged(QString const &userName, Id const &id, bool state);
        void logicalModelElementAdded(const Id &parent, const Id &id, const Id &logicalId
                                      , QString const &name, const QPointF &position);
        void logicalModelChanged(Id const &id, QVariant const &value, int role);
        void graphicalModelElementAdded(const Id &parent, const Id &id, const Id &logicalId
                                      , QString const &name, const QPointF &position);
        void graphicalModelChanged(Id const &id, QVariant const &value, int role);

        void diagramCreated(QString const &name, Id const &editor, Id const &diagram, Id const nodeId, Id const containerLink);

//        void connectedToClient();

        void propDeleted(QString const &propDisplayedName);
        void propUpdated(Id const &id
                         , QString const &property
                         , QString const &propertyType
                         , QString const &propertyDefaultValue
                         , QString const &propertyDisplayedName
                         );

        void shapeUpdated(Id const &id, QString const &graphics);
        void nodeAdded(Id const &diagram, QString const &name, bool isRootDiagramNode, Id const &nodeId);
        void edgeAdded(
                Id const &diagram
                , QString const &name
                , QString const &labelText
                , QString const &labelType
                , QString const &lineType
                , QString const &beginType
                , QString const &endType
                , Id edgeId, Id associationId
                );
        void propAdded(Id const &id, QString const &propDisplayedName);
        void elementDeleted(Id const &id);
        void graphElemRemoved(Id const &graphiclId);
        void logElemRemoved(Id const &logId);
        void receivedChatMsg(QString userName, QString msg);

    public slots:
        void onElementBlocked(QString const &userName, Id const &id, bool state);
        void onMetaModelChanged(QString const &params);
        void onConnected();
        void onDataChanged(QString modelIdentifier, QString id, const QVariant &value, int role);
        void onElementAdded(const QString &modelIdentifier, const QString &parent, const QString &id, const QString &logicalId, QString const &name
                                     , QPointF const &position);
        //ex-server
//        void onNewConnection();
        void onReadyRead();
        void onDisconnected();
    private:
        //ex-server
        void processSocketMsg(QByteArray const &msg);
        void emitAddElem(QStringList* params);
        void emitSetData(QStringList* params);
        void emitEdgeAdded(QStringList* params);
        std::vector<QStringList*> divideMsg(QByteArray const &msg);
        void emitSignals(QStringList* params);
        void sendMsg(QString buf);
//        QTcpServer *mServer;
        QTcpSocket *mSocket;
        QString mTmpStorage;
        QStringList* mLastMsg;
//        QTextStream *mOutStream;
//        QFile *mLogFile;

        quint16 getFirsMessageSize();

        QByteArray mBuffer;
};

}
}
}
}
