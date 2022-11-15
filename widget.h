/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextCharFormat>
#include <QtGui>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QMenu>
#include <QAction>
#include <QTextEdit>

/*枚举标志信息类型:消息 0、新用户加入 1、用户退出 2、文件名 3、拒绝接受文件 4*/
enum MessageType{Message,NewParticipant,Participantleft,FileName,Refuse};

namespace Ui {
class Widget;                                                        //Widget界面命名空间
}

class QUdpSocket;                                                    //QUdpSocket类
class TcpServer;                                                     //TcpServer类
class frmEmailTool;
class Widget : public QWidget                                        //继承自QWidget
{
    Q_OBJECT                                                         //基类宏

public:
    explicit Widget(QWidget *parent = 0);                            //构造函数
    ~Widget();                                                       //析构函数
    QAction *b1,*b2,*b3,*b4,*b;                                      //背景

private:
    Ui::Widget *ui;                                                  //ui布局
    QUdpSocket * udpSocket;                                          //udp
    qint16 port;                                                     //udp端口
    QString fileName;
    QString localHostName;                                           //本机ip
    TcpServer *server;                                               //远程ip
    frmEmailTool *mail;                                              //邮箱
    QColor color;                                                    //透明色
    void drawWindowShadow(QPainter &p);                              //阴影
    void mouseMoveEvent(QMouseEvent *event);                         //鼠标移动事件
    void mousePressEvent(QMouseEvent *event);                        //鼠标点击事件
    QPoint StartPos;                                                 //记录窗口当前的位置
    QMap<QString,QList<QString> > msgMap;                            //消息体

protected:
    void sendMessage(MessageType type,QString serverAddress="");                    //发消息

    void newParticipant(QString userName,QString localHostName,QString ipAddress);  //新加入的好友

    void participantleft(QString userName,QString localHostName,QString time);      //离开的好友

    QString getIP();                                                                //获取ip

    QString getUserName();                                                          //获取用户名

    QString getMessage();                                                           //获取信息

    QString getPlainMessage();                                                      //获取纯文本信息

    void hasPendingFile(QString userName, QString serverAddress, QString clientAddress, QString fileName); //是否接受文件

    bool saveFile(const QString& fileName);                                         //保存文件

    void closeEvent(QCloseEvent *);                                                 //关闭事件

    void paintEvent(QPaintEvent *);                                                 //阴影指针

    bool eventFilter(QObject *obj, QEvent *event);                                  //时间过滤

private slots:
    void processPendingDatagrams();                                                 //接受数据

    void getFileName(QString);                                                      //获取文件名

    void on_sendButton_clicked();                                                   //点击发送

    void on_sendToolButton_clicked();                                               //发送控件

    void on_sendMailButton_clicked();                                               //邮件发送

    void on_checkMessageButton_clicked();                                           //信息发送

    void on_searchButton_clicked();                                                 //搜索

    void on_fontComboBox_currentFontChanged(const QFont &f);                        //现在的字体

    void on_sizeComboBox_currentIndexChanged(QString);                              //现在的位置

    void on_boldToolButton_clicked(bool checked);                                   //加粗

    void on_italicToolButton_clicked(bool checked);                                 //斜体

    void on_underlineToolButton_clicked(bool checked);                              //下划线

    void on_colorToolButton_clicked();                                              //字体颜色

    void currentFormatChanged(const QTextCharFormat &format);                       //格式化

    void on_exitButton_clicked();                                                   //退出

    void on_saveToolButton_clicked();                                               //保存

    void on_clearToolButton_clicked();                                              //清空

    void on_closeButton_clicked();                                                  //关闭

    void on_minButton_clicked();                                                    //最小化

//    void on_maxButton_clicked();                                                  ////最大化

    void bg();                                                                      //背景1

    void bg1();                                                                     //背景2

    void bg2();                                                                     //背景3

    void bg3();                                                                     //背景4
};

class NoFocusDelegate : public QStyledItemDelegate {                                //虚线

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // WIDGET_H
