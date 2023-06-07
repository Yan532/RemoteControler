#ifndef WINDOWS_EVENTAPI_H
#define WINDOWS_EVENTAPI_H

#include <QPoint>

class Windows_eventapi
{
public:
    static void mousePress(const QPointF &pos);
    static void mouseRelease(const QPointF &pos);
    static void mouseMove(const QPointF &pos);
    //static void keyPress(const int &key);
    //static void keyRelease(const int &key);
};

#endif // WINDOWS_EVENTAPI_H
