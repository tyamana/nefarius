#include <stdio.h>
#include "nefarius.h"
#include <string>
#include <list>

using std::string;
using std::list;

//Card

Card::Card() : cost(0), points(0) {}

Card::~Card() {}

void Card::setCardOwner(Player* pl) { player = pl; }

//Effect



//Player

Player::Player() : money(10), id(0), inGame(true)
{
	cards = new list<Card*>;
}

Player::Player(int id, int money, list<Card*>* cards, bool inGame) : id(id), money(money), spies(spies), cards(cards), inGame(inGame) 
{
	spies.free_spies = 5;
	spies.spies_map[spying] = 0;
	spies.spies_map[research] = 0;
	spies.spies_map[work] = 0;
	spies.spies_map[invention] = 0;
}

PlayerAction Player::getAction()
{
	int action;
	cin >> action;
	return (PlayerAction)action;
}

Player::~Player() { delete cards; }

//GameController

GameController::GameController() {}

int GameController::createPlayers(int players_num)
{
	players_number = players_num;
	players.resize(players_num);
	for (int i = 0; i < players_num; i++)
	{
		//создаем 
		list<Card*>* cards = new list<Card*>();
		for(int j = 0; j < 3; j++)
		{
			cards->push_back((*DECK)[CURRENT_DECK_INDEX]);
			
			CURRENT_DECK_INDEX++;
		}
		
		Player* player = new Player(i, 10, cards, true);
		if (!player)
			return -1;
		players[i] = player;
	}

	return 0;
}

void GameController::getActions(vector<PlayerAction>* actions)
{
	for (int i = 0; i < players_number; i++)
	{
		cout << "Ваш ход, игрок", i, "! [ВВедите 0 - spying, 1 - research, 2 - work или 3 - invention]";
		PlayerAction action = players[i]->getAction();
		(*actions)[i] = action;
	}
}

void GameController::payProfitForSpies(vector<PlayerAction> actions)
{

}

void GameController::performActions(vector<PlayerAction> actions)
{
	payProfitForSpies(actions);
}