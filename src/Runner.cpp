#include "Runner.h"

Runner::Runner(Gladiator *gladiator) : controller(gladiator), gladiator(gladiator) {
    // Initialization
}

void Runner::instructions() {
    // Example usage
    controller.goTo(11, 1);
}

void Runner::run() {
    // Example usage
    controller.run();
}