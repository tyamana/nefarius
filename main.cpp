#include <stdio.h>
#include <iostream>
#include <string>
//include "game.h"
#include "nefarius.h"
//#include "nefarius.cpp"

using std::cin;
using std::cout;
using std::string;
using std::vector;

void play_game()
{
	GameController* gameCtrl = new GameController();

	gameCtrl->sendMessage("Game is ready to start!\n");

	initGameCards();
	shuffleDeck();

	gameCtrl->createPlayers(2);
	gameCtrl->createGameField();

	//vector<GameController::PlayerAction> actions(2);
	//gameCtrl->getActions(&actions);
	//gameCtrl->performActions(actions);

	deleteGameCards();
	delete gameCtrl;
}

int main()
{
	play_game();

	return 0;
}