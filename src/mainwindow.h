#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTcpSocket>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	Ui::MainWindow Ui_;
	QTcpSocket Socket_;

	enum LogMode
	{
		System,
		Data
	};
public:
	MainWindow ();
private:
	void Log (QString, LogMode);
private slots:
	void on_Connect__released ();
	void on_Disconnect__released ();
	void on_SendData__released ();

	void handleConnected ();
	void handleDisconnected ();
	void handleError (QAbstractSocket::SocketError);
	void handleBytesWritten (qint64);
	void handleReadyRead ();
};

#endif

