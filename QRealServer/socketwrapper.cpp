#include "socketwrapper.h"

SocketWrapper::SocketWrapper(int id, QTcpSocket *socket) {
	this->id = id;
	this->socket = socket;
	
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

quint16 SocketWrapper::getFirsMessageSize() {
    if(buffer.size() >= 2) {
        return (buffer.at(0) - 1) * 127 + (buffer.at(1) - 1) + 2;
    } else {
        return 0;
    }
}

QByteArray SocketWrapper::getFirstMessage() {
    quint16 firstMessageSize = getFirsMessageSize();
    Q_ASSERT(buffer.size() >= firstMessageSize);
    QByteArray result = buffer.left(firstMessageSize);
    buffer = buffer.remove(0, firstMessageSize);
    return result;
}

void SocketWrapper::onReadyRead() {
    QByteArray input = socket->readAll();
    buffer.append(input);
    quint16 firstMessageSize = getFirsMessageSize();
    while (buffer.size() >= 2 && buffer.size() >= firstMessageSize) {
        emit socketReady(*this);
        firstMessageSize = getFirsMessageSize();
    }
}

void SocketWrapper::onDisconnected() {
    emit socketClosed(*this);
}
