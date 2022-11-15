/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#include "search.h"

/*指向下一条消息*/
void MsgSearch::getNext(const QString &pattern, int *next) {
    next[0] = -1;
    int j = -1;
    for(int i = 1;i < pattern.length();i++) {
        while(j > -1 && pattern[j + 1] != pattern[i]) {
            j = next[j];
        }
        if(pattern[j + 1] == pattern[i]) {
            j = j + 1;
        }
        next[i] = j;
    }
}

/*KMP模式匹配算法*/
bool MsgSearch::KMP(const QString &pattern, const QString &txt) {
    int *next = new int[pattern.length()];
    getNext(pattern, next);
    int j = -1;
    for(int i = 0;i < txt.length();i++) {
        while(j > -1 && pattern[j + 1] != txt[i]) {
            j = next[j];
        }
        if(pattern[j + 1] == txt[i]) {
            j = j + 1;
        }
        if(j == pattern.length() - 1) {
            delete[]next;
            return true;
        }
    }
    delete[]next;
    return false;
}
