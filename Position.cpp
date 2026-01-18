#include <iostream>
#include <vector>
#include <string>
#include "Position.h"
#include "Ship.h"

class Position;

Position::Position(int X, char Y) : x(X), y(Y), Hit(false), Ship_Present(nullptr) {}

int Position::GetX() const {
	return x;
}
char Position::GetY() const {
	return y;
}
void Position::PutShip(Ship& ship) {
	Ship_Present = &ship;
}
const Ship* Position::GetShip() const {
	return Ship_Present;
}
bool Position::IsHit() const {
	return Hit;
}
void Position::Attack() {
	Hit = true;
	if (Ship_Present != nullptr) {
		(*Ship_Present).DamageShip();
	}
}