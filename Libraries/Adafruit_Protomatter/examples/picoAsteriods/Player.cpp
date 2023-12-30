#include <iostream>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <Adafruit_Protomatter.h>
#include <vector>
#include <random>

#include "Constants.cpp"
#include "Point.cpp"
#include "Asteroid.cpp"
#include "SDL_Point.cpp"
#include "Vertex.cpp"
// using namespace std;

class Player : public Point
{
protected:
    int radius = 1;
    int rotation = 0;
    float radiansRotation = 0;

private:
    float degreesRotationAngle = 0;

    int dXv = 0.0; // Direction Horizontal velocity 1 = Right, -1 = Left
    int dYv = 0.0; // Direction Vertical velocity 1 = Down, -1 = Up

    float vX = 0; // Horizonal velocity per frame
    float vY = 0; // Vertical velocity per frame

    vertex position;
    vertex velocity;
    vertex angleAcceleration;

    bool thrusting = false;
    bool dead = false;
    int explodeTime = 0;
    int blinkTime = 0;
    int blinkNumber = 0;
    int blinkOn = 0;
    int invincibilty = false;
    bool canShoot = true;

    // SP175 Contribution
    bool rotating = false;
    bool shooting = false;
    int rotationDir = 0;        // -1 - Left, 1 = Right
    float playerFireRate = 0.7; // how fast we can fire
    int playerFireDelay = 20;   // delay before next bullet fires
    // int shootTimer = playerFireDelay / playerFireRate;
    float shootTimer = 0;

    std::vector<SDL_Point> shipPoints;
    std::vector<SDL_Point> thrustPoints;

    int debugColRed = 255 >> 3;
    int debugColGreen = 165 >> 3;
    int debugColBlue = 0;

    // Line shipExplosionLines[4] = {};

    int getXPositive(float angleInRadians, float proportion)
    {
        return xPoint + (radius * proportion * cos(angleInRadians));
    }

    int getYPositive(float angleInRadians, float proportion)
    {
        return yPoint + (radius * proportion * sin(angleInRadians));
    }

    int getXNegative(float angleInRadians, float proportion)
    {
        return xPoint - (radius * proportion * cos(angleInRadians));
    }

    int getYNegative(float angleInRadians, float proportion)
    {
        return yPoint - (radius * proportion * sin(angleInRadians));
    }

    float getRadiansFromAngle(float angle)
    {
        // angles is in degrees, return is in radians
        // return static_cast< float >(angle) / 180 * M_PI;
        return angle / 180.0 * M_PI;
    }

    void DrawShape(float radiansRot, int sides, float radiansAngle)
    {
        float currentAngle = radiansRot;

        int x1 = getXPositive(currentAngle, 1);
        int y1 = getYPositive(currentAngle, 1);

        int x2 = 0;
        int y2 = 0;

        for (int ns = 1; ns <= sides; ns++)
        {
            currentAngle += radiansAngle;

            x2 = getXPositive(currentAngle, 1);
            y2 = getYPositive(currentAngle, 1);

            matrix->drawLine(x1, y1, x2, y2, matrix->Color888(colRed, colGreen, colBlue));

            x1 = x2;
            y1 = y2;
        }
    }

public:
    //std::vector<Laser> Lasers; // https://www.geeksforgeeks.org/queue-cpp-stl/

    Player() {}

    Player(Adafruit_Protomatter* mtrix) : Point(mtrix, WIDTH / 2, HEIGHT / 2)
    {
        NewShip();
        // shipPoints = new SDL_Point[4];
        // thrustPoints = new SDL_Point[4];
        shipPoints.resize(4);
        thrustPoints.resize(4);
    }
    // Player(Adafruit_Protomatter* mtrix, Mix_Chunk *hitRock) : Point(mtrix, WIDTH / 2, HEIGHT / 2)
    // {
    //     NewShip();
    //     // shipPoints = new SDL_Point[4];
    //     // thrustPoints = new SDL_Point[4];
    //     shipPoints.resize(4);
    //     thrustPoints.resize(4);
    // }
    int getRotation()
    {
        return rotation;
    }

    int getXPoint()
    {
        return xPoint;
    }
    int getYPoint()
    {
        return yPoint;
    }

    int getExplodeTime()
    {
        return explodeTime;
    }

    int getRadius()
    {
        return radius;
    }
    
    void LivesShip(int x, int y)
    {
        xPoint = x;
        yPoint = y;
        setRotation(90);
    }

