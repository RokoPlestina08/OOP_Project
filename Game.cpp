#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "Ship.h"
#include "Position.h"
#include "Player.h"
#include "Player1.h"
#include "Player2.h"
#include "Game.h"

//////////////////ANSI CODES//////////////////

#define RESET_COLORS (std::cout << "\033[0m")
#define SET_BACKGROUND_BLUE (std::cout << "\033[44m")
#define SET_BACKGROUND_GRAY (std::cout << "\033[48;5;242m")
#define SET_BACKGROUND_DARK_BLUE (std::cout << "\033[48;5;18m")
#define CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition) (std::cout << "\033[" + std::to_string(YAxisPosition) << ";" + std::to_string(XAxisPosition) << "H")
#define SET_TEXT_GREEN (std::cout << "\033[38;5;118m")
#define SET_TEXT_RED (std::cout << "\033[38;5;196m")
#define SET_TEXT_DARK_RED (std::cout << "\033[38;5;124m")
#define SET_TEXT_YELLOW (std::cout << "\033[33m")
#define CURSOR_DOWN(x) (std::cout << "\033[" << std::to_string(x) << "E")
#define HIDE_CURSOR (std::cout << "\033[?25l")
#define SHOW_CURSOR (std::cout << "\033[?25h")
#define SAVE_CURSOR (std::cout << "\033[s")
#define LOAD_CURSOR (std::cout << "\033[u")
#define ERASE_TO_END (std::cout << "\033[0J")
#define CURSOR_UP(x) (std::cout << "\033[" + std::to_string(x) << "F\033[0J")

//////////////////////////////////////////////

class Game;

Game::Game() : Player1Grid(nullptr), Player2Grid(nullptr), Round(0), Turn("Player1"), GridSize(10), ShowEnemyShips(false), FastRounds(false), Player1AsBot(false) {};

void Game::PrintGrids() {
	RESET_COLORS;
	int Player1GridIndex = 0; int Player2GridIndex = 0;
	int XCount = 0;
	for (int i = 0; i <= (GridSize * 2) + 2; ++i) {
		for (int k = 1; k <= 2; ++k) {
			for (int j = 0; j <= (GridSize * 4) + 4; ++j) {
				if (j % 4 == 0) {
					if (i % 2 == 0)
						std::cout << "+";
					else
						std::cout << "|";
				}
				else if ((j % 2 == 0) && (i % 2 == 1)) {
					if (i < 2) {
						if (!(j < 4)) {
							std::cout << (Player1Grid->at(XCount % 10)).GetX();
							++XCount;
						}
						else std::cout << " ";
					}
					else if (j < 4) {
						if (k == 1) {
							std::cout << (Player1Grid->at(Player1GridIndex)).GetY();
						}
						else {
							std::cout << (Player2Grid->at(Player2GridIndex)).GetY();
						}
					}
					else {
						SET_BACKGROUND_BLUE;
						std::cout << "~";
						RESET_COLORS;
						if (k == 1) {
							++Player1GridIndex;
						}
						else {
							++Player2GridIndex;
						}
					}
				}
				else if (i % 2 == 0)
					std::cout << "-";
				else if ((i % 2 == 1) && ((j < 4) || (i < 2))) {
					std::cout << " ";
				}
				else if (i % 2 == 1) {
					SET_BACKGROUND_BLUE;
					std::cout << " ";
					RESET_COLORS;
				}
			}
			std::cout << "             ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Game::UpdateGrid() {
	HIDE_CURSOR;
	for (int i = 0; i < 100; ++i) {
		Position Pos = Player1Grid->at(i);
		for (int k = 0; k < 2; ++k) {
			int YAxisPosition = 4 + ((Pos.GetY() - 65) * 2);
			int XAxisPosition = (k * 58) + 7 + (Pos.GetX() * 4);
			CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition);
			if (Pos.GetShip() == nullptr) {
				if (Pos.IsHit()) {
					SET_TEXT_GREEN; SET_BACKGROUND_BLUE;
					std::cout << "0";
				}
				else {
					SET_BACKGROUND_BLUE;
					std::cout << "~";
				}
				CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition - 1); std::cout << " ";
				CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition + 1); std::cout << " ";
				RESET_COLORS;
			}
			else {
				if (Pos.GetShip()->ShipStatus() == "Sunk") {
					SET_TEXT_DARK_RED; SET_BACKGROUND_DARK_BLUE;
					std::cout << "X";
				}
				else if (Pos.IsHit()) {
					if ((k == 0) || (ShowEnemyShips)) {
						SET_TEXT_RED; SET_BACKGROUND_GRAY;
					}
					else {
						SET_TEXT_RED; SET_BACKGROUND_BLUE;
					}
					std::cout << "X";
				}
				else {
					if ((k == 0) || (ShowEnemyShips)) {
						SET_BACKGROUND_GRAY;
						std::cout << "#";
					}
					else {
						SET_BACKGROUND_BLUE;
						std::cout << "~";
					}
				};
				CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition - 1); std::cout << " ";
				CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition + 1); std::cout << " ";
				RESET_COLORS;
			}
			Pos = Player2Grid->at(i);
		}
	}
	RESET_COLORS;
	CURSOR_DOWN(3);
	SHOW_CURSOR;
}

