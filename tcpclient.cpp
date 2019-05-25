#include "tcpclient.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QMessageBox>

TcpClient::TcpClient(MainWindow *window) :
	SocketHandler(new QTcpSocket{}),
	_window(window)
{
	_ipAddr = _window->ui()->ipAddrText->text();
	_ipPort = _window->ui()->ipPortText->text();
}

void
TcpClient::pingBtnClicked()
{
	_socket->connectToHost(_ipAddr, _ipPort.toUShort());

	QString msg{ R"(tcp无法连接)" };
	if (_socket->waitForConnected()) {
		msg = R"(tcp可连接)";
	}

	QMessageBox::information(_window, "消息", msg);

	_socket->close();
}

void
TcpClient::sendBtnClicked()
{
	auto cnt = _window->ui()->sendMsg->toPlainText();
	if (cnt.isEmpty()) {
		return;
	}

	_socket->connectToHost(_ipAddr, _ipPort.toUShort());

	auto conn = new QMetaObject::Connection{};
	*conn = connect(_socket, &QTcpSocket::connected, [=]() {
		_socket->write(_window->ui()->sendMsg->toPlainText().toStdString().c_str());
		_socket->disconnect(*conn);
	});

	if (_socket->waitForReadyRead()) {
		_window->ui()->receMsg->append(QString{ _socket->readAll() });
	}
	_socket->close();
}
