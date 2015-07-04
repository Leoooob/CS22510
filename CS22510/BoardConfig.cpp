#include "stdafx.h"
#include "BoardConfig.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

const char* confManager = "Manager.conf";
const char* confAphids = "Aphids.conf";
const char* confLadybirds = "Ladybirds.conf";

BoardConfig::BoardConfig(void) {
	ifstream myFile(confManager);
	string myLine;
	int lineNo = 0;

	if(!myFile) {
		cout << "Cannot find file: " << myFile <<endl;
	}

	while(getline(myFile,myLine)) {
		stringstream ss(myLine);
		//cout << lineNo << endl;		//check the lineNo manually

		if (!lineNo) {
			ss >> setup.GridX >> setup.GridY; 
			//cout << "Grid size: " << myLine << endl;		//test the data before submitting it to make sure that it is correct
			//cout << "Grid size: [" << setup.GridX << "][" << setup.GridY << "]" << endl;
		}
		else if (lineNo == 1) {
			ss >> setup.aphidNo; 
			//cout << "aphid no: " << myLine << endl;
			//cout << "Number of Aphids: " << setup.aphidNo << endl;
		}
		else if (lineNo >= 2 && lineNo < (setup.aphidNo + 2)) {
			CreatureLocation aphid;
			ss >> aphid.x >> aphid.y; 
			setup.locAphid.push_back(aphid);
			//cout << "Aphid coords: [" << aphid.x << "][" << aphid.y << "]" << endl;
		}
		else if (myLine.length() <= 2 && lineNo != 1) {
			//cout << "line length: " << myLine.length() << endl;
			ss >> setup.ladybirdNo;
			//cout << "Number of Ladybirds: " << setup.ladybirdNo << endl;
		}
		else {
			CreatureLocation ladybird;
			ss >> ladybird.x >> ladybird.y; 
			setup.locLadybird.push_back(ladybird);
			//cout << "Ladybird coords: [" << ladybird.x << "][" << ladybird.y << "]" << endl;
		}
		lineNo++;
	}
}
	
void BoardConfig::AphidConfig(void) {
	ifstream myFile(confAphids);
	string myLine;
	int lineNo = 0;

	if(!myFile) {
		cout << "Cannot find file: " << myFile <<endl;
	}
	else {
		myFile >> confAphid.Move >> confAphid.Kill >> confAphid.Help >> confAphid.Mate;
	}
	//cout << "\nAphid Probabilities"		<< endl;
	//cout << "Move: "					<< confAphid.Move << endl;
	//cout << "Kill: "					<< confAphid.Kill<< endl;
	//cout << "Additional probability: "	<< confAphid.Help << endl;
	//cout << "Mating :"					<< confAphid.Mate << endl;
};

void BoardConfig::LadybirdConfig(void) {
	ifstream myFile(confLadybirds);
	string myLine;
	int lineNo = 0;

	if(!myFile) {
		cout << "Cannot find file: " << myFile <<endl;
	}
	else {
		myFile >> confLadybird.Move >> confLadybird.Kill >> confLadybird.Help >> confLadybird.Mate;
	}
	//cout << "\nLadybird Probabilities"	<< endl;
	//cout << "Move: "					<< confLadybird.Move << endl;
	//cout << "Kill: "					<< confLadybird.Kill << endl;
	//cout << "Change Direction: "		<< confLadybird.Help << endl;
	//cout << "Mating :"					<< confLadybird.Mate << endl;
};

BoardConfig::~BoardConfig(void) {
}
