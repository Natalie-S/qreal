#pragma once
#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <cstdio>
#include <QMessageBox>
#include "models/details/collaborativeDevelopement/valuesSerializer.h"

//class ValueSerializer;

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
        void disconnectFromServer();
    signals:
        void connectedToServer();

    public slots:
        void onMetaModelChanged(QString const &params);
        void onConnected();
        void onDataChanged(QString modelIdentifier, QString id, const QVariant &value, int role);
        void onElementAdded(const QString &modelIdentifier, const QString &parent, const QString &id, const QString &logicalId, QString const &name
                                     , QPointF const &position);
        void onDiagramCreated(QString const &name);
//        void onPluginsLoaded();
//        void onNodeAdded(QString const &diagram, QString const &name, bool isDiagramNode);
//        void onEdgeAdded(
//                QString const &diagram
//                , QString const &name
//                , QString const &labelText
//                , QString const &labelType
//                , QString const &lineType
//                , QString const &beginType
//                , QString const &endType
//                );
    private:
        QTcpSocket *mSocket;
        void sendMsg(QString buf);
};

}
}
}
}
