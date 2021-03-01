#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent():
    mType(EventType::Invalid),
    mKey(0u)
{
}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key):
    mType(type),
    mKey(key)
{
}

bool KeyboardEvent::IsPress() const
{
    return this->mType == EventType::Press;
}

bool KeyboardEvent::IsRelease() const
{
    return this->mType == EventType::Release;
}

bool KeyboardEvent::IsValid() const
{
    return this->mType == EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
    return this->mKey;
}
