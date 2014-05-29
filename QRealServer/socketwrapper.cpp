#include "socketwrapper.h"

SocketWrapper::SocketWrapper(int id, QTcpSocket *socket) {
	this->id = id;
	this->socket = socket;
	
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void SocketWrapper::onReadyRead() {
	emit socketReady(id);
}
void SocketWrapper::onDisconnected() {
	emit socketClosed(id);
}
