#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "Ship.h"
#include "Position.h"
#include "Player.h"
#include "Player2.h"
#include "Player1.h"

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

class Player2;

Player2::Player2(std::string name) : Player(name), InitialPositionHit(nullptr), LastPositionFired(nullptr) {
	for (int i = 65; i < (65 + 10); ++i) {
		for (int j = 0; j < 10; ++j) {
			Grid.push_back(Position(j, i));
		}
	}
}
int Player2::Attack(bool ShowEnemyShips, bool FastRounds, int k) {
	HIDE_CURSOR;
	int InputY;
	int InputX;
	if (InitialPositionHit == nullptr) {
		InputY = rand() % 10;
		InputX = rand() % 10;
		if (((*EnemyGrid).at(((InputY) * 10) + InputX)).IsHit() == true) {
			return -1;
		}
		int SmallestEnemyShipSizeRemaining = GetSmallestEnemyShipSizeRemaining();
		int Count = 0;
		int CountUp = 0;
		int CountDown = 0;
		int CountLeft = 0;
		int CountRight = 0;
		if (InputX + SmallestEnemyShipSizeRemaining < 10) { //check if it can fit on the right
			for (int i = 1; i < SmallestEnemyShipSizeRemaining; ++i) { //check if there are ships on the right
				if (((*EnemyGrid).at((InputY * 10) + InputX + i)).IsHit() == true) {
					++Count;
					break;
				}
				else {
					CountRight++;
				}
			}
		}
		else ++Count;
		if (InputX - SmallestEnemyShipSizeRemaining >= 0) { //check if it can fit on the left
			for (int i = 1; i < SmallestEnemyShipSizeRemaining; ++i) { //check if there are ships on the left
				if (((*EnemyGrid).at((InputY * 10) + InputX - i)).IsHit() == true) {
					++Count;
					break;
				}
				else {
					CountLeft++;
				}
			}
		}
		else ++Count;
		if (InputY - SmallestEnemyShipSizeRemaining >= 0) { //check if it can fit up
			for (int i = 1; i < SmallestEnemyShipSizeRemaining; ++i) { //check if there are ships above
				if (((*EnemyGrid).at(((InputY - i) * 10) + InputX)).IsHit() == true) {
					++Count;
					break;
				}
				else {
					CountUp++;
				}
			}
		}
		else ++Count;
		if (InputY + SmallestEnemyShipSizeRemaining < 10) { //check if it can fit down
			for (int i = 1; i < SmallestEnemyShipSizeRemaining; ++i) { //check if there are ships below
				if (((*EnemyGrid).at(((InputY + i) * 10) + InputX)).IsHit() == true) {
					++Count;
					break;
				}
				else {
					CountDown++;
				}
			}
		}
		else ++Count;
		if (Count == 4) {
			return -1;
		}
		InputX += CountRight - CountLeft;
		if ((CountRight - CountLeft) == 0) {
			InputY += CountDown - CountUp;
		}
		//
		int CurrentCount = CheckForHitPositions((InputY * 10) + InputX, *EnemyGrid);
		int SmallestCount = CurrentCount;
		int SmallestCountAtIndex = (InputY * 10) + InputX;
		CurrentCount = CheckForHitPositions(((InputY + 1) * 10) + InputX, *EnemyGrid);
		if (CurrentCount < SmallestCount) {
			SmallestCount = CurrentCount;
			SmallestCountAtIndex = ((InputY + 1) * 10) + InputX;
		}
		CurrentCount = CheckForHitPositions(((InputY - 1) * 10) + InputX, *EnemyGrid);
		if (CurrentCount < SmallestCount) {
			SmallestCount = CurrentCount;
			SmallestCountAtIndex = ((InputY - 1) * 10) + InputX;
		}
		CurrentCount = CheckForHitPositions((InputY * 10) + InputX + 1, *EnemyGrid);
		if (CurrentCount < SmallestCount) {
			SmallestCount = CurrentCount;
			SmallestCountAtIndex = (InputY * 10) + InputX + 1;
		}
		CurrentCount = CheckForHitPositions((InputY * 10) + InputX - 1, *EnemyGrid);
		if (CurrentCount < SmallestCount) {
			SmallestCount = CurrentCount;
			SmallestCountAtIndex = (InputY * 10) + InputX - 1;
		}
		InputY = std::floor(SmallestCountAtIndex / 10);
		InputX = SmallestCountAtIndex % 10;
	}
	else {
		if ((LastPositionFired == nullptr) || (LastPositionFired->GetShip() == nullptr)) {
			InputY = InitialPositionHit->GetY() - 65;
			InputX = InitialPositionHit->GetX();
			AttackDirection = rand() % 4;
		}
		else {
			if ((*InitialPositionHit->GetShip()).ShipStatus() == "Sunk") {
				auto EnemyShip = std::find(EnemyShipsRemaining.begin(), EnemyShipsRemaining.end(), (*InitialPositionHit->GetShip()).GetShipSize());
				if (EnemyShip != EnemyShipsRemaining.end()) {
					EnemyShipsRemaining.erase(EnemyShip);
				}
				for (auto it = PositionsHit.begin(); it != PositionsHit.end();) {
					if ((*it)->GetShip()->ShipStatus() == "Sunk") {
						it = PositionsHit.erase(it);
					}
					else ++it;
				}
				if (PositionsHit.size() > 0) {
					InitialPositionHit = PositionsHit.at(0);
					LastPositionFired = nullptr;
					CanShootUp = CanShootDown = CanShootLeft = CanShootRight = true;
				}
				else InitialPositionHit = nullptr;
				return -1;
			}
			else {
				InputY = LastPositionFired->GetY() - 65;
				InputX = LastPositionFired->GetX();
			}
		}
		if ((CanShootUp) && (AttackDirection == 0)) {
			CanShootUp = false;
			if (InputY - 1 >= 0) {
				--InputY;
			}
			else {
				AttackDirection = rand() % 4;
				LastPositionFired = nullptr;
				return -1;
			}
		}
		else if ((CanShootDown) && (AttackDirection == 1)) {
			CanShootDown = false;
			if (InputY + 1 < 10) {
				++InputY;
			}
			else {
				AttackDirection = rand() % 4;
				LastPositionFired = nullptr;
				return -1;
			}
		}
		else if ((CanShootLeft) && (AttackDirection == 2)) {
			CanShootLeft = false;
			if (InputX - 1 >= 0) {
				--InputX;
			}
			else {
				AttackDirection = rand() % 4;
				LastPositionFired = nullptr;
				return -1;
			}
		}
		else if ((CanShootRight) && (AttackDirection == 3)) {
			CanShootRight = false;
			if (InputX + 1 < 10) {
				++InputX;
			}
			else {
				AttackDirection = rand() % 4;
				LastPositionFired = nullptr;
				return -1;
			}
		}
		else {
			if (!(CanShootUp) && !(CanShootDown) && !(CanShootLeft) && !(CanShootRight)) {
				auto EnemyShip = std::find(EnemyShipsRemaining.begin(), EnemyShipsRemaining.end(), (*InitialPositionHit->GetShip()).GetShipSize());
				if (EnemyShip != EnemyShipsRemaining.end()) {
					EnemyShipsRemaining.erase(EnemyShip);
				}
				for (auto it = PositionsHit.begin(); it != PositionsHit.end();) {
					if ((*it)->GetShip()->ShipStatus() == "Sunk") {
						it = PositionsHit.erase(it);
					}
					else ++it;
				}
				if (PositionsHit.size() > 0) {
					InitialPositionHit = PositionsHit.at(0);
					LastPositionFired = nullptr;
					CanShootUp = CanShootDown = CanShootLeft = CanShootRight = true;
				}
				else InitialPositionHit = nullptr;
			}
			AttackDirection = rand() % 4;
			return -1;
		}
	}

	if ((((InputY * 10) + InputX) >= 100) || (((InputY * 10) + InputX) < 0)) {
		return -1;
	}
	else {

		const Position& position = (*EnemyGrid).at((InputY * 10) + InputX);
		
		if (position.IsHit() == false) {
			int x = position.GetX();
			int y = position.GetY() - 65;
			int YScreenPosition = 4 + (y * 2);
			int XScreenPosition = (k * 58) + 7 + (x * 4); //k depends if player1 is also a bot (then it needs to attack right grid, not left)
			LastPositionFired = &position;
			std::cout << Name << " attacked "; SET_TEXT_YELLOW; std::cout << static_cast<char>(y + 65) << x; RESET_COLORS; std::cout << " position. ";
			VisualizeAttackedPosition(XScreenPosition, YScreenPosition, true); //color the grid yellow
			std::this_thread::sleep_for(std::chrono::seconds(1));
			if (InitialPositionHit != nullptr) {
				if (!(CanShootUp) && (AttackDirection == 0)) {
					if (position.GetShip() != nullptr) {
						CanShootUp = true;
						CanShootLeft = false;
						CanShootRight = false;
					}
				}
				else if (!(CanShootDown) && (AttackDirection == 1)) {
					if (position.GetShip() != nullptr) {
						CanShootDown = true;
						CanShootLeft = false;
						CanShootRight = false;
					}
				}
				else if (!(CanShootLeft) && (AttackDirection == 2)) {
					if (position.GetShip() != nullptr) {
						CanShootLeft = true;
						CanShootUp = false;
						CanShootDown = false;
					}
				}
				else if (!(CanShootRight) && (AttackDirection == 3)) {
					if (position.GetShip() != nullptr) {
						CanShootRight = true;
						CanShootUp = false;
						CanShootDown = false;
					}
				}
			}
			if (position.GetShip() != nullptr) {
				SET_TEXT_RED;
				std::cout << "It's a hit";
				RESET_COLORS;
				SAVE_CURSOR;
				PositionsHit.push_back(&position);
				if (InitialPositionHit == nullptr) {
					InitialPositionHit = PositionsHit.at(0);
					CanShootUp = CanShootDown = CanShootLeft = CanShootRight = true;
					AttackDirection = rand() % 4;
				}
				if (not FastRounds) {
					for (int i = 0; i < 4; ++i) {
						if ((not ShowEnemyShips) && (k == 1)) {
							SET_BACKGROUND_BLUE;
						}
						else if ((not ShowEnemyShips) && (k == 0)) {
							SET_BACKGROUND_GRAY;
						}
						else {
							SET_BACKGROUND_GRAY;
						}
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
					if ((not ShowEnemyShips) && (k == 1)) {
						SET_BACKGROUND_BLUE;
					}
					else if ((not ShowEnemyShips) && (k == 0)) {
						SET_BACKGROUND_GRAY;
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
			CURSOR_UP(1); ERASE_TO_END;
			RESET_COLORS;
			VisualizeAttackedPosition(XScreenPosition, YScreenPosition, false); //color the grid to default
		}
		else {
			return -1;
		}
		RESET_COLORS;
		SHOW_CURSOR;
		return (InputY * 10) + InputX;
	}
}

int Player2::CheckForHitPositions(int IndexPos, const std::vector<Position>& Grid) const {
	int Count = 0;
	if ((IndexPos < 0) || (IndexPos >= 100)) {
		return 5;
	}
	if ((Grid.at(IndexPos)).IsHit() == true) { //dont check position thats been attacked since no checks beforehand
		return 5; 
	}
	if ((IndexPos - 10) >= 0) {
		if ((Grid.at(IndexPos - 10)).IsHit() == true) {
			Count++;
		}
	}
	if ((IndexPos + 10) < 100) {
		if ((Grid.at(IndexPos + 10)).IsHit() == true) {
			Count++;
		}
	}
	if (((IndexPos % 10) - 1) >= 0) {
		if ((Grid.at(IndexPos - 1)).IsHit() == true) {
			Count++;
		}
	}
	if (((IndexPos % 10) + 1) < 10) {
		if ((Grid.at(IndexPos + 1)).IsHit() == true) {
			Count++;
		}
	}
	return Count;
}