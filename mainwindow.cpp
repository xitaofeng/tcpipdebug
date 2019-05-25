#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpclient.h"
#include "udpclient.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, _ui(new Ui::MainWindow{})
	, _cfg(new QVariantMap{})
	, _handler(nullptr)
	, _tcpGrp(new QButtonGroup{})
{
	_ui->setupUi(this);
	QString err{};
	if (!initilize(&err)) {
		QMessageBox::critical(this, "系统错误", err, "确认");
		exit(1);
	}
	setFixedSize(size());
	connect(_tcpGrp,
		QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled),
		this,
		&MainWindow::handleChanged);
	_ui->tcpRadioBtn->setChecked(true);
	connect(_ui->clearReceBtn, &QPushButton::clicked, _ui->receMsg, &QTextEdit::clear);
	connect(_ui->clearSendBtn, &QPushButton::clicked, _ui->sendMsg, &QTextEdit::clear);
}

MainWindow::~MainWindow()
{
	delete _cfg;
	delete _ui;
}

/**
 * @brief MainWindow::initilize
 * 初始化系统中的相关配置内容；
 * @param msg QString 出错时的返回消息
 * @return bool
 */
bool MainWindow::initilize(QString *msg)
{
	setWindowFlags(windowFlags() & ~(Qt::WindowMaximizeButtonHint));

	QFile fp{ QString("%1/config.json").arg(QCoreApplication::applicationDirPath()) };
	if (!fp.open(QFile::ReadOnly)) {
		*msg = "配置文件打开错误";
		return false;
	}
	QJsonParseError err{};
	auto json = QJsonDocument::fromJson(fp.readAll(), &err);
	if (err.error != QJsonParseError::NoError) {
		*msg = "配置文件解析错误";
		return false;
	}
	*_cfg = json.toVariant().toMap();
	_ui->ipAddrText->setText((*_cfg)["tcp"].toMap()["addr"].toString());
	_ui->ipPortText->setText((*_cfg)["tcp"].toMap()["port"].toString());
	_tcpGrp->addButton(_ui->tcpRadioBtn);
	_tcpGrp->addButton(_ui->udpRadioBtn);
	return true;
}

void MainWindow::handleChanged(QAbstractButton *btn, bool checked)
{
	if (!checked) {
		return;
	}
	disableMsg();
	delete _handler;
	if (btn->text() == "TCP") {
		_handler = new TcpClient{ this };
	}
	else if (btn->text() == "UDP")
	{
		_handler = new UdpClient{ this };
	}
	else {
		// 默认作为tcpclient使用
		_handler = new TcpClient{ this };
	}
	enableMsg();
	return;
}

/**
 * @brief MainWindow::disableMsg
 * 重置相关信号；
 */
void MainWindow::disableMsg()
{
	_ui->pingBtn->disconnect();
	_ui->sendBtn->disconnect();
}

/**
 * @brief MainWindow::enableMsg
 * 连接信号，激活按钮相关功能；
 */
void MainWindow::enableMsg()
{
	if (_handler == nullptr) {
		qDebug() << "handler为空";
		return;
	}
	connect(_ui->pingBtn, &QPushButton::clicked, _handler, &SocketHandler::pingBtnClicked);
	connect(_ui->sendBtn, &QPushButton::clicked, _handler, &SocketHandler::sendBtnClicked);
}
