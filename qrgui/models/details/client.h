#pragma once
#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <cstdio>
#include <QMessageBox>

class QTcpSocket;

class Client : public QObject
{
    Q_OBJECT
    public:
        explicit Client(QObject *parent = 0);
        void connectToServer();        
        void disconnectFromServer();
    signals:

    public slots:
        void onConnected();
        void onDataChanged();
    private:
        QTcpSocket *mSocket;
};
