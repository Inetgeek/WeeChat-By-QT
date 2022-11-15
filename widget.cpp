/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#include "widget.h"
#include "ui_widget.h"
#include "search.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include "messages.h"
#include "config.h"
#include "frmemailtool.h"

#include<QDebug>
#include<QUdpSocket>
#include<QHostInfo>
#include<QMessageBox>
#include<QScrollBar>
#include<QDateTime>
#include<QNetworkInterface>
#include<QProcess>
#include <QFileDialog>
#include <QColorDialog>
#include <QPropertyAnimation>
#include <QMouseEvent>

/*构造函数*/
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    udpSocket = new QUdpSocket(this);
    port = UDP_SOCKET_PORT ;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    sendMessage(NewParticipant);
    qDebug()<<getIP();
    server = new TcpServer(this);
    mail = new frmEmailTool();
    connect(server, SIGNAL(sendFileName(QString)), this, SLOT(getFileName(QString)));

    connect(ui->messageTextEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),this, SLOT(currentFormatChanged(QTextCharFormat)));

    setWindowFlags(Qt::FramelessWindowHint);//无边框
    setAttribute(Qt::WA_TranslucentBackground);//背景透明
//    ui->page->setWindowOpacity(0.5); //半透明


    ui->sendButton->setFocus();    //设置默认焦点
    ui->sendButton->setShortcut( QKeySequence::InsertParagraphSeparator );  //设置快捷键为键盘的“回车”键
    ui->sendButton->setShortcut(Qt::Key_Enter);  //设置快捷键为enter键
    ui->sendButton->setShortcut(Qt::Key_Return); //设置快捷键为小键盘上的enter键
    ui->sendButton->setFocus();    //设置默认焦点
    ui->sendButton->setDefault(true);  //设置默认按钮，设置了这个属性，当用户按下回车的时候，就会按下该按钮
    ui->messageBrowser->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical{width:8px;background:rgba(0,0,0,0%);margin:0px,0px,0px,0px;padding-top:9px;padding-bottom:9px;}"//留9px给上下箭头
        "QScrollBar::handle:vertical{width:8px;background:rgba(0,0,0,25%);border-radius:4px;min-height:20;}"//上下设置为椭圆
        "QScrollBar::handle:vertical:hover{width:8px;background:rgba(0,0,0,50%);border-radius:4px;min-height:20;}"//鼠标悬浮颜色变深
        "QScrollBar::add-line:vertical{height:9px;width:8px;border-image:url(:/imgs/bottom.png);subcontrol-position:bottom;}"//下箭头
        "QScrollBar::sub-line:vertical{height:9px;width:8px;border-image:url(:/imgs/top.png);subcontrol-position:top;}"//上箭头
        "QScrollBar::add-line:vertical:hover{height:7px;width:6px;border-image:url(:/imgs/bottom.png);border:1px;subcontrol-position:bottom;}"//鼠标悬浮下箭头
        "QScrollBar::sub-line:vertical:hover{height:7px;width:6px;border-image:url(:/imgs/top.png);border:1px;subcontrol-position:top;}"//鼠标悬浮上箭头
        "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:rgba(0,0,0,10%);border-radius:4px;}");//滚动时部分

    ui->messageTextEdit->verticalScrollBar()->setStyleSheet(
    "QScrollBar:vertical{width:8px;background:rgba(0,0,0,0%);margin:0px,0px,0px,0px;padding-top:9px;padding-bottom:9px;}"//留9px给上下箭头
    "QScrollBar::handle:vertical{width:8px;background:rgba(0,0,0,25%);border-radius:4px;min-height:20;}"//上下设置为椭圆
    "QScrollBar::handle:vertical:hover{width:8px;background:rgba(0,0,0,50%);border-radius:4px;min-height:20;}"//鼠标悬浮颜色变深
    "QScrollBar::add-line:vertical{height:9px;width:8px;border-image:url(:/imgs/bottom.png);subcontrol-position:bottom;}"//下箭头
    "QScrollBar::sub-line:vertical{height:9px;width:8px;border-image:url(:/imgs/top.png);subcontrol-position:top;}"//上箭头
    "QScrollBar::add-line:vertical:hover{height:7px;width:6px;border-image:url(:/imgs/bottom.png);border:1px;subcontrol-position:bottom;}"//鼠标悬浮下箭头
    "QScrollBar::sub-line:vertical:hover{height:7px;width:6px;border-image:url(:/imgs/top.png);border:1px;subcontrol-position:top;}"//鼠标悬浮上箭头
    "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:rgba(0,0,0,10%);border-radius:4px;}");//滚动时部分

    ui->userTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格只读
    ui->userTableWidget->setItemDelegate(new NoFocusDelegate());//去掉表格虚框
    ui->userTableWidget->setTextElideMode(Qt::ElideNone);//防止文本过长显示右边

    /* 显示菜单 */
    QMenu * menu = ui->menuButton->getmenu();
    b1 = new QAction(QIcon(":/imgs/menu.png"), tr("&夕阳沙漠"), this);
    b2 = new QAction(QIcon(":/imgs/menu.png"), tr("&炫彩窗口"), this);
    b3 = new QAction(QIcon(":/imgs/menu.png"), tr("&仰望星空"), this);
    b = new QAction(QIcon(":/imgs/menu.png"), tr("&原始背景"), this);

    menu->addAction(b1);
    menu->addAction(b2);
    menu->addAction(b3);
    menu->addAction(b);

    connect(b1,SIGNAL(triggered(bool)),this,SLOT(bg1()));
    connect(b2,SIGNAL(triggered(bool)),this,SLOT(bg2()));
    connect(b3,SIGNAL(triggered(bool)),this,SLOT(bg3()));
    connect(b,SIGNAL(triggered(bool)),this,SLOT(bg()));

    ui->messageTextEdit->installEventFilter(this);//回车键发消息监听
    ui->username->setText(getUserName());
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);//任务栏使程序最小化

    QFile file("./msgs.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        if(line[0] == '[') {
            ui->messageBrowser->insertPlainText(line);
            ui->messageBrowser->insertPlainText("\n");
            QStringList list = line.split(']');
            QString key = list[0].append(']');
            auto user = msgMap.find(key);
            if(user == msgMap.end()) {
                msgMap.insert(key, QList<QString>());
                user = msgMap.find(key);
            }
            QString msg = in.readLine();
            QString value = line + "<br>" + msg;
            user.value()<<value;
            ui->messageBrowser->insertPlainText(msg);
            ui->messageBrowser->insertPlainText("\n");
        }
    }
    file.close();
}

