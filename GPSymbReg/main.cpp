#include <ecf/ECF.h>
#include "SymbRegEvalOp.h"
#include "Lexicase.h"

int main(int argc, char **argv) {

    StateP state (new State);

	// set the evaluation operator
	state->setEvalOp(new SymbRegEvalOp);
    LexicaseP lexicase = (LexicaseP) new Lexicase;
    state->addAlgorithm(lexicase);

	state->initialize(argc, argv);
	state->run();

	return 0;
}
