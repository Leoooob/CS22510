#pragma once

#include<vector>

#include "LadyBird.h"
#include "Aphid.h"

class Cell
{
public:
	Cell(void);
	~Cell(void);
	std::vector<Aphid> aphids;
	std::vector<LadyBird> ladybirds;
};

