#ifndef Player2_h
#define Player2_h

class Player;

class Player2 : public Player {
public:
	Player2(std::string name);

	int Attack(bool ShowEnemyShips, bool FastRounds, int k) override;

	int CheckForHitPositions(int IndexPos, const std::vector<Position>& Grid) const;
private:
	const Position* InitialPositionHit;
	const Position* LastPositionFired;
	std::vector<const Position*> PositionsHit;
	bool CanShootUp, CanShootDown, CanShootLeft, CanShootRight;
	int AttackDirection;
};

#endif
