#include "server.h"

Server::Server(QObject *parent) : QObject(parent) {
    mServer = new QTcpServer(this);
    connect(mServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void Server::listen() {
    QHostAddress address;
    address.setAddress(QString("127.0.0.1"));
    address.toIPv4Address();

    mServer->setMaxPendingConnections(8);
    mServer->listen(address, PORT);

    qDebug() << "Listening for" << address.toString() << "address on port:" << PORT;
}

void Server::onNewConnection() {
    qDebug() << "New pending connection";
    QTcpSocket *socket = mServer->nextPendingConnection();
    if(socket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "Connection established with socket" << counter;

        SocketWrapper *wrapper = new SocketWrapper(counter, socket);
        counter++;

        mSockets.push_back(wrapper);

        connect(wrapper, SIGNAL(socketReady(int)), this, SLOT(onReadyRead(int)));
        connect(wrapper, SIGNAL(socketClosed(int)), this, SLOT(onDisconnected(int)));
    }
}

void Server::onReadyRead(int id) {
    QTcpSocket *socket = findWrapperById(id)->socket;
    do {
        QByteArray ba = socket->readLine();
		//TODO записать в файл
        printf(">> %i: %s\n", id, ba.constData());
        foreach (SocketWrapper *wrapper, mSockets) {
            if((int)wrapper->id != id) {
                wrapper->socket->write(ba.constData());
                wrapper->socket->flush();
                printf("Sending to %i\n", wrapper->id);
            }
        }
        fflush(stdout);
    } while(socket->canReadLine());
}

void Server::onDisconnected(int id) {
    SocketWrapper *wrapper = findWrapperById(id);
    qDebug() << "Socket" << id << "Disconnected";
    disconnect(wrapper, SIGNAL(socketReady(int)));
    disconnect(wrapper, SIGNAL(socketClosed(int)));
    wrapper->socket->deleteLater();
    mSockets.erase(std::remove(mSockets.begin(), mSockets.end(), wrapper), mSockets.end());
}

SocketWrapper *Server::findWrapperById(int id) {
    for(SocketWrapper *wrapper : mSockets) {
        if((int)wrapper->id == id) {
            return wrapper;
        }
    }
    Q_ASSERT(false);
    return NULL;
}
