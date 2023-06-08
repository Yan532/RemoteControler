#ifndef WINDOWS_EVENTAPI_H
#define WINDOWS_EVENTAPI_H

#include <QPoint>

class Windows_eventapi
{
public:
    static void mouseLeftPress(const QPoint &pos);
    static void mouseRightPress(const QPoint &pos);
    static void mouseLeftRelease(const QPoint &pos);
    static void mouseRightRelease(const QPoint &pos);
    static void mouseMove(const QPoint &pos);
    static void keyPress(const int &key);
    static void keyRelease(const int &key);
};

#endif // WINDOWS_EVENTAPI_H
