#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDialog>
#include <QElapsedTimer>

class QFile;
class QTcpServer;
class QTcpSocket;

namespace Ui {
class TcpServer;                                        //TcpServer界面命名空间
}

class TcpServer : public QDialog {                      //继承自QDialog
    Q_OBJECT                                            //基类宏

public:
    explicit TcpServer(QWidget *parent = 0);            //构造函数
    ~TcpServer();                                       //析构函数

    void initServer();                                  //初始化服务器
    void refused();                                     //拒绝服务

protected:
    void closeEvent(QCloseEvent *);                     //关闭连接事件


private:
    Ui::TcpServer *ui;                                  //服务器界面

    qint16 tcpPort;                                     //服务器端口
    QTcpServer *tcpServer;                              //服务器连接
    QString fileName;                                   //文件路径
    QString theFileName;                                //文件
    QFile *localFile;                                   //本地文件路径

    qint64 TotalBytes;                                  //数据总大小
    qint64 bytesWritten;                                //发送后的数据大小
    qint64 bytesToWrite;                                //将要发送的数据大小
    qint64 payloadSize;                                 //窗口大小
    QByteArray outBlock;                                //发送的数据块

    QTcpSocket *clientConnection;                       //客户端长连接
    QElapsedTimer time;                                 //连接起止时间

private slots:
    void sendMessage();                                 //发送消息

    void updateClientProgress(qint64 numBytes);         //更新客户端进程

    void on_serverOpenBtn_clicked();                    //服务器连接打开

    void on_serverSendBtn_clicked();                    //服务器数据发送

    void on_serverCloseBtn_clicked();                   //服务器连接关闭

signals:
    void sendFileName(QString fileName);                //发送文件
};

#endif // TCPSERVER_H

