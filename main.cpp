#include <stdio.h>
#include <iostream>
#include "nefarius.h"

using std::vector;

void play_game()
{
	GameController* gameCtrl = new GameController();

	gameCtrl->sendMessage("Game is ready to start!\n");

	gameCtrl->initGameCards();
	gameCtrl->shuffleDeckAndSetIndexToZero(GameController::mainDeck);

	gameCtrl->createPlayers(2);

	do
	{
		gameCtrl->sendMessage("New turn!\n");
		vector<PlayerAction> actions(2);
		gameCtrl->getActions(actions);
		gameCtrl->performActions(actions);

	} while (!gameCtrl->weHaveWinner());

	gameCtrl->deleteGameCards();
	delete gameCtrl;
}

int main()
{
	play_game();

	system("pause");
	return 0;
}