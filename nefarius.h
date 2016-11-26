#include <stdio.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <map>
#include <time.h>
#include <algorithm>
//#include "game.h"

using namespace std;

class Effect;
class Player;
class GameField;
class Card;

//random seed
static const unsigned RAND_SEED = 432;
//������� �����
static vector<Card*>* DECK;
//������ � ������ � ������ ������
static int CURRENT_DECK_INDEX;

enum PlayerAction {spying, research, work, invention};

//����� �������� ����������
//�� ������ ������� ��� ��� ��� ���� �������� � ����, ��������� ���������
class GameController // : public IGameController
{
public:
	GameController();
	~GameController();
	void sendMessage(string message) { std::cout << message; }
	int createPlayers(int players_num);
	int createGameField();
	void getActions(vector<PlayerAction>* actions);
	void performActions(vector<PlayerAction> actions);
	void payProfitForSpies(vector<PlayerAction> actions);
	int play();

private:
	vector<Player*> players;
	int players_number;
};

//����� �����
//������� ����� �����������. ��������: ���������, ���������� �����, �������.
class Card
{
public:
	Card();
	Card(int cost, int points/*, set<Effect> effects*/) : cost(cost), points(points) /*, effects(effects)*/ {}
	int getCost() const { return cost; }
	int getPoints() const { return points; }
	void setCardOwner(Player*);
	~Card();
private:
	int cost;
	int points;
	Player* player;
	//set<Effect> effects;
};

//����� �����
//� ���� ���� �������������, ���������� �������, �����(����) ����
class Player //: public IPlayer
{
public:
	Player();
	Player(int id, int money, list<Card*>* cards, bool inGame);
	~Player();

	struct Spies
	{
		//�����������: ������� ���� -> ���������� ������� ������ �� ���
		map<PlayerAction, int> spies_map;
		//������ �� ����������� �� ������� �����
		int free_spies;
	};

	//�������� ������ �� ����� ����
	PlayerAction getAction();
	bool isInGame() { return inGame; }

private:
	int id;
	int money;
	list<Card*>* cards;
	Spies spies;
	bool inGame;
	int points;
};

// ������ �� ����� �����������.
// ������ �������. ������ ��������� �� ������ ���� �� ���� �����������.
// ����� ������/�������� ������, �����, �������.
class Effect
{
public:
	Effect();
	~Effect();

	void applyEffect();

	enum GameStuff { gameCard, coin, spy, point };

private:
	bool act_on_owner; // ��������: true - �� ����, false - �� ���� �����������
	bool add;          // true - ������ ��������� ���-����, false - ��������
	// ������ ������ ��������� ���-���� � ����������� �� ���������� ����-���� �������.
	// ��������, ��������� ������ �� ������ ����� � ����.
	// ��������������, true - ����� ������, false - ���.
	bool dependent;
	// ����� ����, ��� ��������� ��� �������� ������.
	// ������������� �������� ����������� ��������������� ��������� ���.
	list<GameStuff> to_add_or_decrease;
	// ���, �� �������� ������� ���������� ������������
	GameStuff on_which_depend;
	int card; // ����� �����, � ������� ���������� ������
};

//������� ������� �����
static void initGameCards()
{
	DECK = new vector<Card*>();

	DECK->push_back(new Card(12, 5));
	DECK->push_back(new Card(12, 5));
	DECK->push_back(new Card(12, 5));
	DECK->push_back(new Card(8, 4));
	DECK->push_back(new Card(8, 4));
	DECK->push_back(new Card(8, 4));
	DECK->push_back(new Card(16, 6));
	DECK->push_back(new Card(16, 6));
	DECK->push_back(new Card(16, 6));

	CURRENT_DECK_INDEX = 0;
}

//������� ������, ���������� �� �����
static void deleteGameCards()
{
	for (vector<Card*>::iterator it = DECK->begin(); it != DECK->end(); it++)
		delete *it;

	delete DECK;
}

//������ ������
static void shuffleDeck()
{
	srand(time(0));
	random_shuffle(DECK->begin(), DECK->end());
	CURRENT_DECK_INDEX = 0;
}