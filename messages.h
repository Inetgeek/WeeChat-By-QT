/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#ifndef MESSAGES_H
#define MESSAGES_H

#include <QDialog>

namespace Ui {
class Messages;                             //消息UI命名空间
}

class Messages : public QDialog             //继承自QDialog类
{
    Q_OBJECT                                //基类宏

public:
    explicit Messages(QList<QString> messages, QWidget *parent = nullptr);      //构造Message
    ~Messages();                                                                //析构Message

private:
    Ui::Messages *ui;                       //Message类ui指针
    QList<QString> msgs;                    //消息指针
    QString user;                           //用户指针

private slots:
    void exit();                            //退出消息
};

#endif // MESSAGES_H
