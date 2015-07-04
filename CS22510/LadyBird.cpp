#include "LadyBird.h"

LadyBird::LadyBird(void){
	
}

LadyBird::~LadyBird(void){

}

float LadyBird::getMove(void) {
	return move;
}

float LadyBird::getMate(void) {
	return mate;
}

float LadyBird::getKill(void) {
	return kill;
}

float LadyBird::getChangeDirection(void) {
	return changeDirection;
}

int LadyBird::getPrefDirection(void) {
	return prefDirection;
}

bool LadyBird::getMoved(void) {
	return moved;
}

void LadyBird::setMoved(bool didIMove) {
	moved = didIMove;
}


void LadyBird::setPrefDirection(int newdir) {
	prefDirection = newdir;
}

void LadyBird::setProbabilities(float moveProb, float mateProb, float killProb, float directionProb, int thePrefDirection) {
	move = moveProb;
	mate = mateProb;
	kill = killProb;
	changeDirection = directionProb;
	prefDirection = thePrefDirection;
}