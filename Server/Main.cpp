#include "Server.h"
#include <QCoreApplication>
#include <QNetworkInterface>

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    QString IPs = "Available IPs";
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.toString().startsWith("192.168."))
             IPs += " (" + address.toString() + ")";
    }
    qDebug() << LOG_TIME << qPrintable(IPs);

    Server::getInstance()->start(LISTEN_PORT);
    return a.exec();
}
