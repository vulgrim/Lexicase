#ifndef LEXICASESELBESTOP_H_
#define LEXICASESELBESTOP_H_

#include "ecf/SelectionOperator.h"

/**
 * \ingroup evol selection
 * \brief Best individual selection operator 
 */
class LexiCaseSelBestOp: public SelectionOperator {
public:
	bool initialize(StateP);
	IndividualP select(const std::vector<IndividualP>&);
    IndividualP select(const std::vector<IndividualP>&, int for_case);
};

typedef boost::shared_ptr<LexiCaseSelBestOp> LexiCaseSelBestOpP;

#endif /* LEXICASESELBESTOP_H_ */

