#include "windows_eventapi.h"
#include "windows.h"

void Windows_eventapi::mouseMove(const QPoint &pos){
    SetCursorPos(int(pos.x()), int(pos.y()));
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

void Windows_eventapi::mousePress(const QPoint &pos){
    SetCursorPos(int(pos.x()), int(pos.y()));
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Windows_eventapi::mouseRelease(const QPoint &pos){
    SetCursorPos(int(pos.x()), int(pos.y()));
    mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0);
}