/*析构函数*/
Widget::~Widget() {
    on_saveToolButton_clicked();
    delete ui;
}

/*发送消息*/
void Widget::sendMessage(MessageType type,QString serverAddress) {
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString localHostName=QHostInfo::localHostName();
    QString address=getIP();
    out<<type<<getUserName()<<localHostName;

    switch(type) {
        case Message: {
            if(ui->messageTextEdit->toPlainText()=="") {
                QMessageBox::warning(0,tr("警告"),tr("发送内容不为空"),QMessageBox::Ok);
                return;
            }
            QString msg = getPlainMessage();
            out<<address<<getMessage()<<msg;
            ui->messageBrowser->verticalScrollBar()->setValue(ui->messageBrowser->verticalScrollBar()->maximum());
            break;
        }

        case NewParticipant:
            out<<address;
            break;

        case Participantleft:
            break;

        case FileName : {
            int row = ui->userTableWidget->currentRow();
            QString clientAddress = ui->userTableWidget->item(row, 2)->text();
            out << address << clientAddress << fileName;
            break;
        }

        case Refuse :
            out << serverAddress;
            break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, port); //UDP广播
}

/*接收消息并处理*/
void Widget::processPendingDatagrams() {
    while(udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int MessageType;
        in>>MessageType;
        QString userName,localHostName,ipAddress,message,plainMsg;
        QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        switch(MessageType) {
            case Message: {
                in>>userName>>localHostName>>ipAddress>>message>>plainMsg;
                ui->messageBrowser->setTextColor(Qt::blue);
                ui->messageBrowser->setCurrentFont(QFont("黑体",12));
                ui->messageBrowser->append("["+userName+"]"+time);
                ui->messageBrowser->append(message);

                QString key = "[" + userName + "]";
                auto user = msgMap.find(key);
                if(user == msgMap.end()) {
                    msgMap.insert(key, QList<QString>());
                    user = msgMap.find(key);
                }
                QString value = "[" + userName + "]" + " " + time + "<br>" + plainMsg;
                qDebug()<<value;
                user.value().append(value);
                break;
            }

            case NewParticipant:
                in>>userName>>localHostName>>ipAddress;
                newParticipant(userName,localHostName,ipAddress);
                break;

            case Participantleft:
                in>>userName>>localHostName;
                participantleft(userName,localHostName,time);
                break;

            case FileName: {
                in >> userName >> localHostName >> ipAddress;
                QString clientAddress, fileName;
                in >> clientAddress >> fileName;
                hasPendingFile(userName, ipAddress, clientAddress, fileName);
                break;
            }

            case Refuse: {
                in >> userName >> localHostName;
                QString serverAddress;
                in >> serverAddress;
                QString ipAddress = getIP();
                if(ipAddress == serverAddress) {
                    server->refused();
                }
                break;
            }
        }
    }
}

