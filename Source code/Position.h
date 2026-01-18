#ifndef Position_h
#define Position_h

class Ship;

class Position {
public:
	Position(int X, char Y);

	int GetX() const;

	char GetY() const;

	void PutShip(Ship& ship);

	const Ship* GetShip() const;

	bool IsHit() const;

	void Attack();

private:
	int x;
	char y;
	bool Hit;
	Ship* Ship_Present;
};

#endif
