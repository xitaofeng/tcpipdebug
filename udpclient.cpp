#include "udpclient.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QUdpSocket>

UdpClient::UdpClient(MainWindow *window)
	: SocketHandler{ new QUdpSocket{} }
	, _window{ window } {
	_ipAddr = _window->ui()->ipAddrText->text();
	_ipPort = _window->ui()->ipPortText->text();
}

UdpClient::~UdpClient() {}

void
UdpClient::pingBtnClicked() {
	_socket->connectToHost(_ipAddr, _ipPort.toUShort());

	QString msg{ R"(udp不可连接)" };
	if (_socket->waitForConnected()) {
		msg = R"(udp可连接)";
	}

	QMessageBox::information(_window, "消息", msg);

	_socket->close();
}

void
UdpClient::sendBtnClicked() {
	auto cnt = _window->ui()->sendMsg->toPlainText();
	if (cnt.isEmpty()) {
		return;
	}

	_socket->connectToHost(_ipAddr, _ipPort.toUShort());

	auto conn = new QMetaObject::Connection{};
	*conn = connect(_socket, &QUdpSocket::connected, [=]() {
		_socket->write(cnt.toStdString().c_str());
		_socket->disconnect(*conn);
	});

	if (_socket->waitForReadyRead()) {
		_window->ui()->receMsg->append(QString{ _socket->readAll() });
	}
	_socket->close();
}
