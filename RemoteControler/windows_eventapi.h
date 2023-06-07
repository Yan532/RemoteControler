#ifndef WINDOWS_EVENTAPI_H
#define WINDOWS_EVENTAPI_H

#include <QPoint>

class Windows_eventapi
{
public:
    static void mousePress(const QPoint &pos);
    static void mouseRelease(const QPoint &pos);
    static void mouseMove(const QPoint &pos);
    //static void keyPress(const int &key);
    //static void keyRelease(const int &key);
};

#endif // WINDOWS_EVENTAPI_H
