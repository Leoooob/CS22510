// CS22510.cpp : Defines the entry point for the console application.
//
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>  

#include "Cell.h"
#include "Aphid.h"
#include "stdafx.h"
#include "BoardConfig.h"

using namespace std;

int turns = 0;
BoardConfig game;

//checks a cell to see if either ladybirds or aphids initiate a fight
void haveAFight(Cell** grid, int i, int j) {
	//Ladybird initiates the scrap
	for (size_t x = 0; x < grid[i][j].ladybirds.size(); x++) {
		if (grid[i][j].ladybirds[x].getMoved() && turns != 0) {
			//cout << "Ladybird has moved" << endl;
			if (grid[i][j].aphids.size() > 0) {
				//cout << "Aphids also exist" << endl;
				int chance = (rand() % 10 + 1);
				if ((grid[i][j].ladybirds[x].getKill() * 10) >= chance) {
					//cout << "Ladybird kills aphid @ [" << i << "][" << j << "]" << endl;
					game.setup.aphidNo--;
					//cout << "Aphid No: " << game.setup.aphidNo << endl;
					grid[i][j].aphids.erase(grid[i][j].aphids.begin());
				}
			}
			//cout << "Ladybird can now move again" << endl;
			grid[i][j].ladybirds[x].setMoved(false);
		}
	}
	//Aphid initiates the scrap
	for (size_t x = 0; x < grid[i][j].aphids.size(); x++) {		//check to see if any of the aphids moved
		if (grid[i][j].aphids[x].getMoved()) {
			//cout << "Aphid has moved" << endl;
			if (grid[i][j].ladybirds.size() > 0) {
				//cout << "Ladybird also exist" << endl;
				int chance = (rand() % 10 + 1);
				//	kill it = (getKill() + (x * getAdditionalProb))
				int killchance = (int)((grid[i][j].aphids[x].getKill() * 10) + (grid[i][j].aphids.size() * grid[i][j].aphids[x].getAdditionalProb()));
				//10 = 100%, if it is higher then cap it.
				if (killchance > 10) {
					killchance = 10;
				}
				if (killchance >= chance) {
					//cout << "Aphid kills ladybird @ [" << i << "][" << j << "]" << endl;
					game.setup.ladybirdNo--;
					//cout << "Ladybird No: " << game.setup.ladybirdNo << endl;
					grid[i][j].ladybirds.erase(grid[i][j].ladybirds.begin());
				}
			}
			//cout << "Aphid can now move again" << endl;
			grid[i][j].aphids[x].setMoved(false);
		}
	}
}

