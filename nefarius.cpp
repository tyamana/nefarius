#include <stdio.h>
#include "nefarius.h"
#include <string>
#include <list>
#include <algorithm>

using std::string;
using std::list;
using std::swap;

//Card
//----------------------------------------------------------------------------

Card::Card() : cost(0), points(0) {}

Card::~Card() {}

void Card::setCardOwner(Player* pl) { player = pl; }

//Effect


//Player
//----------------------------------------------------------------------------

Player::Player() : money(10), id(0), inGame(true)
{
	cards = new list<Card*>;
}

Player::Player(int id, int money, list<Card*>* cards, bool inGame) : id(id), money(money), spies(spies), cards(cards), inGame(inGame) 
{
	points = 0;

	spies.free_spies = 5;
	spies.spies_map[spying] = 0;
	spies.spies_map[research] = 0;
	spies.spies_map[work] = 0;
	spies.spies_map[invention] = 0;
}

Player::~Player() { delete cards; }

PlayerAction Player::getAction()
{
	int action;
	cin >> action;
	spiesObs->registerAction(id, (PlayerAction)action);
	return (PlayerAction)action;
}

void Player::putSpyOnField(PlayerAction field)
{
	if (spies.free_spies > 0)
	{
		spies.free_spies--;
		spies.spies_map[field] += 1;
	}
	else
		cout << "You don't have any free spies!\n";
}

#pragma warning(disable : 4018)
Card* Player::getCard(int requested_card_number)
{
	if (requested_card_number > cards->size())
	{
		cout << "Too big number, you don't have so many cards!\n";
		return NULL;
	}
	else
	{
		list<Card*>::iterator card_it = cards->begin();
		// ����� �������� ������ ����� ����� �������� ���������������� �� (requested_card_number - 1)
		for (int i = 1; i < requested_card_number; i++)
			card_it++;

		Card* requested_card = *card_it;
		cards->erase(card_it);

		return requested_card;
	}
}

void Player::addCard(Card* card)
{
	cards->push_back(card);
}

//GameController
//----------------------------------------------------------------------------

GameController::GameController()
{
	players = vector<Player*>(0);
	players_number = 0;
	spiesObs = new SpiesObserver();
}

GameController::~GameController()
{
	delete spiesObs;
}

int GameController::createPlayers(int players_num)
{
	players_number = players_num;
	players.resize(players_num);

	for (int i = 0; i < players_num; i++)
	{
		// ������ ������ �����
		list<Card*>* cards = new list<Card*>();
		for(int j = 0; j < 3; j++)
		{
			cards->push_back(deck[current_deck_index]);
			current_deck_index++;
		}
		
		// ������� ������
		Player* player = new Player(i, 10, cards, true);
		if (!player)
			return -1;

		//����������� ��� �� �������� �������
		spiesObs->subscribe(player);
		player->addSpiesObserver(spiesObs);

		players[i] = player;
	}

	return 0;
}

// ������� ������� �����
void GameController::initGameCards()
{
	CardFactory cardFactory;
	for (int i = 0; i < 12; i++)
		deck.push_back(cardFactory.createCard(CardFactory::noEffects));

	//deck.push_back(new Card(12, 5));
	//deck.push_back(new Card(12, 5));
	//deck.push_back(new Card(12, 5));
	//deck.push_back(new Card(8, 4));
	//deck.push_back(new Card(8, 4));
	//deck.push_back(new Card(8, 4));
	//deck.push_back(new Card(16, 6));
	//deck.push_back(new Card(16, 6));
	//deck.push_back(new Card(16, 6));

	current_deck_index = 0;
}

void GameController::deleteGameCards()
{
	for (vector<Card*>::iterator it = deck.begin(); it != deck.end(); it++)
		delete *it;
	for (vector<Card*>::iterator it = throw_deck.begin(); it != throw_deck.end(); it++)
		delete *it;
}

void GameController::shuffleDeckAndSetIndexToZero(DeckType deckType)
{
	switch (deckType)
	{
		case mainDeck:
			random_shuffle(deck.begin(), deck.end());
			break;
		case throwDeck:
			random_shuffle(throw_deck.begin(), throw_deck.end());
			break;
	}
	current_deck_index = 0;
}

#pragma warning(disable : 4018)
void GameController::getCardsToPlayerFromDeck(int player_index, int cards_num)
{
	for (int i = 0; i < cards_num; i++)
	{
		if (current_deck_index >= deck.size())
		{
			shuffleDeckAndSetIndexToZero(throwDeck);
			swap(deck, throw_deck);
			throw_deck.clear();
		}
		else
		{
			players[player_index]->addCard(deck[current_deck_index]);
			current_deck_index++;
		}
	}
}

void GameController::getActions(vector<PlayerAction>& actions)
{
	for (int player = 0; player < players_number; player++)
	{
		cout << "It's your turn, player" << player << "! [Put 0 - spying, 1 - research, 2 - work, 3 - invention]\n";
		PlayerAction action = players[player]->getAction();
		actions[player] = action;
	}
}

// ������ �� �������
// �� ������� ������ ������ �� ����, ������� � ���� ��� ��������� ���� �����-����� ����� ��� ������
// ����� �������� ���� �������
void GameController::payProfitForSpies(vector<PlayerAction> actions)
{
	for (int player = 0; player < players_number; player++)
		spiesObs->handleAction(player);
}

