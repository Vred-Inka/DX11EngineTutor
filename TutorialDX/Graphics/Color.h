#pragma once
typedef unsigned char BYTE;

class Color
{
public:
    Color();
    Color(unsigned int val);
    Color(BYTE r, BYTE g, BYTE b);
    Color(BYTE r, BYTE g, BYTE b, BYTE a);
    Color(const Color& src);

    Color& operator=(const Color& src);
    bool operator==(const Color& src) const;
    bool operator!=(const Color& src) const;

    constexpr BYTE GetR() const;
    void SetR(BYTE r);

    constexpr BYTE GetG() const;
    void SetG(BYTE g);

    constexpr BYTE GetB() const;
    void SetB(BYTE b);

    constexpr BYTE GetA() const;
    void SetA(BYTE a);
    
private:
    union
    {
        BYTE rgba[4];
        unsigned int color;
    };
};

namespace Colours
{
    const Color UnloadedTextureColor(0, 0, 255);
    const Color UnhandledTextureColor(255, 0, 0);
}