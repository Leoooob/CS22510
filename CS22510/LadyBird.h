#pragma once

class LadyBird {
public:
	LadyBird(void);
	~LadyBird(void);

	float getMove();
	float getMate();
	float getKill();
	float getChangeDirection();
	int getPrefDirection();
	bool getMoved();

	void setMoved(bool);
	void setPrefDirection(int);
	void setProbabilities(float, float, float, float, int);

private:
	float move;
	float mate;
	float kill;
	float changeDirection;
	int prefDirection;
	bool moved;
};

