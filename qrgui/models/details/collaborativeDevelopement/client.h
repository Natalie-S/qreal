#pragma once
#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <cstdio>
#include <QMessageBox>
#include "models/details/collaborativeDevelopement/valuesSerializer.h"

class ValueSerializer;

class QTcpSocket;

class Client : public QObject
{
    Q_OBJECT
    public:
        explicit Client(QObject *parent = 0);
        void connectToServer(QString addr);
        void disconnectFromServer();
    signals:

    public slots:
        void onConnected();
        void onDataChanged(QString id, const QVariant &value, int role);
        void onElementAdded(QString parent, QString id,QString logicalId, QString const &name
                                     , QPointF const &position);
    private:
        QTcpSocket *mSocket;
};
