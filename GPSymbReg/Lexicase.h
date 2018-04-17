//
// Created by kristijan on 08/04/18.
//

#ifndef LEXICASE_H
#define LEXICASE_H

#include <boost/shared_ptr.hpp>
#include <ecf/SelBestOp.h>
#include <ecf/SelRandomOp.h>
#include <ecf/SelFitnessProportionalOp.h>
#include "ecf/Algorithm.h"
#include "ecf/SelRandomOp.h"
#include "ecf/SelBestOp.h"
#include "SymbRegEvalOp.h"
#include "LexiCaseFitnessMin.h"
#include "LexiCaseSelBestOp.h"
#include "LexiCaseSelOp.h"
#include "ecf/SelWorstOp.h"

class Lexicase : public Algorithm {
public:
    Lexicase();
    bool advanceGeneration(StateP state, DemeP deme);
    bool initialize(StateP state);
    void registerParameters(StateP state);

protected:
    SelWorstOpP selWorstOp;
    LexiCaseSelOpP lexiCaseSelOp;
    float distance;
    SelRandomOpP selRandomOpP;

};

typedef boost::shared_ptr<Lexicase> LexicaseP;

#endif //LEXICASE_H
