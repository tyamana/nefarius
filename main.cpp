#include <stdio.h>
#include <iostream>
#include "nefarius.h"

using std::vector;

#pragma warning(disable : 4244)
void play_game()
{
	srand(time(0));
	GameController gameCtrl;

	gameCtrl.sendMessage("Game is ready to start!\n");

	gameCtrl.initGameCards();
	gameCtrl.shuffleDeckAndSetIndexToZero(GameController::mainDeck);

	gameCtrl.createPlayers(2);

	do
	{
		gameCtrl.sendMessage("New turn!\n");
		vector<PlayerAction> actions(2);
		gameCtrl.getActions(actions);
		gameCtrl.performActions(actions);

	} while (!gameCtrl.weHaveWinner());

	gameCtrl.deleteGameCards();
}

int main()
{
	play_game();

	system("pause");
	return 0;
}