#include "RobotDriver.h"
#include "gladiator.h"
#include "config.h"

RobotDriver::RobotDriver(Gladiator *gladiator) : gladiator(gladiator) {
    squareSize = gladiator->maze->getSquareSize();
}

void RobotDriver::run() {
    // Get current position
    RobotData data = gladiator->robot->getData();
    // Get target position
    Position position = data.position;

    // Check if target is reached (within MARGIN_RATIO * Square size)
    targetReached = (abs(position.x - targetX) < MARGIN_RATIO * squareSize) && (abs(position.y - targetY) < MARGIN_RATIO * squareSize);

    if (!targetReached) {
        // Move towards target
        // First, calculate the angle to the target
        float dx = targetX - position.x;
        float dy = targetY - position.y;
        float theta = std::atan2(dy, dx);

        float angleReached = abs(theta - position.a) < ANGLE_MARGIN;
        if (!angleReached) {
            // Rotate towards target
            if (theta > position.a) {
                rotateRight();
            } else {
                rotateLeft();
            }
        } else {
            // Move forward toward the obj
            forward();
        }
    }

    gladiator->log("Running driver");
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
