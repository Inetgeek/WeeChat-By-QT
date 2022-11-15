/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#ifndef SEARCH_H
#define SEARCH_H

#include <QString>

class MsgSearch {
public:
    void getNext(const QString &pattern, int *next);        //获得下一条消息

    bool KMP(const QString &pattern, const QString &txt);   //KMP模式匹配搜索算法
};

#endif // SEARCH_H
