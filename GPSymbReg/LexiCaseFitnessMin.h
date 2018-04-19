#ifndef LEXICASEFITNESSMIN_H_
#define LEXICASEFITNESSMIN_H_

#include "ecf/Fitness.h"

/**
 * \ingroup evol main
 * \brief Fitness for minimization problems.
 *
 * Fitness is better if the fitness value is smaller.
 */
class LexiCaseFitnessMin: public Fitness {
public:
	LexiCaseFitnessMin() {}

	bool isBetterThan(FitnessP other);
	void write(XMLNode&);
	LexiCaseFitnessMin* copy();

	std::vector<double> vektor;

	double getValue() override;
	bool isBetterThan(FitnessP other, uint index = 0);
	bool isWorseThan(FitnessP other, uint index = 0);
};
typedef boost::shared_ptr<LexiCaseFitnessMin> LexiCaseFitnessMinP;

#endif /* LEXICASEFITNESSMIN_H_ */

