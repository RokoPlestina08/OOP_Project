#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "Ship.h"
#include "Position.h"
#include "Player.h"
#include "Player1.h"
#include "Player2.h"

//////////////////ANSI CODES//////////////////

#define RESET_COLORS (std::cout << "\033[0m")
#define SET_BACKGROUND_BLUE (std::cout << "\033[44m")
#define SET_BACKGROUND_GRAY (std::cout << "\033[48;5;242m")
#define SET_BACKGROUND_DARK_BLUE (std::cout << "\033[48;5;18m")
#define SET_BACKGROUND_YELLOW (std::cout << "\033[48;5;221m")
#define CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition) (std::cout << "\033[" + std::to_string(YAxisPosition) << ";" + std::to_string(XAxisPosition) << "H")
#define SET_TEXT_GREEN (std::cout << "\033[38;5;118m")
#define SET_TEXT_RED (std::cout << "\033[38;5;196m")
#define SET_TEXT_DARK_RED (std::cout << "\033[38;5;124m")
#define SET_TEXT_CRIMSON_RED (std::cout << "\033[31m")
#define SET_TEXT_YELLOW (std::cout << "\033[38;5;226m")
#define SET_TEXT_WHITE (std::cout << "\033[38;5;255m")
#define CURSOR_DOWN(x) (std::cout << "\033[" << std::to_string(x) << "E")
#define HIDE_CURSOR (std::cout << "\033[?25l")
#define SHOW_CURSOR (std::cout << "\033[?25h")
#define SAVE_CURSOR (std::cout << "\033[s")
#define LOAD_CURSOR (std::cout << "\033[u")
#define ERASE_TO_END (std::cout << "\033[0J")
#define CURSOR_UP(x) (std::cout << "\033[" + std::to_string(x) << "F\033[0J")
#define BOLD_TEXT (std::cout << "\033[1m")
#define UNBOLD_TEXT (std::cout << "\033[2m")

//////////////////////////////////////////////

class Player;

Player::Player(std::string name) : Name(name) {
	Ships.reserve(5);
	CreateShip("Carrier", 5);
	CreateShip("Battleship", 4);
	CreateShip("Destroyer", 3);
	CreateShip("Submarine", 3);
	CreateShip("Patrol Boat", 2);
}
void Player::CreateShip(std::string ShipName, int ShipSize) {
	Ships.push_back(Ship(ShipName, ShipSize));
	EnemyShipsRemaining.push_back(ShipSize);
}

const Ship& Player::GetShip(int Index) const {
	return Ships.at(Index);
}

std::string Player::GetName() const {
	return Name;
}

int Player::GetSmallestEnemyShipSizeRemaining() const {
	int Smallest = EnemyShipsRemaining.at(0);
	for (int ShipSize : EnemyShipsRemaining) {
		if (ShipSize < Smallest) {
			Smallest = ShipSize;
		}
	}
	return Smallest;
}
int Player::ShipsLeft() const {
	int Count = Ships.size();
	for (const Ship ship : Ships) {
		if (ship.ShipStatus() == "Sunk") {
			--Count;
		}
	}
	return Count;
}

const std::vector<Position>* Player::GetGrid() const {
	return &Grid;
}

void Player::PutEnemyGrid(const std::vector<Position>* Grid) {
	EnemyGrid = Grid;
}

void Player::RegisterAttack(int Index) {
	Grid.at(Index).Attack();
}

void Player::SinkShip(int Index) {
	Ships.at(Index).SinkShip();
}

