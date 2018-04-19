#include "ecf/ECF_base.h"
#include "ecf/FitnessMin.h"
#include "LexiCaseFitnessMin.h"
#include <sstream>

bool LexiCaseFitnessMin::isBetterThan(FitnessP other, uint index){
	LexiCaseFitnessMinP compareTo = boost::static_pointer_cast<LexiCaseFitnessMin>(other);
	return vektor[index] < compareTo->vektor[index];
}

bool LexiCaseFitnessMin::isWorseThan(FitnessP other, uint index) {
    LexiCaseFitnessMinP compareTo = boost::static_pointer_cast<LexiCaseFitnessMin>(other);
    return vektor[index] > compareTo->vektor[index];
}

double LexiCaseFitnessMin::getValue() {
	double value = 0;
	for (int i = 0; i < vektor.size(); i++) value += abs(vektor[i]);
	return value;
}

bool LexiCaseFitnessMin::isBetterThan(FitnessP other) {
    LexiCaseFitnessMinP compareTo = boost::static_pointer_cast<LexiCaseFitnessMin>(other);
    return this->getValue() < compareTo->getValue();
}

LexiCaseFitnessMin* LexiCaseFitnessMin::copy() {
	auto *newObject = new LexiCaseFitnessMin(*this);
	return newObject;
}


void LexiCaseFitnessMin::write(XMLNode &xFitness) {
	xFitness = XMLNode::createXMLTopNode("LexiCaseFitnessMin");
	std::stringstream sValue;
	sValue << value_;
	xFitness.addAttribute("value", sValue.str().c_str());
}