/*用户进入聊天*/
void Widget::newParticipant(QString userName,QString localHostName,QString ipAddress) {
    bool isEmpty=ui->userTableWidget->findItems(localHostName,Qt::MatchExactly).isEmpty();
    if(isEmpty) {
        QTableWidgetItem *user=new QTableWidgetItem(userName);
        QTableWidgetItem *host=new QTableWidgetItem(localHostName);
        QTableWidgetItem *ip=new QTableWidgetItem(ipAddress);

        user->setTextAlignment(Qt::AlignCenter); //文字居中显示
        host->setTextAlignment(Qt::AlignCenter);
        ip->setTextAlignment(Qt::AlignCenter);
        ui->userTableWidget->insertRow(0);
        ui->userTableWidget->setItem(0,0,user);
        ui->userTableWidget->setItem(0,1,host);
        ui->userTableWidget->setItem(0,2,ip);
        ui->messageBrowser->setTextColor(Qt::gray);
        ui->messageBrowser->setCurrentFont(QFont("黑体",10));
        ui->messageBrowser->append(tr("%1 上线！").arg(userName));
        ui->userNumLabel->setText(tr("在线人数：%1").arg(ui->userTableWidget->rowCount()));
        sendMessage(NewParticipant);
    }
}

/*用户离开聊天*/
void Widget::participantleft(QString userName, QString localHostName, QString time) {
    int rowNum=ui->userTableWidget->findItems(localHostName,Qt::MatchExactly).first()->row();
    ui->userTableWidget->removeRow(rowNum);
    ui->messageBrowser->setTextColor(Qt::gray);
    ui->messageBrowser->setCurrentFont(QFont("黑体",10));
    ui->messageBrowser->append(tr("%1 于 %2 离开！").arg(userName).arg(time));
    ui->userNumLabel->setText(tr("在线人数：%1").arg(ui->userTableWidget->rowCount()));
}

/*获取电脑IP*/
QString Widget::getIP() {
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info=QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses()) {
        qDebug()<<address.toString();
        if(address.protocol()==QAbstractSocket::IPv4Protocol) {
            return address.toString();
        }
    }
    return 0;
}

/*获取电脑用户名*/
QString Widget::getUserName() {
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment=QProcess::systemEnvironment();
    foreach(QString string,envVariables) {
        int index=environment.indexOf(QRegExp(string));
        if(index!=-1) {
            QStringList stringlList=environment.at(index).split('=');
            if(stringlList.size()==2) {
                return stringlList.at(1);
                break;
            }
        }
    }
    return "unknown";
}

/*获取富文本信息*/
QString Widget::getMessage() {
    QString msg=ui->messageTextEdit->toHtml();
    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
    return msg;
}

/*获取纯文本信息*/
QString Widget::getPlainMessage() {
    QString msg=ui->messageTextEdit->toPlainText();
    return msg;
}

/*发送按钮*/
void Widget::on_sendButton_clicked() {
    sendMessage(Message);
}

/*获取要发送的文件名*/
void Widget::getFileName(QString name) {
    fileName = name;
    sendMessage(FileName);
}

/*传输文件按钮*/
void Widget::on_sendToolButton_clicked() {
    if(ui->userTableWidget->selectedItems().isEmpty()) {
        QMessageBox::warning(0, tr("选择用户"), tr("请先从用户列表选择要传送的用户！"), QMessageBox::Ok);
        return;
    }

    if(ui->userTableWidget->selectedItems()[0]->text() == getUserName()) {
        QMessageBox::warning(0, tr("选择不合法"), tr("您不可以向自己发送文件！"), QMessageBox::Ok);
        return;
    }
    server->show();
    server->initServer();
}

/*发送邮件*/
void  Widget::on_sendMailButton_clicked(){
    mail->setWindowTitle("发送邮件");
    mail->show();
}

/*查看聊天记录*/
void Widget::on_checkMessageButton_clicked() {
    if(ui->userTableWidget->selectedItems().isEmpty()) {
        QMessageBox::warning(0, tr("选择用户"), tr("请选择需要查看聊天记录的用户"), QMessageBox::Ok);
        return;
    }
    QString user = ui->userTableWidget->selectedItems()[0]->text();
    qDebug()<<user;
    QList<QString> msgs = msgMap.find("[" + user + "]").value();
    for(int i=0;i<msgs.length();i++)
    {
        qDebug()<<"[" + user + "] "<<msgs[i];
    }
    Messages messagesWindow(msgs);
    messagesWindow.setWindowTitle(user + "的消息记录");
    messagesWindow.exec();
}

