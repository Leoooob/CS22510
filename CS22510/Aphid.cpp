#include "Aphid.h"

Aphid::Aphid(void) {

}

float Aphid::getMove(void) {
	return move;
}

float Aphid::getMate() {
	return mate;
}

float Aphid::getKill() {
	return kill;
}

float Aphid::getAdditionalProb() {
	return additionalProbability;
}

bool Aphid::getMoved(void) {
	return moved;
}

void Aphid::setMoved(bool didIMove) {
	moved = didIMove;
}

void Aphid::setProbabilities(float moveProb, float mateProb, float killProb, float additionalProb) {
	move = moveProb;
	mate = mateProb;
	kill = killProb;
	additionalProbability = additionalProb;
}

Aphid::~Aphid(void) {

}
