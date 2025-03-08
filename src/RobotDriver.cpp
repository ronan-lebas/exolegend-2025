#include "RobotDriver.h"
#include "gladiator.h"
#include "config.h"

RobotDriver::RobotDriver(Gladiator *gladiator) : gladiator(gladiator)
{
    squareSize = gladiator->maze->getSquareSize();
    targetReached = false;
    fastMode = false;
}

void RobotDriver::reset()
{
    gladiator->log("Call of reset function driver");
    targetReached = false;
}

void RobotDriver::run()
{
    // Get current position
    RobotData data = gladiator->robot->getData();
    // Get target position
    Position position = data.position;

    // gladiator->log("Current position: (%f, %f)", position.x, position.y);
    // gladiator->log("Target position: (%f, %f)", targetX, targetY);

    // Check if target is reached (within MARGIN_RATIO * Square size)
    targetReached = (abs(position.x - targetX) < MARGIN_RATIO * squareSize) && (abs(position.y - targetY) < MARGIN_RATIO * squareSize);

    if (!targetReached)
    {
        // Move towards target
        // First, calculate the angle to the target
        float dx = targetX - position.x;
        float dy = targetY - position.y;
        float theta = std::atan2(dy, dx);

        // Normalize theta to be within the interval [-pi, pi] most close to position.a
        while (theta - position.a > M_PI)
            theta -= 2 * M_PI;
        while (theta - position.a < -M_PI)
            theta += 2 * M_PI;

        float obj_a = position.a;

        // Normalize obj_a to be within the interval [-pi, pi]
        while (obj_a > M_PI)
            obj_a -= 2 * M_PI;
        while (obj_a < -M_PI)
            obj_a += 2 * M_PI;

        float diff = abs(theta - obj_a);
        // gladiator->log("Angle to target: %f", theta);
        // gladiator->log("Current angle: %f", obj_a);
        // gladiator->log("Diff angle: %f", diff);

        if (diff > ANGLE_MARGIN)
        {
            // Rotate towards target
            if (theta > position.a)
            {
                stop();
                rotateLeft();
                // gladiator->log("Rotate left");
            }
            else
            {
                stop();
                rotateRight();
                // gladiator->log("Rotate right");
            }
        }
        else
        {
            // Move forward toward the obj
            stop();
            forward();
        }
    }
}

void RobotDriver::goTo(float x, float y)
{
    targetX = x;
    targetY = y;
    targetReached = false;
    fastMode = false;
}

void RobotDriver::goToMax(float x, float y)
{
    targetX = x;
    targetY = y;
    targetReached = false;
    fastMode = true;
}

void RobotDriver::forward()
{
    if (fastMode)
    {
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, FAST_FORWARD_SPEED);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, FAST_FORWARD_SPEED);
    }
    else
    {
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, FORWARD_SPEED);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, FORWARD_SPEED);
    }
}

void RobotDriver::rotateLeft()
{
    if (fastMode)
    {
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, -FAST_ROTATE_SPEED);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, FAST_ROTATE_SPEED);
    }
    else
    {
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, -ROTATE_SPEED);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, ROTATE_SPEED);
    }
}

void RobotDriver::rotateRight()
{
    if (fastMode)
    {
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, FAST_ROTATE_SPEED);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, -FAST_ROTATE_SPEED);
    }
    else
    {
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, ROTATE_SPEED);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, -ROTATE_SPEED);
    }
}

void RobotDriver::forwardLeft()
{
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, FORWARD_SPEED);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0);
}

void RobotDriver::forwardRight()
{
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, FORWARD_SPEED);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0);
}

void RobotDriver::stop()
{
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0);
}