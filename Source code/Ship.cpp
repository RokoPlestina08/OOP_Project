#include <iostream>
#include <vector>
#include <string>
#include "Ship.h"
#include "Position.h"

class Ship;

Ship::Ship(std::string name, int size) : Name(name), Size(size), Health(size) {
	Coordinates.reserve(Size);
}

int Ship::GetShipSize() const {
	return Size;
}
void Ship::AddPosition(Position& position) {
	Coordinates.push_back(&position);
}
std::string Ship::GetName() const {
	return Name;
}
std::string Ship::ShipStatus() const {
	if (Health > 0) {
		return "Operational";
	}
	else if (Health == 0) {
		return "Destroyed";
	}
	else return "Sunk";
}
void Ship::SinkShip() {
	if (Health == 0) {
		Health = -1;
	}
}
Position& Ship::GetShipPosition(int Index) const {
	return *Coordinates.at(Index);
}
void Ship::DamageShip() {
	if (Health > 0) {
		--Health;
	}
	else if (Health == 0) {
		Health = -1;
	}
}
