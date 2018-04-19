#include <cmath>
#include <ecf/ECF.h>
#include "SymbRegEvalOp.h"
#include "LexiCaseFitnessMin.h"

// u lexicase algoritmu: provjera odgovarajuceg fitnes objekta:
//if(!boost::dynamic_pointer_cast<LexiCaseFitnessMinP> (fitness))


// called only once, before the evolution � generates training data
bool SymbRegEvalOp::initialize(StateP state)
{
    nSamples = 10;
    double x = -10;
    for(uint i = 0; i < nSamples; i++) {
        domain.push_back(x);
        codomain.push_back(x + sin(x) * cos(x));
        x += 2;
    }
    return true;
}


FitnessP SymbRegEvalOp::evaluate(IndividualP individual) {
    LexiCaseFitnessMinP fitness (new LexiCaseFitnessMin);

    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();
    // (you can also use boost smart pointers:)
    //TreeP tree = boost::static_pointer_cast<Tree::Tree> (individual->getGenotype());

    for(uint i = 0; i < nSamples; i++) {
        // for each test data instance, the x value (domain) must be set
        tree->setTerminalValue("X", &domain[i]);
        // get the y value of the current tree
        double result;
        tree->execute(&result);
        fitness->vektor.push_back(fabs(codomain[i] - result));
    }

    return fitness;
}
