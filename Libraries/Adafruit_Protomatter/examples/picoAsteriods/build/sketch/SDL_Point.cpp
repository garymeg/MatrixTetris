#line 1 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\picoAsteriods\\SDL_Point.cpp"
#pragma once

class SDL_Point
{
    public:
        float x = 0;
        float y = 0;

        SDL_Point() {}

        SDL_Point(int x, int y)
        {
            x = x;
            y = y;
        }
};