    void explodeShip()
    {
        // if we have a collision but the explosion timer is not set yet then
        // initialise the velocities for each segment to explode
        int extraForce, varyAngle = 0;

        if (isPlayerDead() && explodeTime == 0)
        {
            // for (int side = 0; side < 4; ++side)
            // {
            //     std::random_device rd;      // Will be used to obtain a seed for the random number engine
            //     std::mt19937 gen(rd());     // Standard mersenne_twister_engine seeded with rd()
            //     std::uniform_int_distribution<> extraForce(-10,40);  // extra explosion strength to add
            //     std::uniform_int_distribution<> varyAngle(-30,30);   // angle to add to explosion line

            //     // std::cout<<"extraForce: ["<<side<<"] "<<extraForce(gen)<<std::endl;
            //     // std::cout<<"angleVary: ["<<side<<"] "<<varyAngle(gen)<<std::endl;

            //     switch (side)
            //     {
            //     case 0:
            //         shipExplosionLines[side] = {matrix, shipPoints[0].x, shipPoints[0].y, shipPoints[1].x, shipPoints[1].y};
            //         shipExplosionLines[side].setDriftDirection(SHIP_EXPLOSION_VEL+extraForce(gen), SHIP_EXPLOSION_VEL+extraForce(gen), (70 + varyAngle(gen) + rotation), velocity.x, velocity.y);
            //         break;
            //     case 1:
            //         shipExplosionLines[side] = {matrix, shipPoints[1].x, shipPoints[1].y, shipPoints[2].x, shipPoints[2].y};
            //         shipExplosionLines[side].setDriftDirection(SHIP_EXPLOSION_VEL+extraForce(gen), SHIP_EXPLOSION_VEL+extraForce(gen), (197 + varyAngle(gen) + rotation), velocity.x, velocity.y);
            //         break;
            //     case 2:
            //         shipExplosionLines[side] = {matrix, shipPoints[2].x, shipPoints[2].y, shipPoints[3].x, shipPoints[3].y};
            //         shipExplosionLines[side].setDriftDirection(SHIP_EXPLOSION_VEL+extraForce(gen), SHIP_EXPLOSION_VEL+extraForce(gen), (163 + varyAngle(gen) + rotation), velocity.x, velocity.y);
            //         break;
            //     case 3:
            //         shipExplosionLines[side] = {matrix, shipPoints[3].x, shipPoints[3].y, shipPoints[0].x, shipPoints[0].y};
            //         shipExplosionLines[side].setDriftDirection(SHIP_EXPLOSION_VEL+extraForce(gen), SHIP_EXPLOSION_VEL+extraForce(gen), (287 + varyAngle(gen) + rotation), velocity.x, velocity.y);
            //         break;
            //     }

            //     shipExplosionLines[side].setRotation();

            //     shipExplosionLines[side].SetColour(255, 136, 0);
            //     shipExplosionLines[side].Draw();
            // }
        }

        setExplodeTime(SHIP_EXPLOSION_DUR * FPS);
    }

    void setDebugColour(int colR, int colG, int colB)
    {
        debugColRed = colR >> 3;
        debugColGreen = colG >> 3;
        debugColBlue = colB >> 3;
    }

    void thrustOn()
    {
        thrusting = true;
    }

    void thrustOff()
    {
        thrusting = false;
    }

    void rotateLeft()
    {
        int rot = rotation + (SHIP_TURN_SPD / FPS);
        setRotation(rot);
    }

    void rotateRight()
    {
        int rot = rotation - (SHIP_TURN_SPD / FPS);
        setRotation(rot);
    }

    void setInvincibility()
    {
        invincibilty = true;
        blinkTime = SHIP_BLINK_DUR * FPS;
        blinkNumber = SHIP_INV_DUR / SHIP_BLINK_DUR;
        blinkOn = 0;
    }
    bool getInvincibility()
    {
        return invincibilty;
    }

    void NewShip()
    {
        xPoint = WIDTH / 2;
        yPoint = HEIGHT / 2;
        radius = SHIP_SIZE / 2;
        setRotation(90);
        thrusting = false;

        position.x = xPoint;
        position.y = yPoint;

        velocity.x = 0;
        velocity.y = 0;

        angleAcceleration.x = 0;
        angleAcceleration.y = 0;

        dead = false;
        explodeTime = 0;
        // blinkTime = SHIP_BLINK_DUR * FPS;
        // blinkNumber = SHIP_INV_DUR / SHIP_BLINK_DUR;
        // blinkOn = 0;
        setInvincibility();
        canShoot = true;
        SetColour(0,0,255<<3);

        //Lasers.clear();
    }

    // void shootLaser(Mix_Chunk *sound)
    // {
    //     if (canShoot && Lasers.size() <= LASER_MAX)
    //     {
    //         int lX = xPoint + ((4 / 3) * radius) * cos(radiansRotation);
    //         int lY = yPoint - ((4 / 3) * radius) * sin(radiansRotation);

