#pragma once

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <qglobal.h>
#include <cstdio>

#include "socketwrapper.h"

class QTcpServer;

class QTcpSocket;

class Server : public QObject {
	Q_OBJECT
	
public:
	explicit Server(QObject *parent = 0);
	void listen();

public slots:
	
	void onNewConnection();
    void onPendingMeaasge(SocketWrapper &wrapper);
    void onDisconnected(SocketWrapper &wrapper);
	
signals:
	
private:

    const quint16 PORT = 1234;

	QTcpServer *mServer;
	std::vector<SocketWrapper*> mSockets;
	
    int mCounter = 0;
	
	SocketWrapper *findWrapperById(int id);
	
    QMap<int, QString> mLocks;
	//TODO перенести сюда блокировки
};
