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
    for (int i = 0; i < pool.size(); i++) {
        LexiCaseFitnessMinP fitness = boost::static_pointer_cast<LexiCaseFitnessMin>(pool.at(i)->fitness);
        for (int j = 0; j < fitness->vektor.size(); j++){
            std::cout << " " << fitness->vektor[j];
        }
        std::cout << "\n";
    }

    IndividualP ind = select(pool, 1).at(0);
    LexiCaseFitnessMinP fitness = boost::static_pointer_cast<LexiCaseFitnessMin>(ind->fitness);
    std::cout << "odabrano: ";
    for (int j = 0; j < fitness->vektor.size(); j++){
        std::cout << " " << fitness->vektor[j];
    }
    std::cout << "\n permutation :";
    for (int j = 0; j < case_permutation.size(); j++){
        std::cout << " " << case_permutation[j];
    }
    std::cout << "\n";
    return ind;
}


std::vector<IndividualP> LexiCaseSelOp::selectMany(const std::vector<IndividualP>& pool, uint repeats) {
    return select(pool, repeats);
}

std::vector<IndividualP> LexiCaseSelOp::select(const std::vector<IndividualP>& pool, uint k) {
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
            if (alive.size() == k) break;
            LexiCaseFitnessMinP fitness = boost::static_pointer_cast<LexiCaseFitnessMin>(it->get()->fitness);
            if((abs(bestFitness->vektor[currentCase] - fitness->vektor[currentCase]) > distance)) {
                it = alive.erase(it);
            }
            else ++it;
        }

        // if there is only one alive individual return it
        if (alive.size() == k) return alive;
    }

    // after all case eliminations if there is more then one individual alive, return random individual
    while (alive.size() > k) {
        int index = state_->getRandomizer()->getRandomInteger(alive.size());
        alive.erase(alive.begin() + index);
    }

    return alive;
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