    //         float vX = (LASER_SPD * cos(radiansRotation) / FPS) + velocity.x;
    //         float vY = -((LASER_SPD * sin(radiansRotation) / FPS) + velocity.y);

    //         Laser newLaser(renderer, lX, lY, vX, vY);
    //         Lasers.push_back(newLaser);
    //         Mix_PlayChannel(2, sound, 0);
    //     }
    // }

    void setRotation(int rot)
    {
        if (rot < 0)
        {
            rot = 360 + rot;
        }
        else if (rot > 360)
        {
            rot = rot - 360;
        }

        rotation = rot;
        radiansRotation = getRadiansFromAngle(rot);

        // std::cout << rotation << std::endl;
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

        if (explodeTime == 0 && blinkOn == 0)
        {
            if (thrusting)
            {
                // Thrust Top Point
                thrustPoints[0].x = xPoint - (radius * (8 / 3.0)) * cos(radiansRotation);
                thrustPoints[0].y = yPoint + (radius * (8 / 3.0)) * sin(radiansRotation);

                // Thrust Bottom Left Point
                thrustPoints[1].x = xPoint - (radius * ((4 / 3.0) * cos(radiansRotation) + 0.5 * sin(radiansRotation)));
                thrustPoints[1].y = yPoint + (radius * ((4 / 3.0) * sin(radiansRotation) - 0.5 * cos(radiansRotation)));

                // Thrust Bottom Middle Point
                thrustPoints[2].x = xPoint - (radius * (2 / 3.0)) * cos(radiansRotation);
                thrustPoints[2].y = yPoint + (radius * (2 / 3.0)) * sin(radiansRotation);

                // Thrust Bottom Right Point
                thrustPoints[3].x = xPoint - (radius * ((4 / 3.0) * cos(radiansRotation) - 0.5 * sin(radiansRotation)));
                thrustPoints[3].y = yPoint + (radius * ((4 / 3.0) * sin(radiansRotation) + 0.5 * cos(radiansRotation)));

                thrustPoints[4] = thrustPoints[0];

                for (int ns=0; ns<3; ns++)
                {
                  matrix->drawLine(thrustPoints[ns].x, thrustPoints[ns].y, thrustPoints[ns+1].x, thrustPoints[ns+1].y, matrix->Color888(255 >> 3, 0, 0));
                }
            }
        }

        // SDL_Point* points = new SDL_Point[4];

        // Ship Top Point
        shipPoints[0].x = xPoint + (radius * (4 / 3.0)) * cos(radiansRotation);
        shipPoints[0].y = yPoint - (radius * (4 / 3.0)) * sin(radiansRotation);

        // Ship Bottom Left Point
        shipPoints[1].x = xPoint - (radius * ((4 / 3.0) * cos(radiansRotation) + sin(radiansRotation)));
        shipPoints[1].y = yPoint + (radius * ((4 / 3.0) * sin(radiansRotation) - cos(radiansRotation)));

        // Ship Bottom Middle Point
        shipPoints[2].x = xPoint - (radius * (2 / 3.0)) * cos(radiansRotation);
        shipPoints[2].y = yPoint + (radius * (2 / 3.0)) * sin(radiansRotation);

        // Ship Bottom Right Point
        shipPoints[3].x = xPoint - (radius * ((4 / 3.0) * cos(radiansRotation) - sin(radiansRotation)));
        shipPoints[3].y = yPoint + (radius * ((4 / 3.0) * sin(radiansRotation) + cos(radiansRotation)));

        shipPoints[4] = shipPoints[0];

        if (explodeTime == 0)
        {
            if (blinkOn == 0)
            {
                for (int ns=0; ns<4; ns++)
                {
                  matrix->drawLine(shipPoints[ns].x, shipPoints[ns].y, shipPoints[ns+1].x, shipPoints[ns+1].y, matrix->Color888(colRed, colGreen, colBlue));
                }
            }
        }
        else if (isPlayerDead())
        {
            // for (auto side = 0; side < 4; ++side)
            // {
            //     shipExplosionLines[side].Draw();
            // }
        }

        // for (auto i = Lasers.begin(); i != Lasers.end(); ++i)
        // {
        //     i->Draw();
        //     // i->Update();
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

    //void Update(Mix_Chunk *shoot, Mix_Chunk *Thrust)
    void Update()
    {

        blinkOn = blinkNumber % 2;

        if (blinkNumber > 0)
        {
            blinkTime--;
            if (blinkTime == 0)
            {
                blinkTime = SHIP_BLINK_DUR * FPS;

                blinkNumber--;
            }
            // Text SC1(renderer, GAMEFONT, 40, WIDTH / 2 - 80, HEIGHT / 2, "G E T   R E A D Y");
            //SC1.setColor(255, 255, 255);
            //SC1.Draw();
            // std::string SC = std::to_string(int(blinkNumber / 6));
            // Text SC2(renderer, GAMEFONT, 360 - (((blinkNumber % 6) * 6) * 10), WIDTH / 2 + 100, HEIGHT / 2, SC);
            //SC2.setColor(255, 255, 255);
            //SC2.Draw();
            invincibilty = true;
        }
        else
            invincibilty = false;

        if (thrusting)
        {
            // if (!Mix_Playing(1))
            // {
            //     Mix_PlayChannel(1, Thrust, 0);
            // }
            // std::cout << "Thrusting" << std::endl;
            velocity.x += (SHIP_THRUST * cos(radiansRotation)) / FPS;
            velocity.y += (SHIP_THRUST * sin(radiansRotation)) / FPS;
            // std::cout << velocity.x << std::endl;

            if (velocity.x > SHIP_MAX_SPEED)
            {
                velocity.x = SHIP_MAX_SPEED;
            }

            if (velocity.y > SHIP_MAX_SPEED)
            {
                velocity.y = SHIP_MAX_SPEED;
            }
        }
        else
        {
            // Mix_HaltChannel(1);
            // std::cout << "Not Thrusting" << std::endl;
            velocity.x -= (FRICTION * velocity.x) / FPS;
            velocity.y -= (FRICTION * velocity.y) / FPS;
        }

        if (explodeTime == 0)
        {
            xPoint += velocity.x;
            yPoint -= velocity.y;
        }
        else
        {
            disablePlayerShooting(true);
            explodeTime--;
            if (explodeTime == 0)
            {
                NewShip();
            }

            if (isPlayerDead())
            {
                // for (int side = 0; side < 4; ++side)
                // {
                //     shipExplosionLines[side].Update();
                // }
            }
        }

        if (xPoint < -radius)
        {
            xPoint = WIDTH + radius;
        }
        else if (xPoint > WIDTH + radius)
        {
            xPoint = -radius;
        }

        if (yPoint < -radius)
        {
            yPoint = HEIGHT + radius;
        }
        else if (yPoint > HEIGHT + radius)
        {
            yPoint = -radius;
        }

        // for (auto i = Lasers.begin(); i != Lasers.end(); ++i)
        // {
        //     // i->Draw();
        //     i->Update();
        // }

        // Ref..... https://stackoverflow.com/questions/4713131/removing-item-from-vector-while-iterating
        // std::vector<Laser>::iterator it = Lasers.begin();
        // while (it != Lasers.end())
        // {
        //     if (it->AmIDead())
        //     {
        //         it = Lasers.erase(it);
        //     }
        //     else
        //         ++it;
        // }

        if (rotating)
        {
            if (rotationDir < 0)
            {
                rotateLeft();
            }
            else
                rotateRight();
        }

        if (shooting)
        {
            if (shootTimer > 0)
            {
                --shootTimer;
                canShoot = false;
            }
            else
            {
                canShoot = true;
                shootTimer = playerFireDelay / playerFireRate;
            }

            //shootLaser(shoot);
        }
    }

    // ----------------------------------------------------------------------------------------------------------
    // SP175 Contribution
    void setRotationDIR(int dir)
    {
        switch (dir)
        {
        case -1: // rotating Left
            rotating = true;
            rotationDir = -1;
            break;
        case 1: // rotating Right
            rotating = true;
            rotationDir = 1;
            break;
        default: // not rotating
            rotating = false;
            rotationDir = 0;
            break;
        }
    }

    void setShooting(bool isPlayerShooting)
    {
        shooting = isPlayerShooting;
        if (!isPlayerShooting)
        {
            shootTimer = 0;
        }
    }

    void setPlayerDead(bool playerDead)
    {
        dead = playerDead;
    }

    bool isPlayerDead()
    {
        return dead;
    }

    void setExplodeTime(int duration)
    {
        explodeTime = duration;
    }

    void disablePlayerShooting(bool disableShot)
    {
        canShoot = !disableShot;
    }

    // ----------------------------------------------------------------------------------------------------------

    double distanceBetweenObjects(Asteroid *theObject)
    {
        float xDiff = xPoint - theObject->getXPoint();
        float yDiff = yPoint - theObject->getYPoint();

        double distance = sqrt((xDiff * xDiff) + (yDiff * yDiff));
        // std::cout << distance << std::endl;
        return distance;
    }
};
