#include "ecf/ECF_base.h"
#include "LexiCaseSelBestOp.h"
#include "LexiCaseFitnessMin.h"

bool LexiCaseSelBestOp::initialize(StateP state) {
	state_ = state;
	return true;
}

IndividualP LexiCaseSelBestOp::select(const std::vector<IndividualP>& pool) {
	if(pool.empty())
		return IndividualP();

	IndividualP best = pool[0];
	for (uint i = 1; i < pool.size(); i++) {
		if (!(best->fitness->isBetterThan(pool[i]->fitness))){
			best = pool[i];
		}
	}

	return best;
}


IndividualP LexiCaseSelBestOp::select(const std::vector<IndividualP>& pool, int for_case) {
	if(pool.empty())
		return IndividualP();

	IndividualP best = pool[0];
	LexiCaseFitnessMinP bestFitness = boost::static_pointer_cast<LexiCaseFitnessMin>(best->fitness);
	for (uint i = 1; i < pool.size(); i++) {
		if (!(bestFitness->isBetterThan(pool[i]->fitness, for_case))) {
			best = pool[i];
			bestFitness = boost::static_pointer_cast<LexiCaseFitnessMin>(pool[i]->fitness);
		}
	}

	return best;
}