/*搜索信息按钮*/
void Widget::on_searchButton_clicked() {
    if(ui->searchBox->toPlainText()=="") return;
    QString target = ui->searchBox->toPlainText().trimmed();
    ui->searchBox->clear();
    MsgSearch search;
    QList<QString> msgs;
    for(auto msgList: msgMap) {
        for(auto msg:msgList)  {
            if(search.KMP(target, msg)) msgs<<msg;
        }
    }
    Messages messagesWindow(msgs);
    messagesWindow.setWindowTitle("查找结果");
    messagesWindow.exec();
}

/*判断是否接收文件*/
void Widget::hasPendingFile(QString userName, QString serverAddress,QString clientAddress, QString fileName) {
    QString ipAddress = getIP();
    if(ipAddress == clientAddress) {
        int btn = QMessageBox::information(this,tr("接受文件"),
                                           tr("来自%1(%2)的文件：%3, 是否接收？")
                                           .arg(userName)
                                           .arg(serverAddress)
                                           .arg(fileName),
                                           QMessageBox::Yes,QMessageBox::No);
        if (btn == QMessageBox::Yes) {
            QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),fileName);
            if(!name.isEmpty()) {
                TcpClient *client = new TcpClient(this);
                client->setFileName(name);
                client->setHostAddress(QHostAddress(serverAddress));
                client->show();
            }
        } else sendMessage(Refuse, serverAddress);
    }
}

/*字体变化*/
void Widget::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->messageTextEdit->setCurrentFont(f);
    ui->messageTextEdit->setFocus();
}

/*字体大小变化*/
void Widget::on_sizeComboBox_currentIndexChanged(QString size)
{
    ui->messageTextEdit->setFontPointSize(size.toDouble());
    ui->messageTextEdit->setFocus();
}

/*字体加粗*/
void Widget::on_boldToolButton_clicked(bool checked) { //粗体，按钮属性设置为checkable
    if(checked) ui->messageTextEdit->setFontWeight(QFont::Bold);
    else ui->messageTextEdit->setFontWeight(QFont::Normal);
    ui->messageTextEdit->setFocus();
}

/*斜体*/
void Widget::on_italicToolButton_clicked(bool checked) { //斜体，checkable
    ui->messageTextEdit->setFontItalic(checked);
    ui->messageTextEdit->setFocus();
}

/*下划线*/
void Widget::on_underlineToolButton_clicked(bool checked) { //下划线，checkable
    ui->messageTextEdit->setFontUnderline(checked);
    ui->messageTextEdit->setFocus();
}

/*字体颜色*/
void Widget::on_colorToolButton_clicked() {
    color = QColorDialog::getColor(color, this);
    if (color.isValid()) {
        ui->messageTextEdit->setTextColor(color);
        ui->messageTextEdit->setFocus();
    }
}

/*保存聊天记录*/
void Widget::on_saveToolButton_clicked() {
    if (ui->messageBrowser->document()->isEmpty()) {
        QMessageBox::warning(0, tr("警告"), tr("聊天记录为空，无法保存！"), QMessageBox::Ok);
    } else {
        QString fileName = "./msgs.txt";
        if(!fileName.isEmpty()) saveFile(fileName);
    }
}

/*保存到文件*/
bool Widget::saveFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("保存文件"), tr("无法保存文件 %1:\n %2").arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << ui->messageBrowser->toPlainText();

    return true;
}

/*清理聊天*/
void Widget::on_clearToolButton_clicked() {
    ui->messageBrowser->clear();
}

/*转换状态*/
void Widget::currentFormatChanged(const QTextCharFormat &format) { //鼠标点击不同大小文字，转换到对应状态
    ui->fontComboBox->setCurrentFont(format.font());

    if (format.fontPointSize() < 9) {
        ui->sizeComboBox->setCurrentIndex(3);
    } else {
        ui->sizeComboBox->setCurrentIndex( ui->sizeComboBox
                                          ->findText(QString::number(format.fontPointSize())));
    }
    ui->boldToolButton->setChecked(format.font().bold());
    ui->italicToolButton->setChecked(format.font().italic());
    ui->underlineToolButton->setChecked(format.font().underline());
    color = format.foreground().color();
}

/*退出按钮*/
void Widget::on_exitButton_clicked() {
    close();
}

/*关闭事件*/
void Widget::closeEvent(QCloseEvent *e) {
    sendMessage(Participantleft);
    QWidget::closeEvent(e);
}

