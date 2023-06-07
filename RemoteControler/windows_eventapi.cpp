#include "windows_eventapi.h"
#include "windows.h"

void Windows_eventapi::mouseMove(const QPointF &pos){
    SetCursorPos(int(pos.x()), int(pos.y()));

}

void Windows_eventapi::mousePress(const QPointF &pos){
    SetCursorPos(int(pos.x()), int(pos.y()));
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

void Windows_eventapi::mouseRelease(const QPointF &pos){
    SetCursorPos(int(pos.x()), int(pos.y()));
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
