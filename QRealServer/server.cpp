#include "server.h"

Server::Server(QObject *parent) : QObject(parent) {
    QFile::remove("msg.log");
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
        qDebug() << "Connection established with socket" << mCounter;
        qDebug() << "Description: " << socket->localAddress().toString() << socket->localPort() << " " << socket->peerAddress().toString() << socket->peerPort();

        SocketWrapper *wrapper = new SocketWrapper(mCounter, socket);
        mCounter++;

        mSockets.push_back(wrapper);

        connect(wrapper, SIGNAL(socketReady(SocketWrapper&)), this, SLOT(onPendingMeaasge(SocketWrapper&)));
        connect(wrapper, SIGNAL(socketClosed(SocketWrapper&)), this, SLOT(onDisconnected(SocketWrapper&)));

//        file = new QFile("msg.log");
//        file->open(QIODevice::ReadOnly | QIODevice::Text);

//        wrapper->socket->write(file->readAll().constData());
//        wrapper->socket->flush();
    }
}

//void Server::processLockRequest(int id) {

//}

void Server::onPendingMeaasge(SocketWrapper &wrapper) {
    QByteArray message = wrapper.getFirstMessage();
    printf(">> %i: %s\n", wrapper.id, message.constData());
    //TODO: should not be oppened for every message
    file = new QFile("msg.log");
    file->open(QIODevice::Append | QIODevice::Text);
    outStream = new QTextStream(file);
    *outStream << message.constData();
    file->flush();
    file->close();
    foreach (SocketWrapper *anotherWrapper, mSockets) {
        if(anotherWrapper->id != wrapper.id) {
            anotherWrapper->socket->write(message.constData());
            anotherWrapper->socket->flush();
            printf("Sending to %i\n", anotherWrapper->id);
        }
    }



//    QTcpSocket *socket = findWrapperById(id)->socket;
//    do {
//        QByteArray ba = socket->readLine();
//        //TODO записать в файл
//        if(ba.startsWith("elemLocked")) {
//            /// lock request





//        } else {
//            printf(">> %i: %s\n", id, ba.constData());
//            foreach (SocketWrapper *wrapper, mSockets) {
//                if((int)wrapper->id != id) {
//                    wrapper->socket->write(ba.constData());
//                    wrapper->socket->flush();
//                    printf("Sending to %i\n", wrapper->id);
//                }
//            }
//        }
//        fflush(stdout);
//    } while(socket->canReadLine());
}

void Server::onDisconnected(SocketWrapper &wrapper) {
    qDebug() << "Socket" << wrapper.id << "Disconnected";
    disconnect(&wrapper, SIGNAL(socketReady(SocketWrapper&)));
    disconnect(&wrapper, SIGNAL(socketClosed(SocketWrapper&)));
    wrapper.socket->deleteLater();
    mSockets.erase(std::remove(mSockets.begin(), mSockets.end(), &wrapper), mSockets.end());
}
