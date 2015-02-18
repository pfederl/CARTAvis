#include "ScriptedCommandListener.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <stdexcept>

ScriptedCommandListener::ScriptedCommandListener(int port, QObject * parent)
    : QObject(parent)
{
    m_tcpServer = new QTcpServer( this);

    /// make sure we know when new connection is established
    connect( m_tcpServer, & QTcpServer::newConnection,
             this, & ScriptedCommandListener::newConnectionCB);

    /// now listen to the port
    if( ! m_tcpServer-> listen( QHostAddress::AnyIPv4, port)) {
        throw std::runtime_error("Coud not listen for scripted commands on given port");
    }
}

void ScriptedCommandListener::newConnectionCB()
{
    qDebug() << "New scripted client connection...";
    if( m_connection != nullptr) {
        qWarning() << "Another client trying to connect? Ignoring...";
        return;
    }
    m_connection = m_tcpServer->nextPendingConnection();
    connect( m_connection, & QTcpSocket::readyRead,
             this, & ScriptedCommandListener::socketDataCB);

}

void ScriptedCommandListener::socketDataCB()
{
    qDebug() << "scripted command listener: socket data ready";
    char buff[ 4096];
    qint64 lineLength = m_connection-> readLine( buff, sizeof(buff));
    if( lineLength == -1) {
        qWarning() << "scripted command listener: something wrong with socket";
        return;
    }

    if( lineLength == 0) {
        qDebug() << "scripted command listener: not a full line yet...";
        return;
    }

    QString str( buff);
    str = str.trimmed();
    emit command( str);
}

QString ScriptedCommandListener::dataTransporter( QString input )
{
    //qDebug() << "(JT) ScriptedCommandListener::dataTransporter()";
    //qDebug() << "input = " << input;
    int inputSize = input.size();
    qDebug() << "(JT) inputSize = " << inputSize;
    input = QString::number(inputSize) + ":" + input;
    qDebug() << "(JT) input (with size) = " << input;
    m_connection->write( input.toLocal8Bit() );
    return input;
}
