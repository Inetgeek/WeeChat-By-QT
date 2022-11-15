/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#include "messages.h"
#include "ui_messages.h"

/*消息构造*/
Messages::Messages(QList<QString> messages, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Messages),
    msgs(messages)
{
    ui->setupUi(this);
    for(int i=0;i<msgs.length();i++) {
        ui->messageBrowser->append(msgs[i]);
    }
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(exit()));
    this->setStyleSheet("background-image: ;QDialog {border-top-left-radius:15px;border-top-right-radius:5px;}");
}

/*退出消息*/
void Messages::exit() {
    this->accept();
}

/*消息析构*/
Messages::~Messages() {
    delete ui;
}
