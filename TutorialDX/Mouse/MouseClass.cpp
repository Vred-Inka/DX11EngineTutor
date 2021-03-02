#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y)
{
    this->mLeftIsDown = true;
    MouseEvent me(MouseEvent::EventType::LPress, x, y);
    this->mEventBuffer.push(me);
}

void MouseClass::OnLeftReleased(int x, int y)
{
    this->mLeftIsDown = false;
    this->mEventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseClass::OnRightPressed(int x, int y)
{
    this->mRightIsDown = true;
    this->mEventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void MouseClass::OnRightReleased(int x, int y)
{
    this->mRightIsDown = false;
    this->mEventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseClass::OnMiddlePressed(int x, int y)
{
    this->mButtonIsDown = true;
    this->mEventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseClass::OnMiddleReleased(int x, int y)
{
    this->mButtonIsDown = false;
    this->mEventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
}

void MouseClass::OnWheelUp(int x, int y)
{
    this->mEventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseClass::OnWheelDown(int x, int y)
{
    this->mEventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
    this->mX = x;
    this->mY = y;
    this->mEventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
    this->mEventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

bool MouseClass::IsLeftDown()
{
    return this->mLeftIsDown;
}

bool MouseClass::IsMiddleDown()
{
    return this->mButtonIsDown;
}

bool MouseClass::IsRightDown()
{
    return this->mRightIsDown;;
}

int MouseClass::GetPosX()
{
    return this->mX;
}

int MouseClass::GetPosY()
{
    return this->mY;
}

MousePoint MouseClass::GetPos()
{
    return { this->mY, this->mY };
}

bool MouseClass::EventBufferIsEmpty()
{
    return this->mEventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
    if (this->EventBufferIsEmpty())
    {
        return MouseEvent();
    }
    else
    {
        MouseEvent e = this->mEventBuffer.front();
        this->mEventBuffer.pop();
        return e;
    }
}
