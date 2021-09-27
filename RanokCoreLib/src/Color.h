#ifndef COLOR_H
#define COLOR_H


struct Color
{
    Color(){}

    Color(float r, float g, float b, float a):
        red(r),
        green(g),
        blue(b),
        alpha(a)
    {}

    static Color fromUint(unsigned char r,unsigned char g,
                          unsigned char b,unsigned char a)
    {
        return Color(r/255.f, g/255.f,
                     b/255.f, a/255.f);
    }

    static Color fromHex(unsigned int color)
    {
        return fromUint(((color >> 24) & 0xFF),
                        ((color >> 16) & 0xFF),
                        ((color >> 8) & 0xFF),
                        ((color) & 0xFF));
    }
    float red = 0.f;
    float green = 0.f;
    float blue = 0.f;
    float alpha = 0.f;
};


#endif // COLOR_H
