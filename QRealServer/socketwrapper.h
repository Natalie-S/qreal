#pragma once

#include <QtNetwork/QTcpSocket>

class SocketWrapper : public QObject {
	Q_OBJECT

public:
	
    SocketWrapper(int id, QTcpSocket *socket);
	
    int id = 0;
    QTcpSocket *socket;

    QByteArray getFirstMessage();
	
public slots:
	void onReadyRead();
	void onDisconnected();
	
signals:
    void socketReady(SocketWrapper&);
    void socketClosed(SocketWrapper&);

private:
    QByteArray buffer;

    quint16 getFirsMessageSize();
};
