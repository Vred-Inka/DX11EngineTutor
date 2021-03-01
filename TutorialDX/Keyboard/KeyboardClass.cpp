#include "KeyboardClass.h"

KeyboardClass::KeyboardClass()
{
    for (int i = 0; i < 256; i++)
    {
        this->mKeyStates[i] = false;
    }
}

bool KeyboardClass::KeyIsPressed(const unsigned char keyCode)
{
    return this->mKeyStates[keyCode];
}

bool KeyboardClass::KeyBufferIsEmpty()
{
    return this->mKeyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
{
    return this->mCharBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
    if (this->mKeyBuffer.empty())
    {
        return KeyboardEvent();
    }
    else
    {
        KeyboardEvent e = this->mKeyBuffer.front();
        this->mKeyBuffer.pop();
        return e;
    }
}

unsigned char KeyboardClass::ReadChar()
{
    if (this->CharBufferIsEmpty())
    {
        return 0u;
    }
    else
    {
        unsigned char e = this->mCharBuffer.front();
        this->mCharBuffer.pop();
        return e;
    }
    return 0;
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
    this->mKeyStates[key] = true;
    this->mKeyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
    this->mKeyStates[key] = false;
    this->mKeyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
    this->mCharBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
    this->mAutoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
    this->mAutoRepeatKeys = false;
}

bool KeyboardClass::IsKeysAutoRepeat()
{
    return this->mAutoRepeatKeys;
}

bool KeyboardClass::IsCharsAutoRepeat()
{
    return this->mAutoRepeatChars;
}

void KeyboardClass::DisableAutoRepeatKeys()
{
    this->mAutoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
    this->mAutoRepeatChars = true;
}