// ���������� �������� ������
void GameController::performPlayerAction(int player_number, PlayerAction action, vector<Effect*>& effects_of_turn)
{
	//������������� ������� - ���������
	switch (action)
	{
		case spying:
			cout << "Player" << player_number << ", choose a field for your spy! [Put 0 - spying, 1 - research, 2 - work, 3 - invention]\n";
			int field;
			cin >> field;
			// ��������� ������ �� ���� "�������" � "������������" ����� ���������
			// �� ���� "������" - �� 1, �� "�����������" - �� 2
			if (field == research || field == spying)
				players[player_number]->putSpyOnField((PlayerAction)field);
			else
			{
				int spy_cost = field == work ? 1 : 2;

				if (players[player_number]->getMoney() >= spy_cost)
				{
					players[player_number]->putSpyOnField((PlayerAction)field);
					players[player_number]->decreaseMoney(spy_cost);
				}
				else
					cout << "Player" << player_number << ", you don't have enough money!\n";
			}
			break;
		case research:
			players[player_number]->increaseMoney(2);
			getCardsToPlayerFromDeck(player_number, 1);
			break;
		case work:
			players[player_number]->increaseMoney(4);
			break;
		case invention:
			cout << "Player" << player_number << ", choose an invention card (You have " << players[player_number]->getCardsNumber() <<")! [Put its number, from 1]\n";
			int card_number;
			cin >> card_number;

			Card* card = players[player_number]->getCard(card_number);
			
			if (!card)
			{
				cout << "You don't have cards!\n";
				break;
			}

			if (players[player_number]->getMoney() >= card->getCost())
			{
				players[player_number]->decreaseMoney(card->getCost());
				players[player_number]->increasePoints(card->getPoints());

				// �������� � effects_of_turn ������� ����������� ����� ������
				set<Effect*>* card_effects = card->getEffects();
				if (card_effects)
					for (set<Effect*>::iterator it = card_effects->begin(); it != card_effects->end(); it++)
					{
						(*it)->setOwner(player_number);
						effects_of_turn.push_back(*it);
					}
			}
			else
			{
				cout << "Player" << player_number << ", you don't have enough money!\n";
				//������ ����� �����
				players[player_number]->addCard(card);
			}
			break;
	}
}

void GameController::performEffect(Effect* eff) { cout << "Effect performed!\n"; }

void GameController::performActions(vector<PlayerAction> actions)
{
	payProfitForSpies(actions);
	
	// �������, ����������� � ���������� ����, ������� ����� ��������� ����� ����, ��� ��� �������� ��������
	vector<Effect*> effects_of_turn;

	// ���������� ��������
	for (int player = 0; player < players_number; player++)
		performPlayerAction(player, actions[player], effects_of_turn);

	// ���������� ��������
	for (vector<Effect*>::iterator it = effects_of_turn.begin(); it != effects_of_turn.end(); it++)
		performEffect(*it);
}

void GameController::printStatistics()
{
	cout << "Statistics:\n";
	for (int player = 0; player < players_number; player++)
		cout << "Player" << player << ": points: " << players[player]->getPoints() <<
									  " money: " << players[player]->getMoney() <<
									  " cards number: " << players[player]->getCardsNumber() << "\n";
}

bool GameController::weHaveWinner()
{
	int winner;
	bool weHaveSingleWinner = false;
	// ���� ���� ����� � ����������� ����� ������ 20, �� ��� ���� ��������� ����
	int pointsOver20 = -1;

	vector<int> points(players_number);
	for (int player = 0; player < players_number; player++)
	{
		int pl_points = players[player]->getPoints();
		if (pl_points > 20)
		{
			if (pl_points > pointsOver20)
			{
				weHaveSingleWinner = true;
				winner = player;
				pointsOver20 = pl_points;
			}
			else if (pl_points == pointsOver20)
				weHaveSingleWinner = false;
		}
		points[player] = pl_points;
	}

	if (weHaveSingleWinner)
		cout << "Game is over! Player" << winner << " wins with " << pointsOver20 << " points!\n";

	printStatistics();

	return weHaveSingleWinner;
}

//SpiesObserver
//---------------------------------------------------------------------------

// ����� "�������" ������� � ������� ������ � ���, ��� ����� ��� ������
void SpiesObserver::handleAction(int playerId)
{
	Player* left_neighbour;
	Player* right_neighbour;
	int players_number = players.size();

	if (playerId == 0)
	{
		left_neighbour = players[players_number - 1];
		right_neighbour = players[playerId + 1];
	}
	else if (playerId == players_number - 1)
	{
		left_neighbour = players[playerId - 1];
		right_neighbour = players[0];
	}
	else
	{
		left_neighbour = players[playerId - 1];
		right_neighbour = players[playerId + 1];
	}


	left_neighbour->increaseMoney(left_neighbour->getSpies().spies_map[actions[playerId]]);
	// ���� ������� ����, �� ����� � ������ ����� - ���� � ��� ��
	if (players_number > 2)
		right_neighbour->increaseMoney(right_neighbour->getSpies().spies_map[actions[playerId]]);
}

Card* CardFactory::createCard(CardFactory::creationFlag flag)
{
	Card* newCard = NULL;

	switch (flag)
	{
		case noEffects:
			int type = rand() % (int)CardFactory::noEffectType3 + (int)CardFactory::noEffectType1;
			switch ((CardFactory::cardType)type)
			{
				case noEffectType1:
					newCard = new Card(12, 5);
					break;
				case noEffectType2:
					newCard = new Card(8, 4);
					break;
				case noEffectType3:
					newCard = new Card(16, 6);
					break;
			}
			break;
	}

	return newCard;
}
