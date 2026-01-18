#ifndef Player1_h
#define Player1_h

class Player;

class Player1 : public Player {
public:
	Player1(std::string name);

	int PlaceShipManually(int ShipIndex);

	int Attack(bool ShowEnemyShips, bool FastRounds, int k) override;

private:
	void DisplayError(std::string Text, int Time) const;

	void DeleteText(int Lines) const;
};

#endif