int Player::PlaceShip(int ShipIndex) {
	Ship& ShipToPlace = Ships.at(ShipIndex);
	int InputY = rand() % 10;
	int InputX = rand() % 10;
	Position& position = Grid.at(((InputY) * 10) + InputX);
	if (!(CheckSurrondingPositions(((InputY) * 10) + InputX, Grid))) {
		return 0;
	}
	if (position.GetShip() == nullptr) {
		int x = position.GetX();
		int y = position.GetY() - 65;
		int PosIndex = (y * 10) + x;
		int ShipSize = ShipToPlace.GetShipSize() - 1;
		/////////////////////////////////// Placement checks
		bool Up, Down, Left, Right;
		Up = Down = Left = Right = false;
		if (x + ShipSize < 10) { //check if it can fit on the right
			Right = true;
			for (int i = 1; i <= ShipSize; ++i) { //check if there are ships on the right
				if ((Grid.at((y * 10) + x + i)).GetShip() != nullptr) {
					Right = false;
					break;
				}
				if (!(CheckSurrondingPositions((y * 10) + x + i, Grid))) {
					Right = false;
					break;
				}
			}
		}
		if (x - ShipSize >= 0) { //check if it can fit on the left
			Left = true;
			for (int i = 1; i <= ShipSize; ++i) { //check if there are ships on the left
				if ((Grid.at((y * 10) + x - i)).GetShip() != nullptr) {
					Left = false;
					break;
				}
				if (!(CheckSurrondingPositions((y * 10) + x - i, Grid))) {
					Left = false;
					break;
				}
			}
		}
		if (y - ShipSize >= 0) { //check if it can fit up
			Up = true;
			for (int i = 1; i <= ShipSize; ++i) { //check if there are ships above
				if ((Grid.at(((y - i) * 10) + x)).GetShip() != nullptr) {
					Up = false;
					break;
				}
				if (!(CheckSurrondingPositions(((y - i) * 10) + x, Grid))) {
					Up = false;
					break;
				}
			}
		}
		if (y + ShipSize < 10) { //check if it can fit down
			Down = true;
			for (int i = 1; i <= ShipSize; ++i) { //check if there are ships below
				if ((Grid.at(((y + i) * 10) + x)).GetShip() != nullptr) {
					Down = false;
					break;
				}
				if (!(CheckSurrondingPositions(((y + i) * 10) + x, Grid))) {
					Down = false;
					break;
				}
			}
		}
		if (!(Up) && !(Down) && !(Left) && !(Right)) return 0;
		/////////////////////////////////// place the actual ship
		int Direction = rand() % 4;
		do {
			Direction = rand() % 4;
			for (int i = 0; i <= ShipSize; ++i) {
				Position* Pos = nullptr;
				if ((Direction == 0) && (Up)) {
					Pos = &Grid.at(PosIndex - (i * 10));
				}
				else if ((Direction == 1) && (Down)) {
					Pos = &Grid.at(PosIndex + (i * 10));
				}
				else if ((Direction == 2) && (Left)) {
					Pos = &Grid.at(PosIndex - i);
				}
				else if ((Direction == 3) && (Right)) {
					Pos = &Grid.at(PosIndex + i);
				}
				if (Pos != nullptr) {
					(*Pos).PutShip(ShipToPlace);
					ShipToPlace.AddPosition(*Pos);
				}
			}
		} while (!(((Direction == 0) && (Up)) || ((Direction == 1) && (Down)) || ((Direction == 2) && (Left)) || ((Direction == 3) && (Right))));
		return 1;
	}
	else return 0;
}

bool Player::CheckSurrondingPositions(int IndexPos, const std::vector<Position>& Grid) const {
	if ((IndexPos - 10) >= 0) {
		if ((Grid.at(IndexPos - 10)).GetShip() != nullptr) {
			return false;
		}
	}
	if ((IndexPos + 10) < 100) {
		if ((Grid.at(IndexPos + 10)).GetShip() != nullptr) {
			return false;
		}
	}
	if (((IndexPos % 10) - 1) >= 0) {
		if ((Grid.at(IndexPos - 1)).GetShip() != nullptr) {
			return false;
		}
	}
	if (((IndexPos % 10) + 1) < 10) {
		if ((Grid.at(IndexPos + 1)).GetShip() != nullptr) {
			return false;
		}
	}
	return true;
}

void Player::VisualizeAttackedPosition(int XScreenPos, int YScreenPos, bool Visualize) const {
	SAVE_CURSOR;
	RESET_COLORS;
	if (Visualize) {
		SET_TEXT_YELLOW;
		BOLD_TEXT;
	}
	CHANGE_CURSOR_LOCATION(YScreenPos + 1, XScreenPos); std::cout << "-";
	CHANGE_CURSOR_LOCATION(YScreenPos + 1, XScreenPos + 1); std::cout << "-";
	CHANGE_CURSOR_LOCATION(YScreenPos + 1, XScreenPos + 2); std::cout << "+";
	CHANGE_CURSOR_LOCATION(YScreenPos, XScreenPos + 2); std::cout << "|";
	CHANGE_CURSOR_LOCATION(YScreenPos - 1, XScreenPos + 2); std::cout << "+";
	CHANGE_CURSOR_LOCATION(YScreenPos - 1, XScreenPos + 1); std::cout << "-";
	CHANGE_CURSOR_LOCATION(YScreenPos - 1, XScreenPos); std::cout << "-";
	CHANGE_CURSOR_LOCATION(YScreenPos - 1, XScreenPos - 1); std::cout << "-";
	CHANGE_CURSOR_LOCATION(YScreenPos - 1, XScreenPos - 2); std::cout << "+";
	CHANGE_CURSOR_LOCATION(YScreenPos, XScreenPos - 2); std::cout << "|";
	CHANGE_CURSOR_LOCATION(YScreenPos + 1, XScreenPos - 2); std::cout << "+";
	CHANGE_CURSOR_LOCATION(YScreenPos + 1, XScreenPos - 1); std::cout << "-";
	UNBOLD_TEXT;
	LOAD_CURSOR;
}
