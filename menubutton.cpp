/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#include "menubutton.h"
#include <QMenu>

/*菜单构造*/
MenuButton::MenuButton(QWidget *parent):
    QPushButton(parent) {
    menu = new QMenu(this);
    connect(this,SIGNAL(clicked()),this,SLOT(PushMenu()));
}

/*获取菜单*/
QMenu *MenuButton::getmenu() {
    return menu;
}

/*显示菜单*/
void MenuButton::PushMenu() {
    QPoint pos;
    int y = pos.y();
    pos.setY(y+this->geometry().height());
    menu->exec(this->mapToGlobal(pos));
}
