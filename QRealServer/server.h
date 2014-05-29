#pragma once

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <cstdio>

#include "common.h"
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
	void onReadyRead(int id);
	void onDisconnected(int id);
	
signals:
	
private:
	QTcpServer *mServer;
	std::vector<SocketWrapper*> mSockets;
	
	int counter = 0;
	
	SocketWrapper *findWrapperById(int id);
	
	//TODO перенести сюда блокировки
};
