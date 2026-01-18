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

//////////////////////////////////////////////

class Player1;

Player1::Player1(std::string name) : Player(name) {
	for (int i = 65; i < (65 + 10); ++i) {
		for (int j = 0; j < 10; ++j) {
			Grid.push_back(Position(j, i));
		}
	}
}

int Player1::PlaceShipManually(int ShipIndex) {
	Ship& ShipToPlace = Ships.at(ShipIndex);
	std::cout << "Enter a coordinate to begin placing your " + ShipToPlace.GetName() + " (eg. A2)" << std::endl;
	char InputY = std::cin.get();
	InputY = std::toupper(InputY);
	int InputX;
	if (std::isdigit(std::cin.peek())) {
		std::cin >> InputX;
	}
	else if (std::isalpha(std::cin.peek())) {
		if (std::toupper(std::cin.get()) == 'U') {
			if (std::toupper(std::cin.get()) == 'T') {
				if (std::toupper(std::cin.get()) == 'O') {
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					return -1; //give signal to place auto
				}
				else {
					DisplayError("Invalid coordinate", 2);
					return 0;
				}
			}
			else {
				DisplayError("Invalid coordinate", 2);
				return 0;
			}
		}
		else {
			DisplayError("Invalid coordinate", 2);
			return 0;
		}
	}
	else {
		DisplayError("Invalid coordinate", 2);
		return 0;
	}
	if (((InputY >= 65) && (InputY < 75)) && ((InputX >= 0) && (InputX < 10))) {
		Position& position = Grid.at(((InputY - 65) * 10) + InputX);
		if (position.GetShip() == nullptr) {
			std::cout << "Enter a direction to place the rest of your ship: ";
			int x = position.GetX();
			int y = position.GetY() - 65;
			int PosIndex = (y * 10) + x;
			int ShipSize = ShipToPlace.GetShipSize() - 1;
			bool Up, Down, Left, Right;
			Up = Down = Left = Right = false;
			if (x + ShipSize < 10) {
				Right = true;
				for (int i = 1; i <= ShipSize; ++i) {
					if ((Grid.at((y * 10) + x + i)).GetShip() != nullptr) {
						Right = false;
						break;
					}
				}
			}
			if (x - ShipSize >= 0) {
				Left = true;
				for (int i = 1; i <= ShipSize; ++i) {
					if ((Grid.at((y * 10) + x - i)).GetShip() != nullptr) {
						Left = false;
						break;
					}
				}
			}
			if (y - ShipSize >= 0) {
				Up = true;
				for (int i = 1; i <= ShipSize; ++i) {
					if ((Grid.at(((y - i) * 10) + x)).GetShip() != nullptr) {
						Up = false;
						break;
					}
				}
			}
			if (y + ShipSize < 10) {
				Down = true;
				for (int i = 1; i <= ShipSize; ++i) {
					if ((Grid.at(((y + i) * 10) + x)).GetShip() != nullptr) {
						Down = false;
						break;
					}
				}
			}
			int YScreenPosition = 4 + (y * 2);
			int XScreenPosition = 7 + (x * 4);
			SAVE_CURSOR;
			SET_BACKGROUND_GRAY;
			CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition); std::cout << "#";
			CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition - 1); std::cout << " ";
			CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition + 1); std::cout << " ";
			RESET_COLORS;
			LOAD_CURSOR;
			std::cout << "(";
			if (Up) {
				std::cout << "Up ";
				SAVE_CURSOR;
				SET_BACKGROUND_YELLOW;
				for (int i = 1; i <= ShipSize; ++i) {
					CHANGE_CURSOR_LOCATION(YScreenPosition - i * 2, XScreenPosition); std::cout << " ";
					CHANGE_CURSOR_LOCATION(YScreenPosition - i * 2, XScreenPosition - 1); std::cout << " ";
					CHANGE_CURSOR_LOCATION(YScreenPosition - i * 2, XScreenPosition + 1); std::cout << " ";
				}
				RESET_COLORS;
				LOAD_CURSOR;
			}
			if (Down) {
				std::cout << "Down ";
				SAVE_CURSOR;
				SET_BACKGROUND_YELLOW;
				for (int i = 1; i <= ShipSize; ++i) {
					CHANGE_CURSOR_LOCATION(YScreenPosition + i * 2, XScreenPosition); std::cout << " ";
					CHANGE_CURSOR_LOCATION(YScreenPosition + i * 2, XScreenPosition - 1); std::cout << " ";
					CHANGE_CURSOR_LOCATION(YScreenPosition + i * 2, XScreenPosition + 1); std::cout << " ";
				}
				RESET_COLORS;
				LOAD_CURSOR;
			}
			if (Left) {
				std::cout << "Left ";
				SAVE_CURSOR;
				SET_BACKGROUND_YELLOW;
				for (int i = 1; i <= ShipSize; ++i) {
					CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition - (i * 4)); std::cout << " ";
					CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition - (i * 4) - 1); std::cout << " ";
					CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition - (i * 4) + 1); std::cout << " ";
				}
				RESET_COLORS;
				LOAD_CURSOR;
			}
			if (Right) {
				std::cout << "Right";
				SAVE_CURSOR;
				SET_BACKGROUND_YELLOW;
				for (int i = 1; i <= ShipSize; ++i) {
					CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition + (i * 4)); std::cout << " ";
					CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition + (i * 4) - 1); std::cout << " ";
					CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition + (i * 4) + 1); std::cout << " ";
				}
				RESET_COLORS;
				LOAD_CURSOR;
			}
			std::cout << ")" << std::endl;
			std::string Direction;
			do {
				std::cin >> Direction;
				Direction.at(0) = std::toupper(Direction.at(0));
				for (int i = 0; i <= ShipSize; ++i) {
					Position* Pos;
					if ((Direction == "Up") && (Up)) {
						Pos = &Grid.at(PosIndex - (i * 10));
					}
					else if ((Direction == "Down") && (Down)) {
						Pos = &Grid.at(PosIndex + (i * 10));
					}
					else if ((Direction == "Left") && (Left)) {
						Pos = &Grid.at(PosIndex - i);
					}
					else if ((Direction == "Right") && (Right)) {
						Pos = &Grid.at(PosIndex + i);
					}
					else if (Direction == "Undo") {
						DeleteText(4);
						return 0;
					}
					else {
						DisplayError("You can't place your ship there", 2);
						break;
					}
					if (Pos != nullptr) {
						(*Pos).PutShip(ShipToPlace);
						ShipToPlace.AddPosition(*Pos);
					}
				}
			} while (!(((Direction == "Up") && (Up)) || ((Direction == "Down") && (Down)) || ((Direction == "Left") && (Left)) || ((Direction == "Right") && (Right))));
		}
		else {
			DisplayError("Ship is already present at that position", 2);
			return 0;
		}
		DeleteText(4);
		return 1;
	}
	else {
		DisplayError("Invalid coordinate", 2);
		return 0;
	}
}
int Player1::Attack(bool ShowEnemyShips, bool FastRounds, int k) {
	std::cout << "Enter a coordinate to attack (eg. A2)" << std::endl;
	char InputY = std::cin.get();
	InputY = std::toupper(InputY);
	int InputX;
	if (std::isdigit(std::cin.peek())) {
		std::cin >> InputX;
	}
	else {
		DisplayError("Invalid coordinate", 2);
		return -1;
	}
	if (((InputY >= 65) && (InputY < 75)) && ((InputX >= 0) && (InputX < 10))) {
		const Position& position = (*EnemyGrid).at(((InputY - 65) * 10) + InputX);
		if (position.IsHit() == false) {
			int x = position.GetX();
			int y = position.GetY() - 65;
			int YScreenPosition = 4 + (y * 2);
			int XScreenPosition = (k * 58) + 7 + (x * 4); //k is always 0 in this case (always attacking the right grid)
			HIDE_CURSOR;
			std::cout << Name << " attacked "; SET_TEXT_YELLOW; std::cout << InputY << x; RESET_COLORS; std::cout << " position. ";
			VisualizeAttackedPosition(XScreenPosition,YScreenPosition,true); //color the grid yellow
			std::this_thread::sleep_for(std::chrono::seconds(1));
			if (position.GetShip() != nullptr) {
				SET_TEXT_RED;
				std::cout << "It's a hit";
				RESET_COLORS;
				SAVE_CURSOR;
				if (not FastRounds) {
					for (int i = 0; i < 4; ++i) {
						if (not ShowEnemyShips) {
							SET_BACKGROUND_BLUE;
						}
						else SET_BACKGROUND_GRAY;
						SET_TEXT_WHITE;
						CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition); std::cout << "X";
						std::this_thread::sleep_for(std::chrono::milliseconds(250));
						SET_TEXT_RED;
						CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition); std::cout << "X";
						std::this_thread::sleep_for(std::chrono::milliseconds(250));
						RESET_COLORS;
					}
				}
				else {
					if (not ShowEnemyShips) {
						SET_BACKGROUND_BLUE;
					}
					else SET_BACKGROUND_GRAY;
					SET_TEXT_RED;
					CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition); std::cout << "X";
					std::this_thread::sleep_for(std::chrono::seconds(1));
					RESET_COLORS;
				}
			}
			else {
				SET_TEXT_GREEN;
				std::cout << "It's a miss";
				RESET_COLORS;
				SAVE_CURSOR;
				if (not FastRounds) {
					for (int i = 0; i < 4; ++i) {
						SET_BACKGROUND_BLUE;
						SET_TEXT_WHITE;
						CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition); std::cout << "0";
						std::this_thread::sleep_for(std::chrono::milliseconds(250));
						SET_TEXT_GREEN;
						CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition); std::cout << "0";
						std::this_thread::sleep_for(std::chrono::milliseconds(250));
						RESET_COLORS;
					}
				}
				else {
					SET_BACKGROUND_BLUE;
					SET_TEXT_GREEN;
					CHANGE_CURSOR_LOCATION(YScreenPosition, XScreenPosition); std::cout << "0";
					std::this_thread::sleep_for(std::chrono::seconds(1));
					RESET_COLORS;
				}
			}
			RESET_COLORS;
			LOAD_CURSOR;
			VisualizeAttackedPosition(XScreenPosition, YScreenPosition, false); //put grid color to default
			SHOW_CURSOR;
			DeleteText(2);
		}
		else {
			DisplayError("That position has been attacked already", 2);
			return -1;
		}
		return ((InputY - 65) * 10) + InputX;
	}
	else {
		DisplayError("Invalid coordinate", 2);
		return -1;
	};
}

void Player1::DisplayError(std::string Text, int Time) const {
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	HIDE_CURSOR;
	SET_TEXT_CRIMSON_RED; std::cout << Text; RESET_COLORS;
	std::this_thread::sleep_for(std::chrono::seconds(Time));
	CURSOR_UP(1); ERASE_TO_END;
	SHOW_CURSOR;
}

void Player1::DeleteText(int Lines) const {
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	CURSOR_UP(Lines); ERASE_TO_END;
}
