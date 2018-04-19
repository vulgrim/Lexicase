#include <ecf/ECF_base.h>
#include "Lexicase.h"
#include <ecf/ECF.h>


Lexicase::Lexicase() {
    // define algorithm name
    name_ = "Lexicase";
    lexiCaseSelOp = static_cast<LexiCaseSelOpP> (new LexiCaseSelOp);
    selRandomOpP = static_cast<SelRandomOpP> (new SelRandomOp);
    selWorstOp = static_cast<SelWorstOpP> (new SelWorstOp);
}


void Lexicase::registerParameters(StateP state) {
    registerParameter(state, "distance", (voidP) new float(3.0), ECF::FLOAT, "distance");
}


bool Lexicase::initialize(StateP state) {
    voidP d = getParameterValue(state, "distance");
    distance = *((float*) d.get());
    lexiCaseSelOp->initialize(state);
    selRandomOpP->initialize(state);
    selWorstOp->initialize(state);
    return true;
}


bool Lexicase::advanceGeneration(StateP state, DemeP deme) {
    distance *= 1.25;
    lexiCaseSelOp->setDistance(distance);
    for(uint iIter = 0; iIter < deme->size(); iIter++) {

//        ECF_LOG(state, 5, "Individuals in tournament: ");

        std::vector<IndividualP> tournament; // = lexiCaseSelOp->selectMany(*deme, 3);
        for (uint i = 0; i < 3; ++i) {
//             select a random individual for the tournament
            tournament.push_back(selRandomOpP->select(*deme));
        }

        // select the worst from the tournament
        IndividualP worst = selWorstOp->select(tournament);
//        ECF_LOG(state, 5, "The worst from the tournament: " + worst->toString());

        // remove pointer to 'worst' individual from vector 'tournament'
        removeFrom(worst, tournament);

        // crossover the first two (remaining) individuals in the tournament
        mate(tournament[0], tournament[1], worst);

        // perform mutation on new individual
        mutate(worst);

        // create new fitness
        evaluate(worst);
//        ECF_LOG(state, 5, "New individual: " + worst->toString());
    }

    return true;
}