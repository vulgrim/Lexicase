//
// Created by kristijan on 08/04/18.
//

#include <ecf/ECF_base.h>
#include "Lexicase.h"
#include <ecf/ECF.h>
#include "LexiCaseFitnessMin.h"
#include "LexiCaseSelBestOp.h"

Lexicase::Lexicase() {
    // define algorithm name
    name_ = "Lexicase";
    selRandomOpP = static_cast<SelRandomOpP> (new SelRandomOp);
    selBestOpP = static_cast<LexiCaseSelBestOpP> (new LexiCaseSelBestOp);
}


void Lexicase::registerParameters(StateP state) {
    registerParameter(state, "distance", (voidP) new float(3.0), ECF::FLOAT, "distance");
}


bool Lexicase::initialize(StateP state) {
    voidP d = getParameterValue(state, "distance");
    distance = *((float*) d.get());

    selRandomOpP->initialize(state);
    selBestOpP->initialize(state);

    return true;
}


bool Lexicase::advanceGeneration(StateP state, DemeP deme) {
    IndividualP best = select(*deme);
    best = copy(best);

    // select individuals
    std::vector<IndividualP> wheel;
    wheel = selectMany(*deme, (uint) deme->size());

    // copy selected to new population
    for(uint i = 0; i < wheel.size(); ++i)
        wheel[i] = copy(wheel[i]);

    // replace old population
    for(uint i = 0; i < deme->size(); i++)
        replaceWith((*deme)[i], wheel[i]);

//    ECF_LOG(state, 5, "Selected individuals:");
//    for(uint i = 0; i < deme->size(); i++){
//        ECF_LOG(state, 5, dbl2str(deme->at(i)->fitness->getValue()));
//    }

    // determine the number of crx operations
    uint noCrx = (int)(deme->size() * 0.5 /2);  // TODO crxRate_

    // perform crossover
    for(uint i = 0; i < noCrx; i++){

        // select parents
        IndividualP parent1 = selRandomOpP->select(*deme);
        IndividualP parent2 = selRandomOpP->select(*deme);
//        ECF_LOG(state, 5, "Parents: " + dbl2str(parent1->fitness->getValue()) + ", " + dbl2str(parent2->fitness->getValue()));

        // create children
        IndividualP child1 = copy(parent1);
        IndividualP child2 = copy(parent2);

        // perform crx operations
        mate(parent1, parent2, child1);
        mate(parent1, parent2, child2);

        // replace parents with children
        replaceWith(parent1, child1);
        replaceWith(parent2, child2);
    }

    // perform mutation on whole population
    mutate(*deme);

    // evaluate new individuals
    for(uint i = 0; i < deme->size(); i++)
        if(!deme->at(i)->fitness->isValid()) {
            evaluate(deme->at(i));
        }

    // elitism: preserve best individual
    IndividualP random = selRandomOpP->select(*deme);
    if(best->fitness->isBetterThan(random->fitness))
        replaceWith(random, best);

    return true;
}


IndividualP Lexicase::select(const std::vector<IndividualP>& population) {
    initCases(population.at(0));   // TODO

    std::vector<IndividualP> alive;
    for (int i = 0; i < population.size(); i++) { alive.push_back(population.at(i)); }  // TODO there's got to be a better way

    std::vector<int> terminate;

    for (int i = 0; i < case_permutation.size(); i++) {
        int currentCase = case_permutation[i];

        // find best individual for current case
        IndividualP caseBest = selBestOpP->select(alive, currentCase);
        LexiCaseFitnessMinP bestFitness = boost::static_pointer_cast<LexiCaseFitnessMin>(caseBest->fitness);

        // terminate individuals with case fitness > (case_best_fitness +- distance)
        terminate.clear();
        for (int j = 0; j < alive.size(); j++){
            if (caseBest->index == alive.at(j)->index) { continue; }
            LexiCaseFitnessMinP fitness = boost::static_pointer_cast<LexiCaseFitnessMin>(alive.at(j)->fitness);
            if (abs(bestFitness->vektor[currentCase] - fitness->vektor[currentCase]) > distance) {
                terminate.push_back(j);
            }
        }

        std::sort(terminate.begin(), terminate.end(), std::greater<int>());
        for (int j = 0; j < terminate.size(); j++) {
            alive.erase(alive.begin() + terminate[j]);
        }

        // if there is only one alive individual return it
        if (alive.size() == 1) { return alive.at(0); }
    }

    // after all case eliminations if there is more then one individual alive, return random individual
    return selRandomOpP->select(alive);

}

std::vector<IndividualP> Lexicase::selectMany(const std::vector<IndividualP>& population, int k) {
    if (population.size() == k) return population;
    initCases(population.at(0));   // TODO

    std::vector<IndividualP> alive;
    for (int i = 0; i < population.size(); i++) { alive.push_back(population.at(i)); }  // TODO there's got to be a better way

    std::vector<int> terminate;

    for (int i = 0; i < case_permutation.size(); i++) {
        int currentCase = case_permutation[i];

        // find best individual for current case
        IndividualP caseBest = selBestOpP->select(alive, currentCase);
        LexiCaseFitnessMinP bestFitness = boost::static_pointer_cast<LexiCaseFitnessMin>(caseBest->fitness);

        // terminate individuals with case fitness > (case_best_fitness +- distance)
        terminate.clear();
        for (int j = 0; j < alive.size(); j++){
            if (caseBest->index == alive.at(j)->index) { continue; }
            LexiCaseFitnessMinP fitness = boost::static_pointer_cast<LexiCaseFitnessMin>(alive.at(j)->fitness);
            if (abs(bestFitness->vektor[currentCase] - fitness->vektor[currentCase]) > distance) {
                terminate.push_back(j);
            }
        }

        // start removing from greater index
        std::sort(terminate.begin(), terminate.end(), std::greater<int>());
        for (int j = 0; j < terminate.size(); j++) {
            if (alive.size() - 1 < k) break;
            alive.erase(alive.begin() + terminate[j]);
        }

        // if there is k alive individuals return it
        if (alive.size() == k) { return alive; }
    }

    // while there are more then k alive individuals
    while (alive.size() > k) {
        int random = state_->getRandomizer()->getRandomInteger(alive.size());
        alive.erase(alive.begin() + random);
    }

    return alive;
}

void Lexicase::initCases(IndividualP ind) {
    if (case_permutation.empty()) {
        LexiCaseFitnessMinP fitness = boost::static_pointer_cast<LexiCaseFitnessMin>(ind->fitness);
        uint nSamples = fitness->vektor.size();
        for (int i = 0; i < nSamples; i++) { case_permutation.push_back(i); }
    }
    std::random_shuffle(case_permutation.begin(), case_permutation.end());
}
