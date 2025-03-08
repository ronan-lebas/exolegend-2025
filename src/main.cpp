#include "Runner.h"
#include "gladiator.h"

#include "gladiator.h"
Gladiator *gladiator;
Runner *runner;

void reset();
void setup()
{
    // instanciation de l'objet gladiator
    gladiator = new Gladiator();
    runner = new Runner(gladiator);
    // enregistrement de la fonction de reset qui s'éxecute à chaque fois avant qu'une partie commence
    gladiator->game->onReset(&reset); // GFA 4.4.1
}

void reset()
{
    // fonction de reset:
    // initialisation de toutes vos variables avant le début d'un match
    gladiator->log("Call of reset function main"); // GFA 4.5.1
    runner->reset();
}

void loop()
{
    if (gladiator->game->isStarted())
    {   
        runner->instructions();
        while (true)
        {
            runner->run();
            delay(50);
        }
    }
}