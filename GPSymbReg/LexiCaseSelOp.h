//
// Created by kristijan on 17/04/18.
//

#ifndef GPSYMBREG_LEXICASESELOP_H
#define GPSYMBREG_LEXICASESELOP_H

#include <ecf/SelRandomOp.h>
#include "ecf/SelectionOperator.h"
#include "LexiCaseSelBestOp.h"

class LexiCaseSelOp : public SelectionOperator {

public:
    bool initialize(StateP);
    bool setDistance(float);
    IndividualP select(const std::vector<IndividualP>&);
    std::vector<IndividualP> selectMany(const std::vector<IndividualP>&, uint);

protected:
    float distance;
    LexiCaseSelBestOpP selBestOpP;
    SelRandomOpP selRandomOpP;
    std::vector<uint> case_permutation;
    void initCases(IndividualP ind);
    std::vector<IndividualP> select(const std::vector<IndividualP>&, uint);
};
typedef boost::shared_ptr<LexiCaseSelOp> LexiCaseSelOpP;


#endif //GPSYMBREG_LEXICASESELOP_H
