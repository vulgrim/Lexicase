//
// Created by kristijan on 17/04/18.
//

#include "ecf/ECF_base.h"
#include "LexiCaseSelOp.h"
#include "LexiCaseFitnessMin.h"
#include <cmath>


bool LexiCaseSelOp::initialize(StateP state) {
    state_ = state;
    selRandomOpP = static_cast<SelRandomOpP> (new SelRandomOp);
    selBestOpP = static_cast<LexiCaseSelBestOpP> (new LexiCaseSelBestOp);

    selRandomOpP->initialize(state);
    selBestOpP->initialize(state);
    return true;
}

IndividualP LexiCaseSelOp::select(const std::vector<IndividualP>& pool) {
    if (pool.empty()) return IndividualP();
    initCases(pool.at(0));   // TODO

    std::vector<IndividualP> alive = pool;
    std::vector<IndividualP>::iterator it;

    for (int i = 0; i < case_permutation.size(); i++) {
        int currentCase = case_permutation[i];

        // find best individual for current case
        IndividualP caseBest = selBestOpP->select(alive, currentCase);
        LexiCaseFitnessMinP bestFitness = boost::static_pointer_cast<LexiCaseFitnessMin>(caseBest->fitness);

        // terminate individuals with case fitness > (case_best_fitness +- distance)
        it = alive.begin();
        while(it != alive.end()) {
            LexiCaseFitnessMinP fitness = boost::static_pointer_cast<LexiCaseFitnessMin>(it->get()->fitness);
            if((abs(bestFitness->vektor[currentCase] - fitness->vektor[currentCase]) > distance)) {
                it = alive.erase(it);
            }
            else ++it;
        }

        // if there is only one alive individual return it
        if (alive.size() == 1) { return alive.at(0); }
    }

    // after all case eliminations if there is more then one individual alive, return random individual
    return selRandomOpP->select(alive);
}


std::vector<IndividualP> LexiCaseSelOp::selectMany(const std::vector<IndividualP>& pool, uint repeats) {
    if (pool.empty()) return pool;
    std::vector<IndividualP> selected;
    for (int i = 0; i < repeats; i++) selected.push_back(select(pool));
    return selected;
}


void LexiCaseSelOp::initCases(IndividualP ind) {
    if (case_permutation.empty()) {
        LexiCaseFitnessMinP fitness = boost::static_pointer_cast<LexiCaseFitnessMin>(ind->fitness);
        uint nSamples = fitness->vektor.size();
        for (int i = 0; i < nSamples; i++) case_permutation.push_back(i);
    }
    std::random_shuffle(case_permutation.begin(), case_permutation.end());
}


bool LexiCaseSelOp::setDistance(float d) {
    this->distance = d;
}
