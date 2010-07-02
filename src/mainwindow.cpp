#include "mainwindow.h"
#include <QByteArray>
#include <QDataStream>

MainWindow::MainWindow ()
{
	Ui_.setupUi (this);
	connect (&Socket_,
			SIGNAL (connected ()),
			this,
			SLOT (handleConnected ()));
	connect (&Socket_,
			SIGNAL (disconnected ()),
			this,
			SLOT (handleDisconnected ()));
	connect (&Socket_,
			SIGNAL (error (QAbstractSocket::SocketError)),
			this,
			SLOT (handleError (QAbstractSocket::SocketError)));
	connect (&Socket_,
			SIGNAL (bytesWritten (qint64)),
			this,
			SLOT (handleBytesWritten (qint64)));
	connect (&Socket_,
			SIGNAL (readyRead ()),
			this,
			SLOT (handleReadyRead ()));

	Ui_.Log_->setPlainText ("Hi there!");
}

void MainWindow::on_Connect__released ()
{
	QString host = Ui_.Host_->text ();
	int port = Ui_.Port_->text ().toInt ();

	Log (QString ("Connecting to %1:%2 ...").arg (host).arg (port), System);

	Socket_.connectToHost (host, port);
}

void MainWindow::on_Disconnect__released ()
{
	Log ("Disconnecting...", System);

	Socket_.disconnectFromHost ();
}

void MainWindow::on_SendData__released ()
{
	QStringList strings = Ui_.ToSend_->toPlainText ().split ('\n', QString::SkipEmptyParts);

	QList<QByteArray> bytes;

	Q_FOREACH (const QString& string, strings)
		bytes << string.trimmed ().toLatin1 ();

	quint32 numLists = bytes.size ();

	QByteArray msg;
	{
		QDataStream ds (&msg, QIODevice::WriteOnly);
		ds.setByteOrder (QDataStream::BigEndian);
		ds << numLists;
		Q_FOREACH (const QByteArray& ba, bytes)
			ds << static_cast<quint32> (ba.size ());
		Q_FOREACH (const QByteArray& ba, bytes)
			ds << ba;
	}
	Log ("Sending:", System);
	Log (msg.toHex (), Data);
	Socket_.write (msg);
}

void MainWindow::Log (QString message, LogMode mode)
{
	if (mode == System)
		message.prepend (" >>> ");
	Ui_.Log_->append (message);
}

void MainWindow::handleConnected ()
{
	Log ("Connected", System);
}

void MainWindow::handleDisconnected ()
{
	Log ("Disconnected", System);
}

void MainWindow::handleError (QAbstractSocket::SocketError error)
{
	Log (QString ("Got error %1 (%2)")
			.arg (error)
			.arg (Socket_.errorString ()), System);
}

void MainWindow::handleBytesWritten (qint64 bytes)
{
	Log (QString ("Wrote %1 bytes").arg (bytes), System);
}

void MainWindow::handleReadyRead ()
{
	Log ("Ready read", System);
	QByteArray all = Socket_.readAll ();
	Log ("Hex:", System);
	Log (all.toHex (), Data);
}
