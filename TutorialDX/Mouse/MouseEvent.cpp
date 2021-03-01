#pragma once
#include "MouseEvent.h"

MouseEvent::MouseEvent() :
    mType(EventType::Invalid),
    mX(0),
    mY(0)
{
}

MouseEvent::MouseEvent(const EventType type, const int x, const int y) :
    mType(type),
    mX(x),
    mY(y)
{
}

bool MouseEvent::IsValid() const
{
    return this->mType != EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const
{
    return this->mType;
}

MousePoint MouseEvent::GetPos() const
{
    return { this->mX, this->mY };
}

int MouseEvent::GetPosX() const
{
    return this->mX;
}

int MouseEvent::GetPosY() const
{
    return  this->mY;
}
