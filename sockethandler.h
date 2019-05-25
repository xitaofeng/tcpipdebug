#pragma once

#include <QObject>
#include <QAbstractSocket>

class SocketHandler : public QObject {
	Q_OBJECT

public:
	SocketHandler(QAbstractSocket *socket, QObject *parent = nullptr)
		: QObject(parent)
		, _socket(socket)
	{}

	SocketHandler(const SocketHandler &socket) = default;

	SocketHandler(SocketHandler &&socket)
	{
		_socket = socket._socket;
		socket._socket = nullptr;
	}

	virtual ~SocketHandler() { delete _socket; }

	virtual void pingBtnClicked() = 0;
	virtual void sendBtnClicked() = 0;

protected:
	QAbstractSocket *_socket;
};
