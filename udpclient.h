#pragma once

#include "sockethandler.h"

class MainWindow;

/**
 * @brief UdpClient class
 * udp协议传输数据
 */
class UdpClient : public SocketHandler
{
	Q_OBJECT
public:
	explicit UdpClient(MainWindow *window);
	~UdpClient() override;

	// 通过 SocketHandler 继承
	Q_SLOT void pingBtnClicked() override;
	Q_SLOT void sendBtnClicked() override;

private:
	MainWindow * const _window;
	QString _ipAddr;
	QString _ipPort;
};
