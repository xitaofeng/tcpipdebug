#pragma once

#include "sockethandler.h"
#include <QButtonGroup>
#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
	class MainWindow;
}

/**
 * @brief The SocketType enum
 * 由RadioButton确定的socket的类型，比如tcp和udp。
 */
enum SocketType {
	TCPClient, TCPServer,
	UDPClient, UDPServer
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	/** 获取MainWindow的ui对象 */
	const Ui::MainWindow* ui() const { return _ui; }

	/** TCP和UDP间的切换 */
	Q_SLOT void handleChanged(QAbstractButton *btn, bool checked);

protected:
	bool initilize(QString *);

	/** 按钮的消息注册 */
	void disableMsg();
	void enableMsg();
	/** 按钮的消息注册 */

private:
	Ui::MainWindow *_ui;
	QVariantMap *_cfg;
	SocketHandler *_handler;
	QButtonGroup *_tcpGrp;
};
