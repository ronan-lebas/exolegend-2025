#include "RobotDriver.h"
#include "gladiator.h"
#include "config.h"

RobotDriver::RobotDriver(Gladiator *gladiator) : gladiator(gladiator) {
    squareSize = gladiator->maze->getSquareSize();
    targetReached = false;
}

void RobotDriver::run() {
    // Get current position
    RobotData data = gladiator->robot->getData();
    // Get target position
    Position position = data.position;

    //gladiator->log("Current position: (%f, %f)", position.x, position.y);
    //gladiator->log("Target position: (%f, %f)", targetX, targetY);

    // Check if target is reached (within MARGIN_RATIO * Square size)
    targetReached = (abs(position.x - targetX) < MARGIN_RATIO * squareSize) && (abs(position.y - targetY) < MARGIN_RATIO * squareSize);

    if (!targetReached) {
        // Move towards target
        // First, calculate the angle to the target
        float dx = targetX - position.x;
        float dy = targetY - position.y;
        float theta = std::atan2(dy, dx);

        //gladiator->log("Angle to target: %f", theta);
        //gladiator->log("Current angle: %f", position.a);
        //gladiator->log("Diff angle: %f", abs(theta - position.a));
        float angleReached = abs(theta - position.a) < ANGLE_MARGIN;
        if (!angleReached) {
            // Rotate towards target
            if (theta > position.a) {
                stop();
                rotateLeft();
            } else {
                stop();
                rotateRight();
            }
        } else {
            // Move forward toward the obj
            stop();
            forward();
        }
    }
}

void RobotDriver::goTo(float x, float y) {
    targetX = x;
    targetY = y;
    targetReached = false;
}

void RobotDriver::forward() {
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, FORWARD_SPEED);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, FORWARD_SPEED);
}

void RobotDriver::rotateLeft() {
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, -ROTATE_SPEED);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, ROTATE_SPEED);
}

void RobotDriver::rotateRight() {
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, ROTATE_SPEED);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, -ROTATE_SPEED);
}

void RobotDriver::stop() {
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0);
}