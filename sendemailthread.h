/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#ifndef SENDEMAILTHREAD_H
#define SENDEMAILTHREAD_H

#include <QThread>
#include <QMutex>
#include <QStringList>

class SendEmailThread : public QThread              //继承自QThread
{
	Q_OBJECT
public:
    static SendEmailThread *Instance();             //初始化
    explicit SendEmailThread(QObject *parent = 0);  //发送邮件线程
    ~SendEmailThread();                             //析构函数

protected:
    void run();                                     //运行函数

private:
    static QScopedPointer<SendEmailThread> self;    //静态指针
    QMutex mutex;                                   //线程锁
    volatile bool stopped;                          //停止

	QString emialTitle;         //邮件标题
	QString sendEmailAddr;      //发件人邮箱
	QString sendEmailPwd;       //发件人密码
	QString receiveEmailAddr;   //收件人邮箱,可多个中间;隔开
	QStringList contents;       //正文内容
	QStringList fileNames;      //附件

signals:
    void receiveEmailResult(const QString &result); //接收邮件发送情况

public slots:    
    void stop(); //停止
    void setEmailTitle(const QString &emailTitle);  //标题
    void setSendEmailAddr(const QString &sendEmailAddr); //发送的邮箱
    void setSendEmailPwd(const QString &sendEmailPwd); //密码
    void setReceiveEmailAddr(const QString &receiveEmailAddr); //收件箱
    void append(const QString &content, const QString &fileName);  //附件

};

#endif // SENDEMAILTHREAD_H