void Game::ShipDestroyedVisual(const Ship& ship, std::string PlayerName, int k) {
	for (int i = 0; i < 6; ++i) {
		HIDE_CURSOR;
		SAVE_CURSOR;
		std::cout << PlayerName << "'s " << ship.GetName() << " has been sunk!";
		for (int j = 0; j < ship.GetShipSize(); ++j) {
			Position Pos = ship.GetShipPosition(j);
			int YAxisPosition = 4 + ((Pos.GetY() - 65) * 2);
			int XAxisPosition = (k * 58) + 7 + (Pos.GetX() * 4);
			if (k == 0) {
				CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition);
				SET_TEXT_RED; SET_BACKGROUND_GRAY;
				std::cout << "X";
				CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition - 1); std::cout << " ";
				CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition + 1); std::cout << " ";
				RESET_COLORS;
			}
			if (k == 1) {
				CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition);
				if (not ShowEnemyShips) {
					SET_BACKGROUND_BLUE;
				}
				else SET_BACKGROUND_GRAY;
				SET_TEXT_RED;
				std::cout << "X";
				CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition - 1); std::cout << " ";
				CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition + 1); std::cout << " ";
				RESET_COLORS;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		LOAD_CURSOR;
		SET_TEXT_YELLOW;
		std::cout << PlayerName << "'s " << ship.GetName() << " has been sunk!";
		RESET_COLORS;
		for (int j = 0; j < ship.GetShipSize(); ++j) {
			Position Pos = ship.GetShipPosition(j);
			int YAxisPosition = 4 + ((Pos.GetY() - 65) * 2);
			int XAxisPosition = (k * 58) + 7 + (Pos.GetX() * 4);
			CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition);
			SET_TEXT_DARK_RED; SET_BACKGROUND_DARK_BLUE;
			std::cout << "X";
			CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition - 1); std::cout << " ";
			CHANGE_CURSOR_LOCATION(YAxisPosition, XAxisPosition + 1); std::cout << " ";
			RESET_COLORS;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		RESET_COLORS;
		LOAD_CURSOR;
		ERASE_TO_END;
		SHOW_CURSOR;
	}
}

