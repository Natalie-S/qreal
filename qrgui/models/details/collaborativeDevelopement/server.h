#pragma once
#include <QObject>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <cstdio>
#include <QMessageBox>
#include <QtNetwork/QHostAddress>
#include <QDebug>
#include "models/details/collaborativeDevelopement/valuesSerializer.h"

class QTcpServer;

class QTcpSocket;

namespace qReal {
namespace models {
namespace details {
namespace collaborativeDevelopment {

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void listen();
    void close();

signals:
    void elemStateChanged(QString const &userName, Id const &id, bool state);
    void logicalModelElementAdded(const Id &parent, const Id &id, const Id &logicalId
                                  , QString const &name, const QPointF &position);
    void logicalModelChanged(Id const &id, QVariant const &value, int role);
    void graphicalModelElementAdded(const Id &parent, const Id &id, const Id &logicalId
                                  , QString const &name, const QPointF &position);
    void graphicalModelChanged(Id const &id, QVariant const &value, int role);

    void diagramCreated(QString const &name, Id const &editor, Id const &diagram, Id const nodeId, Id const containerLink);

    void connectedToClient();

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


public slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
private:
    void processSocketMsg(QByteArray const &msg);
    void emitAddElem(QStringList* params);
    void emitSetData(QStringList* params);
    void emitEdgeAdded(QStringList* params);
    std::vector<QStringList*> divideMsg(QByteArray const &msg);
    void emitSignals(QStringList* params);
    QTcpServer *mServer;
    QTcpSocket *mSocket;
    QStringList* mTmpStorage;
};

}
}
}
}