//checks a cell to see if any insects breed
void makeBabby(Cell** grid, int i, int j) {
	//Ladybird's sexy time
	if (grid[i][j].ladybirds.size() > 1) {
		//cout << "Ladybirds sexytime begins" << endl;
		int noSexyTime = 0;
		int chance = 0;

		if ((grid[i][j].ladybirds.size()) % 2) {	//if no ladybirds is odd
			noSexyTime = ((grid[i][j].ladybirds.size() - 1) / 2);		//number of pairs of ladybirds
			//cout << "odd no pairs:" << noSexyTime << endl;
		}
		else {
			noSexyTime = (grid[i][j].ladybirds.size() / 2);
			//cout << "even no pairs:" << noSexyTime << endl;
		}

		for (int c = 0; c < noSexyTime; c++) {	//loop for each pair
			chance = (rand() % 10 + 1);
			//cout << "Ladybird try make babby @ chance: " << chance << endl;
			if ((grid[i][j].ladybirds[0].getMate() * 10) >= chance) {	//getMate for any ladybird, all the same
				//cout << "Ladybird babby" << endl;
				LadyBird babbyLadybird;
				float move		= grid[i][j].ladybirds[0].getMove();
				float mate		= grid[i][j].ladybirds[0].getMate();
				float kill		= grid[i][j].ladybirds[0].getKill();
				float changedir = grid[i][j].ladybirds[0].getChangeDirection();

				babbyLadybird.setProbabilities(move, mate, kill, changedir, 1);	//preferred dir defaults to 1 (NORTH)
				babbyLadybird.setMoved(false);
				game.setup.ladybirdNo++;
				grid[i][j].ladybirds.push_back(babbyLadybird);
			}
			else {
				//cout << "Ladybirds fail to make babby" << endl;
			}

		}
	}
	//Aphid's sexy time
	if (grid[i][j].aphids.size() > 1) {
		//cout << "Aphids sexytime begins" << endl;
		int noSexyTime = 0;
		int chance = 0;

		if ((grid[i][j].aphids.size()) % 2) {
			noSexyTime = ((grid[i][j].aphids.size() - 1) / 2);
			//cout << "odd no pairs:" << noSexyTime << endl;
		}
		else {
			noSexyTime = (grid[i][j].aphids.size() / 2);
			//cout << "even no pairs:" << noSexyTime << endl;
		}

		for (int c = 0; c < noSexyTime; c++) {
			chance = (rand() % 10 + 1);
			//cout << "Aphid try make babby @ chance: " << chance << endl;
			if ((grid[i][j].aphids[0].getMate() * 10) >= chance) {
				//cout << "Aphid babby" << endl;
				Aphid babbyAphid;
				float move		= grid[i][j].aphids[0].getMove();
				float mate		= grid[i][j].aphids[0].getMate();
				float kill		= grid[i][j].aphids[0].getKill();
				float addprob	= grid[i][j].aphids[0].getAdditionalProb();

				babbyAphid.setProbabilities(move, mate, kill, addprob);
				babbyAphid.setMoved(false);
				game.setup.aphidNo++;
				grid[i][j].aphids.push_back(babbyAphid);
			}
			else {
				//cout << "Aphids fail to make babby" << endl;
			}
		}
	}
}

//checks to see if a ladybird changes it's preferred direction
void changePrefDirLadybird(Cell** grid, int i, int j) {
	if (grid[i][j].ladybirds.size() > 0) {
		for (size_t x = 0; x < grid[i][j].ladybirds.size(); x++) {	//using size_t instead of int to avoid warning
			int chance = (rand() % 10 + 1);							// of unsigned/signed mismatch in my comparison
			if ((grid[i][j].ladybirds[x].getChangeDirection() * 10) >= chance) {
				bool foundnew = false;
				while (!foundnew) {
					int newdir = rand() % 4 + 1;	//1 = N, 2 = E, 3 = S, 4 = W
					//cout << "newdir rand: " << newdir << endl;
					if (newdir != grid[i][j].ladybirds[x].getPrefDirection()) {
						grid[i][j].ladybirds[x].setPrefDirection(newdir);
						//cout << "newdir: " << grid[i][j].ladybirds[x].getPrefDirection() << endl;
						foundnew = true;
					}
				}
				//cout << "Changing preferred direction" << endl;
			}
		}
	}
}

//returns an int to be used by ladybug movement, to decide subdirection
int makeSubDir() {
	int subdir = rand() % 2;
	return subdir;
}

//returns a boolean to check if a location is in bounds of our grid
bool inBounds(int x, int y) {
	if (x > game.setup.GridX - 1 || y > game.setup.GridY - 1) {		//Count starts at 0, -1 from grid size
		return false;
	}
	else if (x < 0 || y < 0) {
		return false;
	}
	else {
		return true;
	}
}

