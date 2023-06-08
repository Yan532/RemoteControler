#include "windows_eventapi.h"
#include "windows.h"

void Windows_eventapi::mouseLeftPress(const QPoint &pos)
{
    SetCursorPos(int(pos.x()), int(pos.y()));
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

void Windows_eventapi::mouseRightPress(const QPoint &pos)
{
    SetCursorPos(int(pos.x()), int(pos.y()));
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
}

void Windows_eventapi::mouseLeftRelease(const QPoint &pos)
{
    SetCursorPos(int(pos.x()), int(pos.y()));
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Windows_eventapi::mouseRightRelease(const QPoint &pos)
{
    SetCursorPos(int(pos.x()), int(pos.y()));
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}

void Windows_eventapi::mouseMove(const QPoint &pos){
    SetCursorPos(int(pos.x()), int(pos.y()));
    mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0);
}

void Windows_eventapi::keyPress(const int &key)
{
    keybd_event(key,0x45,KEYEVENTF_EXTENDEDKEY,0);
}

void Windows_eventapi::keyRelease(const int &key)
{
    keybd_event(key,0x45,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,0);
}

