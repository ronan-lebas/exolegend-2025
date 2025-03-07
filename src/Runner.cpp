#include "Runner.h"

Runner::Runner(Gladiator *gladiator) : controller(gladiator), gladiator(gladiator) {
    // Initialization
    controller.goTo(6, 6);
}

void Runner::run() {
    // Example usage
    controller.run();
}