void Game::StartGame() {
	RESET_COLORS;
	//////////
	SAVE_CURSOR;
	bool Selected = false;
	while (not Selected) {
		std::cout << "Select options for the game: Fast rounds (Y/N), Bot as player 1 (Y/N), Show enemy ships (Y/N). | Eg. YYY,YNY,YYN,NYY" << std::endl;
		char Option1 = std::cin.peek();
		char Option2 = '\0';
		char Option3 = '\0';
		if (std::isalpha(Option1)) {
			Option1 = std::cin.get();
			Option2 = std::cin.peek();
			if (std::isalpha(Option2)) {
				Option2 = std::cin.get();
				Option3 = std::cin.peek();
				if (std::isalpha(Option3)) {
					Option3 = std::cin.get();
				}
			}
		}
		Option1 = std::toupper(Option1);
		Option2 = std::toupper(Option2);
		Option3 = std::toupper(Option3);
		if (((Option1 == 'Y') || (Option1 == 'N')) && ((Option2 == 'Y') || (Option2 == 'N')) && ((Option3 == 'Y') || (Option3 == 'N'))) {
			if (Option1 == 'Y') {
				FastRounds = true;
			}
			if (Option2 == 'Y') {
				Player1AsBot = true;
			}
			if (Option3 == 'Y') {
				ShowEnemyShips = true;
			}
			LOAD_CURSOR;
			ERASE_TO_END;
			Selected = true;
		}
		else {
			HIDE_CURSOR;
			SET_TEXT_RED;
			std::cout << "Invalid options";
			RESET_COLORS;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			CURSOR_UP(2);
			ERASE_TO_END;
			SHOW_CURSOR;
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	if (not Player1AsBot) {
		P1 = std::make_unique<Player1>(Player1("Player 1"));
		Player1Grid = P1->GetGrid();
	}
	else {
		P1_Alt = std::make_unique<Player2>(Player2("Player 1"));
		Player1Grid = P1_Alt->GetGrid();
	}
	P2 = std::make_unique<Player2>(Player2("Player 2"));
	Player2Grid = P2->GetGrid();

	if (not Player1AsBot) {
		P1->PutEnemyGrid(Player2Grid);
	}
	else {
		P1_Alt->PutEnemyGrid(Player2Grid);
	}
	P2->PutEnemyGrid(Player1Grid);

	PrintGrids();

	for (int i = 0; i < 5; ++i) {
		int Status = 0;
		do {
			if (not Player1AsBot) {
				Status = P1->PlaceShipManually(i);
			}
			else {
				Status = P1_Alt->PlaceShip(i);
			}

			if (Status == -1) { //if player types auto to place automatically
				CURSOR_UP(2);
				ERASE_TO_END;
				for (int j = i; j < 5; ++j) {
					Status = 0;
					do {
						Status = P1->PlaceShip(j);
						UpdateGrid();
					} while (Status == 0);
					++i;
				}
				Status = 1;
			}
			UpdateGrid();
		} while (Status == 0);
	}
	for (int i = 0; i < 5; ++i) {
		int Status = 0;
		do {
			Status = P2->PlaceShip(i);
			UpdateGrid();
		} while (Status == 0);
	}
	while (1) {
		int Index = -1;
		if (not Player1AsBot) {
			if (P1->ShipsLeft() > 0) {
				do {
					Index = P1->Attack(ShowEnemyShips, FastRounds, 1);
					UpdateGrid();
				} while (Index < 0);
				P2->RegisterAttack(Index);
				UpdateGrid();
				for (int i = 0; i < 5; ++i) {
					if (P2->GetShip(i).ShipStatus() == "Destroyed") {
						P2->SinkShip(i);
						ShipDestroyedVisual(P2->GetShip(i), P2->GetName(), 1);
						UpdateGrid();
					}
				}
			}
			else break;
		}
		else {
			if (P1_Alt->ShipsLeft() > 0) {
				do {
					Index = P1_Alt->Attack(ShowEnemyShips, FastRounds, 1);
					UpdateGrid();
				} while (Index < 0);
				P2->RegisterAttack(Index);
				UpdateGrid();
				for (int i = 0; i < 5; ++i) {
					if (P2->GetShip(i).ShipStatus() == "Destroyed") {
						P2->SinkShip(i);
						ShipDestroyedVisual(P2->GetShip(i), P2->GetName(), 1);
						UpdateGrid();
					}
				}
			}
			else break;
		}
		Index = -1;
		if (P2->ShipsLeft() > 0) {
			do {
				Index = P2->Attack(ShowEnemyShips, FastRounds, 0);
				UpdateGrid();
			} while (Index < 0);
			if (not Player1AsBot) {
				P1->RegisterAttack(Index);
			}
			else {
				P1_Alt->RegisterAttack(Index);
			}
			UpdateGrid();
			for (int i = 0; i < 5; ++i) {
				if (not Player1AsBot) {
					if (P1->GetShip(i).ShipStatus() == "Destroyed") {
						P1->SinkShip(i);
						ShipDestroyedVisual(P1->GetShip(i), P1->GetName(), 0);
						UpdateGrid();
					}
				}
				else {
					if (P1_Alt->GetShip(i).ShipStatus() == "Destroyed") {
						P1_Alt->SinkShip(i);
						ShipDestroyedVisual(P1_Alt->GetShip(i), P1_Alt->GetName(), 0);
						UpdateGrid();
					}
				}
			}
		}
		else break;
		++Round;
	}
	std::cout << "Game over" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	if (P2->ShipsLeft() == 0) {
		SET_TEXT_GREEN;
		std::cout << "You won" << std::endl;
	}
	else {
		SET_TEXT_RED;
		std::cout << "You lost" << std::endl;
	}
	std::this_thread::sleep_for(std::chrono::seconds(2));
	RESET_COLORS;
	std::cout << "Game lasted " << Round << " Rounds" << std::endl;
	ShowEnemyShips = true;
	UpdateGrid();
	std::this_thread::sleep_for(std::chrono::seconds(8));
}
