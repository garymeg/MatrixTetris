#line 1 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\picoAsteriods\\Asteroid.cpp"
#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <Adafruit_Protomatter.h>
#include <vector>

#include "Shape.cpp"
#include "Constants.cpp"
#include "SDL_Point.cpp"

class Asteroid: public Shape
{
    protected:

    private:
        float degreesRotationAngle = 0;

        int dXv = 0.0;          // Direction Horizontal velocity 1 = Right, -1 = Left
        int dYv = 0.0;          // Direction Vertical velocity 1 = Down, -1 = Up

        float vX = 0;           // Horizonal velocity per frame 
        float vY = 0;           // Vertical velocity per frame

        float radiusOffsets[ROID_VERT*2];
        std::vector<SDL_Point> points;
        //SDL_Point* points;

        int getX(float angleInRadians, int offsetIndex)
        {
            return xPoint + (radius * radiusOffsets[offsetIndex] * cos(angleInRadians));
        }

        int getY(float angleInRadians, int offsetIndex)
        {
            return yPoint + (radius * radiusOffsets[offsetIndex] * sin(angleInRadians));
        }

    public:
        Asteroid() { }

        Asteroid(Adafruit_Protomatter* mtrix, int x, int y, int r, int level) : Shape(mtrix, x, y, r)
        {
            if ((rand() % 10) > 5)
            {
                dXv = 1;
            }
            else
            {
                dXv = -1;
            }

            if ((rand() % 10) > 5)
            {
                dYv = 1;
            }
            else
            {
                dYv = -1;
            }

            // std::cout << dXv << std::endl;
            // std::cout << dYv << std::endl;

            float lvlMultipler = 1 + (0.5 * level);
            int levelSpeed = (int)(ROID_SPD * lvlMultipler);
            vX = ((rand() % levelSpeed) / FPS) * (float)dXv;
            // std::cout << vX << std::endl;
            vY = ((rand() % levelSpeed) / FPS) * (float)dYv;
            // std::cout << vY << std::endl;

            radius = r;
            setNoOfSides((rand() % (ROID_VERT+1)) + (ROID_VERT / 2));
            //points = SDL_Point[noOfSides + 1];
            points.resize(noOfSides + 1);

            setRotation(0);
            degreesRotationAngle = (rand() % 30 + 20) / FPS;

            for (int offset=0;offset<=noOfSides;offset++)
            {
                // gives us a jagged offset of between 40% -> 100%
                //radiusOffsets[offset] = (rand() % (ROID_JAG) + (100 - ROID_JAG)) / 100.0;
                radiusOffsets[offset] = (rand() % (ROID_JAG) + (130 - ROID_JAG)) / 100.0;
                //radiusOffsets[offset] = 1;
            }
        }

        void setRotation(int rot)
        {
            rotation = rot;
            radiansRotation = getRadiansFromAngle(270 + rot);
        }

        // vertex getCentrePoint()
        // {
        //     vertex vCentre;
        //     vCentre.x = xPoint;
        //     vCentre.y = yPoint;

        //     return vCentre;
        // }

        void Draw()
        {
          Serial.println("Draw");
            radiansRotation += getRadiansFromAngle(degreesRotationAngle);

            float currentAngle = radiansRotation;

            for (int ns=0; ns<noOfSides; ns++)
            {
                points[ns].x = getX(currentAngle, ns);
                points[ns].y = getY(currentAngle, ns);

                currentAngle += radiansAngleForSides;
            }

            points[noOfSides] = points[0];

            for (int ns=0; ns<noOfSides; ns++)
            {
              matrix->drawLine(points[ns].x, points[ns].y, points[ns+1].x, points[ns+1].y, matrix->Color888(colRed, colGreen, colBlue));
            }

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

        void Update()
        {
            xPoint += vX;
            yPoint += vY;

            //std::cout << xPoint << std::endl;

            if (xPoint < -radius)
            {
                xPoint = WIDTH + radius;
            } else if (xPoint > WIDTH + radius)
            {
                xPoint = -radius;
            }

            if (yPoint < -radius)
            {
                yPoint = HEIGHT + radius;
            } else if (yPoint > HEIGHT + radius)
            {
                yPoint = -radius;
            }
        }
};
