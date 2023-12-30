#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <Adafruit_Protomatter.h>
#include <vector>

class Point
{
    protected:
        float xPoint = 0;
        float yPoint = 0;

        int colRed = 255 >> 3;
        int colGreen = 255 >> 3;
        int colBlue = 255 >> 3;

        Adafruit_Protomatter* matrix;

    public:
        Point() {}

        Point(Adafruit_Protomatter* mtrix, int x, int y)
        {
            xPoint = x;
            yPoint = y;
            matrix = mtrix;
        }

        Point(Adafruit_Protomatter* mtrix, int x, int y, int r, int g, int b)
        {
            xPoint = x;
            yPoint = y;
            matrix = mtrix;

            colRed = r;
            colGreen = g;
            colBlue = b;
        }

        float getXPoint()
        {
            return xPoint;
        }

        float getYPoint()
        {
            return yPoint;
        }

        void SetColour(int cRed, int cGreen, int cBlue)
        {
            colRed = cRed >> 3;
            colGreen = cGreen >> 3;
            colBlue = cBlue >> 3;
        }

        void Draw()
        {
          matrix->drawPixel(xPoint, yPoint, matrix->Color888(colRed, colGreen, colBlue));
        }

        double distanceBetweenObjects(const Point &object)
        {
            int xDiff = xPoint-object.xPoint;
            int yDiff = yPoint-object.yPoint;

            double distance = sqrt((xDiff*xDiff)+(yDiff*yDiff));
            //std::cout << distance << std::endl;
            return distance;
        }
};
