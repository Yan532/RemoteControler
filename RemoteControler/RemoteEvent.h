#ifndef REMOTEEVENT_H
#define REMOTEEVENT_H

#include <QPoint>

class RemoteEvent{
public:
    enum class EventType
    {
        NoType = 0,
        LeftPressed,
        RightPressed,
        LeftReleased,
        RightReleased,
        Moved,
        KeyInput,
        KeyUP
    };

    RemoteEvent() { }
    RemoteEvent(EventType type, const QPoint &position)
        : m_type(type), m_position(position) { }
    RemoteEvent(EventType type, const int &k)
        : m_type(type), key(k){}

    EventType type() const { return m_type; }
    void setType(EventType type) { m_type = type; }
    QPoint position() const { return m_position; }
    void setPosition(const QPoint &position) { m_position = position; }
    int getkey() const{return key;}
    void setkey(const int &k){key = k;}

    bool isEmpty() { return m_type == EventType::NoType && m_position.isNull(); }
    int size() { return sizeof(m_type) + sizeof(m_position) + sizeof(key) + sizeof(int); }

private:
    EventType m_type = EventType::NoType;
    QPoint m_position;
    int key = 0;
};

#endif // REMOTEEVENT_H
