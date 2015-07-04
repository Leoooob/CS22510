#pragma once
#include<vector>

class BoardConfig {
public:
	BoardConfig(void);
	~BoardConfig(void);

	void AphidConfig(void);
	void LadybirdConfig(void);

	struct CreatureLocation {
		int x,y;
	};

	struct ManagerSetup {
		int GridX, GridY, aphidNo, ladybirdNo;
		std::vector<CreatureLocation> locAphid, locLadybird;
	};

	struct CreatureConfig {
		float Move, Mate, Kill, Help;
	};

	ManagerSetup setup;
	CreatureLocation locAphid;
	CreatureLocation locLadybird;
	CreatureConfig confAphid;
	CreatureConfig confLadybird;
};