/**/
void Widget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    if(this->width() > 45 && this->height() > 45) drawWindowShadow(p);

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::white);
    p.drawRoundedRect(QRect(5,5,this->width() - 10,this->height() - 10),2.0f,2.0f);
}

/*绘制窗口阴影*/
void Widget::drawWindowShadow(QPainter &p) {
    QList<QPixmap> pixmaps;
    pixmaps.append(QPixmap(":/imgs/shadow_left.png"));
    pixmaps.append(QPixmap(":/imgs/shadow_right.png"));
    pixmaps.append(QPixmap(":/imgs/shadow_top.png"));
    pixmaps.append(QPixmap(":/imgs/shadow_bottom.png"));
    pixmaps.append(QPixmap(":/imgs/shadow_corner1.png"));
    pixmaps.append(QPixmap(":/imgs/shadow_corner2.png"));
    pixmaps.append(QPixmap(":/imgs/shadow_corner3.png"));
    pixmaps.append(QPixmap(":/imgs/shadow_corner4.png"));

    p.drawPixmap(0,0,5,5,pixmaps[4]);
    p.drawPixmap(this->width() - 5,0,5,5,pixmaps[5]);
    p.drawPixmap(0,this->height() - 5,5,5,pixmaps[6]);
    p.drawPixmap(this->width() - 5,this->height() - 5,5,5,pixmaps[7]);

    p.drawPixmap(0,5,5,this->height() - 10,pixmaps[0].scaled(5,this->height() - 10));
    p.drawPixmap(this->width() - 5,5,5,this->height() - 10,pixmaps[1].scaled(5,this->height() - 10));
    p.drawPixmap(5,0,this->width() - 10,5,pixmaps[2].scaled(this->width() - 10,5));
    p.drawPixmap(5,this->height() - 5,this->width() - 10,5,pixmaps[3].scaled(this->width() - 10,5));
}

/*绘制逐渐消失特效*/
void Widget::on_closeButton_clicked() {
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(600);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation,SIGNAL(finished()),this,SLOT(close()));//效果显示完后关闭
}

/*最小化*/
void Widget::on_minButton_clicked() {
    this->showMinimized(); //最小化
}

/*最大化*/
//void Widget::on_maxButton_clicked() {
//    this->showMaximized();//最大化
//    QMessageBox::warning(0,tr("提示"),tr("测试功能"),QMessageBox::Ok);
//}

/*鼠标点击事件*/
void Widget::mousePressEvent(QMouseEvent *event) {
    if(event->buttons() == Qt::LeftButton) {
    //记录窗口此时的坐标
    StartPos = event->globalPos() - this->frameGeometry().topLeft();
    }
}

/*鼠标移动事件*/
void Widget::mouseMoveEvent(QMouseEvent *event) {
    //去表格虚框
    QPoint EndPos;
    if ( event->buttons() == Qt::LeftButton ) {
    EndPos = event->globalPos() - StartPos;
    this->move(EndPos);
    }
}

/*绘制*/
void NoFocusDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const {
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus) itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
    QStyledItemDelegate::paint(painter, itemOption, index);
}

/*背景*/
void Widget::bg1() {
    ui->stackedWidget->setStyleSheet("QStackedWidget {background-image: url(:/imgs/bg1.png);}");
    ui->label->setStyleSheet("QLabel { color:#000000; }");
    ui->username->setStyleSheet("QLabel { color:#000000; }");
}

/*背景*/
void Widget::bg2() {
    ui->stackedWidget->setStyleSheet("QStackedWidget {background-image: url(:/imgs/bg2.png);}");
    ui->label->setStyleSheet("QLabel { color:#ffffff; }");
    ui->username->setStyleSheet("QLabel { color:#ffffff; }");
}

/*背景*/
void Widget::bg3() {
    ui->stackedWidget->setStyleSheet("QStackedWidget {background-image: url(:/imgs/bg3.png);}");
    ui->label->setStyleSheet("QLabel { color:#ffffff; }");
    ui->username->setStyleSheet("QLabel { color:#ffffff; }");
}

/*背景*/
void Widget::bg() {
    ui->stackedWidget->setStyleSheet("");
    ui->label->setStyleSheet("QLabel { color:#000000; }");
    ui->username->setStyleSheet("QLabel { color:#000000; }");
}

/*事件过滤*/
bool Widget::eventFilter(QObject *obj, QEvent *event) {
    if(obj == ui->messageTextEdit) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast <QKeyEvent *>(event);
            if(keyEvent->key()==Qt::Key_Return) {
                qDebug()<<"int event filter";
                emit sendMessage(Message);
                return true;
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
