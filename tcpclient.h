/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>
#include <QHostAddress>
#include <QFile>
#include <QElapsedTimer>

class QTcpSocket;

namespace Ui {
class TcpClient;                                            //tcpclient界面命名空间
}

class TcpClient : public QDialog {                          //继承自QDialog
    Q_OBJECT                                                //基类宏

public:
    explicit TcpClient(QWidget *parent = 0);                //构造函数
    ~TcpClient();                                           //析构函数

    void setHostAddress(QHostAddress address);              //设置主机地址
    void setFileName(QString fileName);                     //设置文件路径

protected:
    void closeEvent(QCloseEvent *);                         //关闭连接事件


private:
    Ui::TcpClient *ui;                                      //客户端界面

    QTcpSocket *tcpClient;                                  //客户端连接
    quint16 blockSize;                                      //数据块大小
    QHostAddress hostAddress;                               //主机地址
    quint16 tcpPort;                                         //主机端口

    qint64 TotalBytes;                                      //数据总大小
    qint64 bytesReceived;                                   //接收到的数据大小
    qint64 fileNameSize;                                    //文件大小
    QString fileName;                                       //文件
    QFile *localFile;                                       //本地文件路径
    QByteArray inBlock;                                     //接收的数据块

    QElapsedTimer time;                                     //连接起止时间

private slots:
    void on_tcpClientCancleBtn_clicked();                   //取消客户端连接

    void on_tcpClientCloseBtn_clicked();                    //关闭客户端连接

    void newConnect();                                      //新建连接

    void readMessage();                                     //读取消息

    void displayError(QAbstractSocket::SocketError);        //打印错误信息
};

#endif // TCPSERVER_H
