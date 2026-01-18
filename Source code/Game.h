#ifndef Game_h
#define Game_h

class Game {
public:

	Game();

	void PrintGrids();

	void UpdateGrid();

	void ShipDestroyedVisual(const Ship& ship, std::string PlayerName, int k);

	void StartGame();


private:
	int Round;
	int GridSize;
	bool ShowEnemyShips;
	bool FastRounds;
	bool Player1AsBot;
	std::string Turn;
	std::unique_ptr<Player1> P1;
	std::unique_ptr<Player2> P1_Alt;
	std::unique_ptr<Player2> P2;
	const std::vector<Position>* Player1Grid;
	const std::vector<Position>* Player2Grid;
};

#endif#
