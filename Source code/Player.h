#ifndef Player_h
#define Player_h

class Player {
protected:
	std::vector<Ship> Ships;
	std::string Name;
	std::vector<int> EnemyShipsRemaining;
	std::vector<Position> Grid;
	const std::vector<Position>* EnemyGrid;
	bool CheckSurrondingPositions(int IndexPos, const std::vector<Position>& Grid) const;
	void VisualizeAttackedPosition(int XScreenPos, int YScreenPos, bool Visualize) const;
public:
	Player(std::string name);

	void CreateShip(std::string ShipName, int ShipSize);

	int PlaceShip(int ShipIndex);

	virtual int Attack(bool ShowEnemyShips, bool FastRounds, int k) = 0;

	void RegisterAttack(int Index);

	void SinkShip(int Index);

	const Ship& GetShip(int Index) const;

	std::string GetName() const;

	int GetSmallestEnemyShipSizeRemaining() const;

	int ShipsLeft() const;

	const std::vector<Position>* GetGrid() const;

	void PutEnemyGrid(const std::vector<Position>* Grid);
};

#endif
