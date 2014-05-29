#pragma once

#include <QtNetwork/QTcpSocket>

class SocketWrapper : public QObject {
	Q_OBJECT

public:
	
    SocketWrapper(int id, QTcpSocket *socket);
	
    int id = 0;
    QTcpSocket *socket;
	
public slots:
	void onReadyRead();
	void onDisconnected();
	
signals:
	void socketReady(int);
	void socketClosed(int);
};
