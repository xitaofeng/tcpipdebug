#pragma once

#include "sockethandler.h"
#include <QObject>

class MainWindow;

/**
 * @brief TcpClient class
 * tcp协议数据传输
 */
class TcpClient : public SocketHandler
{
	Q_OBJECT
public:
	explicit TcpClient(MainWindow *window);
	~TcpClient() override {}

public slots:
	void pingBtnClicked() override;
	void sendBtnClicked() override;

private:
	MainWindow * const _window;
	QString _ipAddr;
	QString _ipPort;
};
