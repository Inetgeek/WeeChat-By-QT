/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#ifndef MENUBUTTON
#define MENUBUTTON

#include <QPushButton>

class QMenu;                                        //主菜单类
class MenuButton : public QPushButton               //继承自QPushButton
{
    Q_OBJECT                                        //基类宏

public:
    explicit MenuButton(QWidget *parent = 0);       //菜单构造
    QMenu *getmenu();                               //获取菜单

private slots:
    void PushMenu();                                //显示菜单

private:
    QMenu *menu;                                    //菜单指针
};
#endif // MENUBUTTON

