#pragma once

class Aphid {
public:
	Aphid(void);
	~Aphid(void);

	float getMove();
	float getMate();
	float getKill();
	float getAdditionalProb();
	bool getMoved(void);

	void setMoved(bool);
	void setProbabilities(float, float, float, float);
private:
	float move;
	float mate;
	float kill;
	float additionalProbability;
	bool moved;
};