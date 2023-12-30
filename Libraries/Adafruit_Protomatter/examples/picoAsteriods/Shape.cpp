#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <Adafruit_Protomatter.h>
#include <vector>

#include "Point.cpp"
#include "Constants.cpp"

class Shape: public Point
{
    protected:
        int radius = 1;
        int rotation = 0;
        int noOfSides = 360;
        float radiansRotation = 0;
        float radiansAngleForSides = 0;

        int debugColRed = 255 >> 3;
        int debugColGreen = 165 >> 3;
        int debugColBlue = 0;

        int getX(float angleInRadians)
        {
            return xPoint + (radius * cos(angleInRadians));
        }

        int getY(float angleInRadians)
        {
            return yPoint + (radius * sin(angleInRadians));
        }

        float getRadiansFromAngle(float angle)
        {
            // angles is in degrees, return is in radians
            // return static_cast< float >(angle) / 180 * M_PI;
            return angle / 180.0 * M_PI;
        }

        void setNoOfSides(int sides)
        {
            // std::cout << sides << std::endl;

            noOfSides = sides;
            float angle = 360.0 / sides;

            radiansAngleForSides = getRadiansFromAngle(angle);
        }

        void DrawShape(float radiansRot, int sides, float radiansAngle)
        {
            float currentAngle = radiansRot;

            int x1 = getX(currentAngle);
            int y1 = getY(currentAngle);

            int x2 = 0;
            int y2 = 0;

            for (int ns=1; ns<=sides; ns++)
            {
                currentAngle += radiansAngle;

                x2 = getX(currentAngle);
                y2 = getY(currentAngle);

                matrix->drawLine(x1, y1, x2, y1, matrix->Color888(colRed, colGreen, colBlue));

                x1 = x2;
                y1 = y2;
            }
        }


    private:

    public:
        Shape() : Point() {}

        Shape(Adafruit_Protomatter* mtrix, int x, int y, int r) : Point(mtrix, x, y)
        {
            radius = r;
        }

        Shape(Adafruit_Protomatter* mtrix, int x, int y, int r, int sides) : Point(mtrix, x, y)
        {
            radius = r;

            setNoOfSides(sides);
        }

        Shape(Adafruit_Protomatter* mtrix, int x, int y, int r, int sides, int rot) : Point(mtrix, x, y)
        {
            radius = r;

            setNoOfSides(sides);
            setRotation(rot);
        }

        Shape(Adafruit_Protomatter* mtrix, int x, int y, int r, int sides, int rot, int red, int green, int blue) : Point(mtrix, x, y, red, green, blue)
        {
            radius = r;

            setNoOfSides(sides);
            setRotation(rot);
        }

        // ~Shape()
        // {
        //     delete(debugBoundary);
        // }        

        int getRadius()
        {
            return radius;
        }
        
        void setRotation(int rot)
        {
            rotation = rot;
            radiansRotation = getRadiansFromAngle(270 + rot);
        }

        void setDebugColour(int colR, int colG, int colB)
        {
            debugColRed = colR >> 3;
            debugColGreen = colG >> 3;
            debugColBlue = colB >> 3;
        }

        void Draw()
        {
            DrawShape(radiansRotation, noOfSides, radiansAngleForSides);
            // float currentAngle = radiansRotation;

            // int x1 = getX(currentAngle);
            // int y1 = getY(currentAngle);

            // int x2 = 0;
            // int y2 = 0;

            // for (int ns=1; ns<=noOfSides; ns++)
            // {
            //     currentAngle += radiansAngleForSides;

            //     x2 = getX(currentAngle);
            //     y2 = getY(currentAngle);

            //     SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

            //     x1 = x2;
            //     y1 = y2;
            // }

            if (SHOW_CENTRE == true)
            {
                matrix->writePixel(xPoint, yPoint, matrix->Color888(debugColRed, debugColGreen, debugColBlue));
                //std::cout << "Draw Centre" << std::endl;
            }

            if (SHOW_BOUNDARY == true)
            {
                matrix->drawCircle(xPoint, yPoint, radius, matrix->Color888(debugColRed, debugColGreen, debugColBlue));
            }
        }
};

