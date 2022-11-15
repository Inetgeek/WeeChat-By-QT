/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#ifndef FRMEMAILTOOL_H
#define FRMEMAILTOOL_H

#include <QDialog>

class QSettings;

namespace Ui
{
class frmEmailTool;                                     //邮件UI命名空间
}

class frmEmailTool : public QDialog                     //继承自QDialog
{
    Q_OBJECT                                            //基类宏

public:
    explicit frmEmailTool(QWidget *parent = 0);         //构造函数
    ~frmEmailTool();                                    //析构函数

private:
    Ui::frmEmailTool *ui;                               //ui
    QSettings *settings;                                //设置

private:
    bool check();                                       //检查状态

private slots:
    void initForm();                                    //初始化邮件控件
    void receiveEmailResult(QString result);            //邮件发送情况

private slots:
    void on_btnSend_clicked();                          //发送
    void on_btnSelect_clicked();                        //选择
    void on_cboxServer_currentIndexChanged(int index);  //位置
};

#endif // FRMEMAILTOOL_H