//checks a cell to see if any ladybirds move
void movementLadybird(Cell** grid, int i, int j) {
	if (grid[i][j].ladybirds.size() > 0) {
		for (size_t x = 0; x < grid[i][j].ladybirds.size(); x++) {
			if (grid[i][j].ladybirds[x].getMoved()) {		//if the ladybird has moved then stop the update
				return;
			}
			int chance = (rand() % 10 + 1);
			if ((grid[i][j].ladybirds[x].getMove() * 10) >= chance) {
				LadyBird newLadybird;

				float move		= grid[i][j].ladybirds[x].getMove();
				float mate		= grid[i][j].ladybirds[x].getMate();
				float kill		= grid[i][j].ladybirds[x].getKill();
				float changedir = grid[i][j].ladybirds[x].getChangeDirection();
				int prefdir		= grid[i][j].ladybirds[x].getPrefDirection();

				newLadybird.setMoved(true);
				newLadybird.setProbabilities(move, mate, kill, changedir, prefdir);

				bool moved = false;
				int newI = 0;
				int newJ = 0;
				int currentdir = grid[i][j].ladybirds[x].getPrefDirection();
				while (!moved) {
					switch (currentdir) {
					//NORTH//////////////////////////////////////////////////////////////////////
					case 1:
						switch(makeSubDir()) {
						case 0:
							//move northwest
							newI = (i-1);
							newJ = (j+1);
							if (inBounds(newI,newJ)) {
								//MOVE the ladybird -> moved = true
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;	//break out of the switch statement
							}
							//Other corner
							else {
								newI = (i+1);
								newJ = (j+1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
								//Reverse direction
								else {
									newI = (i+1);
									newJ = (j-1);
									newLadybird.setPrefDirection(3);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].ladybirds.push_back(newLadybird);
										moved = true;
										break;
									}
									//Other corner of reverse
									else {
										newI = (i-1);
										newJ = (j-1);
										try {
											grid[newI][newJ].ladybirds.push_back(newLadybird);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						case 1:
							//move up
							newI = i;
							newJ = (j+1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newJ = (j-1);
								newLadybird.setPrefDirection(3);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
							}
						case 2:
							//move northeast
							newI = (i+1);
							newJ = (j+1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newI = (i-1);
								newJ = (j+1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
								else {
									newI = (i-1);
									newJ = (j-1);
									newLadybird.setPrefDirection(3);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].ladybirds.push_back(newLadybird);
										moved = true;
										break;
									}
									else {
										newI = (i+1);
										newJ = (j-1);
										try {
											grid[newI][newJ].ladybirds.push_back(newLadybird);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						}
						break;
					//EAST//////////////////////////////////////////////////////////////////////
					case 2:
						//cout << "east" << endl;
						switch(makeSubDir()) {
						case 0:
							//move northeast
							newI = (i+1);
							newJ = (j+1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newI = (i-1);
								newJ = (j+1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
								else {
									newI = (i-1);
									newJ = (j-1);
									newLadybird.setPrefDirection(4);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].ladybirds.push_back(newLadybird);
										moved = true;
										break;
									}
									else {
										newI = (i+1);
										newJ = (j-1);
										try {
											grid[newI][newJ].ladybirds.push_back(newLadybird);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						case 1:
							//move right
							newI = (i+1);
							newJ = (j);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newI = (i-1);
								newLadybird.setPrefDirection(4);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
							}
						case 2:
							//move southwest
							newI = (i-1);
							newJ = (j-1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newI = (i+1);
								newJ = (j-1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
								else {
									newI = (i+1);
									newJ = (j+1);
									newLadybird.setPrefDirection(4);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].ladybirds.push_back(newLadybird);
										moved = true;
										break;
									}
									else {
										newI = (i-1);
										newJ = (j+1);
										try {
											grid[newI][newJ].ladybirds.push_back(newLadybird);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						}
						break;
					//SOUTH//////////////////////////////////////////////////////////////////////
					case 3:
						//cout << "south" << endl;
						switch(makeSubDir()) {
						case 0:
							//move southwest
							newI = (i-1);
							newJ = (j-1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newI = (i+1);
								newJ = (j-1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
								else {
									newI = (i+1);
									newJ = (j+1);
									newLadybird.setPrefDirection(1);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].ladybirds.push_back(newLadybird);
										moved = true;
										break;
									}
									else {
										newI = (i-1);
										newJ = (j+1);
										try {
											grid[newI][newJ].ladybirds.push_back(newLadybird);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						case 1:
							//move down
							newI = (i);
							newJ = (j-1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newJ = (j+1);
								newLadybird.setPrefDirection(1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
							}
						case 2:
							//move southeast
							newI = (i+1);
							newJ = (j-1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newI = (i-1);
								newJ = (j-1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
								else {
									newI = (i-1);
									newJ = (j+1);
									newLadybird.setPrefDirection(1);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].ladybirds.push_back(newLadybird);
										moved = true;
										break;
									}
									else {
										newI = (i+1);
										newJ = (j+1);
										try {
											grid[newI][newJ].ladybirds.push_back(newLadybird);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						}
						break;
					//WEST//////////////////////////////////////////////////////////////////////
					case 4:
						//cout << "west" << endl;
						switch(makeSubDir()) {
						case 0:
							//move southwest
							newI = (i-1);
							newJ = (j-1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newI = (i+1);
								newJ = (j-1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
								else {
									newI = (i+1);
									newJ = (j+1);
									newLadybird.setPrefDirection(2);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].ladybirds.push_back(newLadybird);
										moved = true;
										break;
									}
									else {
										newI = (i-1);
										newJ = (j+1);
										try {
											grid[newI][newJ].ladybirds.push_back(newLadybird);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						case 1:
							//move left
							newI = (i-1);
							newJ = (j);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newI = (i+1);
								newLadybird.setPrefDirection(2);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
							}
						case 2:
							//move northwest
							newI = (i-1);
							newJ = (j+1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].ladybirds.push_back(newLadybird);
								moved = true;
								break;
							}
							else {
								newI = (i+1);
								newJ = (j+1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].ladybirds.push_back(newLadybird);
									moved = true;
									break;
								}
								else {
									newI = (i+1);
									newJ = (j-1);
									newLadybird.setPrefDirection(2);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].ladybirds.push_back(newLadybird);
										moved = true;
										break;
									}
									else {
										newI = (i-1);
										newJ = (j-1);
										try {
											grid[newI][newJ].ladybirds.push_back(newLadybird);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						}
						break;
	//////////////////////////////////////////////////////////////////////////////////////////////////////
					}
				}
				// Delete old ladybird
				//if size == 0, don't delete else delete
				if (grid[i][j].ladybirds.size() > 0) {
					grid[i][j].ladybirds.erase(grid[i][j].ladybirds.begin() + x);
				}
			}
		}
	}
}

//checks a cell to see if any aphids move
void movementAphid(Cell** grid, int i, int j) {
	if (grid[i][j].aphids.size() > 0) {
		for (size_t x = 0; x < grid[i][j].aphids.size(); x++) {
			if (grid[i][j].aphids[x].getMoved()) {
				return;
			}
			int chance = (rand() % 10 + 1);
			if ((grid[i][j].aphids[x].getMove() * 10) >= chance) {
				Aphid newAphid;
			
				float move					= grid[i][j].aphids[x].getMove();
				float mate					= grid[i][j].aphids[x].getMate();
				float kill					= grid[i][j].aphids[x].getKill();
				float additionalProbability = grid[i][j].aphids[x].getAdditionalProb();

				newAphid.setMoved(true);
				newAphid.setProbabilities(move, mate, kill, additionalProbability);

				bool moved = false;
				int newI = 0;
				int newJ = 0;
				int mydir = (rand() % 8 + 1);
				while (!moved) {
					switch (mydir) {
						//north
						case 1:
							newI = i;
							newJ = (j+1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].aphids.push_back(newAphid);
								moved = true;
								break;
							}
							//Reverse direction
							else {
								newJ = (j-1);
								try {	//should never be out of bounds, but just incase
									grid[newI][newJ].aphids.push_back(newAphid);
									moved = true;
									break;
								}
								catch (const std::out_of_range& oor) {
									std::cerr << "Out of Range error: " << oor.what() << endl;
								}
							}
						case 2:
							//move northeast
							newI = (i+1);
							newJ = (j+1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].aphids.push_back(newAphid);
								moved = true;
								break;
							}
							else {
								newI = (i-1);
								newJ = (j+1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].aphids.push_back(newAphid);
									moved = true;
									break;
								}
								else {
									newI = (i-1);
									newJ = (j-1);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].aphids.push_back(newAphid);
										moved = true;
										break;
									}
									else {
										newI = (i+1);
										newJ = (j-1);
										try {	//should never be out of bounds, but just incase
											grid[newI][newJ].aphids.push_back(newAphid);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						//east
						case 3:
							newI = (i+1);
							newJ = (j);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].aphids.push_back(newAphid);
								moved = true;
								break;
							}
							else {
								newI = (i-1);
								try {
									grid[newI][newJ].aphids.push_back(newAphid);
									moved = true;
									break;
								}
								catch (const std::out_of_range& oor) {
									std::cerr << "Out of Range error: " << oor.what() << endl;
								}
							}
						//southeast
						case 4:
							newI = (i+1);
							newJ = (j-1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].aphids.push_back(newAphid);
								moved = true;
								break;
							}
							else {
								newI = (i-1);
								newJ = (j-1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].aphids.push_back(newAphid);
									moved = true;
									break;
								}
								else {
									newI = (i-1);
									newJ = (j+1);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].aphids.push_back(newAphid);
										moved = true;
										break;
									}
									else {
										newI = (i+1);
										newJ = (j+1);
										try {
											grid[newI][newJ].aphids.push_back(newAphid);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						//south
						case 5:
							newI = (i);
							newJ = (j-1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].aphids.push_back(newAphid);
								moved = true;
								break;
							}
							else {
								newJ = (j+1);
								try {
									grid[newI][newJ].aphids.push_back(newAphid);
									moved = true;
									break;
								}
								catch (const std::out_of_range& oor) {
									std::cerr << "Out of Range error: " << oor.what() << endl;
								}
							}
						//southwest
						case 6:
							newI = (i-1);
							newJ = (j-1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].aphids.push_back(newAphid);
								moved = true;
								break;
							}
							else {
								newI = (i+1);
								newJ = (j-1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].aphids.push_back(newAphid);
									moved = true;
									break;
								}
								else {
									newI = (i+1);
									newJ = (j+1);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].aphids.push_back(newAphid);
										moved = true;
										break;
									}
									else {
										newI = (i-1);
										newJ = (j+1);
										try {
											grid[newI][newJ].aphids.push_back(newAphid);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
						//west
						case 7:
							newI = (i-1);
							newJ = (j);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].aphids.push_back(newAphid);
								moved = true;
								break;
							}
							else {
								newI = (i+1);
								try {
									grid[newI][newJ].aphids.push_back(newAphid);
									moved = true;
									break;
								}
								catch (const std::out_of_range& oor) {
									std::cerr << "Out of Range error: " << oor.what() << endl;
								}
							}
						//northwest
						case 8:
							newI = (i-1);
							newJ = (j+1);
							if (inBounds(newI,newJ)) {
								grid[newI][newJ].aphids.push_back(newAphid);
								moved = true;
								break;
							}
							else {
								newI = (i+1);
								newJ = (j+1);
								if (inBounds(newI,newJ)) {
									grid[newI][newJ].aphids.push_back(newAphid);
									moved = true;
									break;
								}
								else {
									newI = (i+1);
									newJ = (j-1);
									if (inBounds(newI,newJ)) {
										grid[newI][newJ].aphids.push_back(newAphid);
										moved = true;
										break;
									}
									else {
										newI = (i-1);
										newJ = (j-1);
										try {
											grid[newI][newJ].aphids.push_back(newAphid);
											moved = true;
											break;
										}
										catch (const std::out_of_range& oor) {
											std::cerr << "Out of Range error: " << oor.what() << endl;
										}
									}
								}
							}
					}
					//delete old aphid
					if (grid[i][j].aphids.size() > 0) {
						grid[i][j].aphids.erase(grid[i][j].aphids.begin() + x);
					}
				}
			}
		}
	}
}

//builds the initial grid, adds creatures to relevant structs
Cell** buildGrid() {
	Cell** grid = new Cell*[game.setup.GridX];
	for (int i = 0; i < game.setup.GridX; i++) {
		grid[i] = new Cell[game.setup.GridY];
	}

	// create creatures and assign them to cells
	for (int i = 0; i < game.setup.aphidNo; i++) {
		Aphid aphid;
		aphid.setProbabilities(game.confAphid.Move, game.confAphid.Mate, game.confAphid.Kill, game.confAphid.Help);
		grid[game.setup.locAphid[i].x][game.setup.locAphid[i].y].aphids.push_back(aphid);
		aphid.setMoved(false);
	}
	for (int i = 0; i < game.setup.ladybirdNo; i++) {
		LadyBird ladybird;
		ladybird.setProbabilities(game.confLadybird.Move, game.confLadybird.Mate, game.confLadybird.Kill, game.confLadybird.Help, 1);
		grid[game.setup.locLadybird[i].x][game.setup.locLadybird[i].y].ladybirds.push_back(ladybird);
		ladybird.setMoved(false);
	}
	return grid;
}

//the loop for the game that is called in main and loops until the user quits or the game ends
void gameLoop(Cell** grid) {
	char input = 'a';
	while (input != 'q') {
		cout << "Turn: " << turns << endl;	

		//prints a message to see which side won, you can also check the number of each creature in the previous grid print
		if (!game.setup.ladybirdNo) {
			cout << "The Ladybirds are dead.. Long live the Aphids." << endl;
			break;
		}
		if (!game.setup.aphidNo) {
			cout << "The Aphids are dead.. Steady yourself." << endl;
			break;
		}

		//these checks performed before outputting number of creatures so that they aren't old values
		if(turns > 0) {		//if it IS the first turn, this won't be called.
			for (int i = 0; i < game.setup.GridX; i++) {
				for (int j = 0; j < game.setup.GridY; j++) {
					movementAphid(grid, i, j);
					changePrefDirLadybird(grid, i, j);
					movementLadybird(grid, i, j);
					haveAFight(grid, i, j);	//see who's best, obviously
					makeBabby(grid, i, j);
				}
			}
		}

		cout << "No of Aphids: " << game.setup.aphidNo << endl;			//these numbers aren't linked to the vector lists
		cout << "No of Ladybirds: " << game.setup.ladybirdNo << endl;	// must be manually updated when adding/removing creatures

		//row divider
		for (int b = 0; b < game.setup.GridX * 4 + 1; b++) {
			cout << "_";
		}
		cout << endl;

		for (int i = 0; i < game.setup.GridX; i++) {
			cout << "|";
			for (int j = 0; j < game.setup.GridY; j++) {
				if (grid[i][j].aphids.empty() && grid[i][j].ladybirds.empty()) {
					cout << "  ";
				}
				else if (grid[i][j].aphids.empty()) {
						cout << " ,";
				}
				else if (!grid[i][j].aphids.empty()) {
					if (grid[i][j].aphids.size() > 9) {
						cout << "~,";
					}
					else {
						cout << grid[i][j].aphids.size() << ",";
					}
				}

				if (grid[i][j].ladybirds.empty()) {
					cout << " ";
				}
				else if (!grid[i][j].ladybirds.empty()) {
					if (grid[i][j].ladybirds.size() > 9) {
						cout << "~";
					}
					else {
						cout << grid[i][j].ladybirds.size();
					}
				}
				cout << "|";
			}
			cout << endl;

			//row divider
			for (int b = 0; b < game.setup.GridX * 4 + 1; b++) {
				cout << "_";
			}
			cout << endl;
		}
		cout << "Press enter to continue, enter q to quit." << endl;
		cin.get(input);	//use .get() so that it can be any input, not just a strict char
		turns++;
	}
	exit(0);
}

int main() {
	game.AphidConfig(); 
	game.LadybirdConfig();
	gameLoop(buildGrid());
}