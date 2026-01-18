#ifndef Ship_h
#define Ship_h

class Position;

class Ship {
public:
	Ship(std::string name, int size);
	 
	int GetShipSize() const;

	void AddPosition(Position& position);

	std::string GetName() const;

	std::string ShipStatus() const;

	void SinkShip();

	Position& GetShipPosition(int Index) const;

	void DamageShip();

private:
	std::string Name;
	int Health;
	int Size;
	std::vector<Position*> Coordinates;
};

#endif

