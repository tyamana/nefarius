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
//игровые карты
static vector<Card*>* DECK;
//индекс в колоде в данный момент
static int CURRENT_DECK_INDEX;

enum PlayerAction {spying, research, work, invention};

//класс игрового диспетчера
//он отдает команды для тех или иных действий в игре, руководит процессом
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

//класс карты
//Игровая карта изобретений. Атрибуты: стоимость, количество очков, эффекты.
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

//класс игрок
//У него есть идентификатор, количество шпионов, набор(лист) карт
class Player //: public IPlayer
{
public:
	Player();
	Player(int id, int money, list<Card*>* cards, bool inGame);
	~Player();

	struct Spies
	{
		//отоюражение: игровое поле -> количество шпионов игрока на нем
		map<PlayerAction, int> spies_map;
		//шпионы не находящиеся на игровых полях
		int free_spies;
	};

	//Действие игрока во время хода
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

// Эффект на карте изобретения.
// Бывают разными. Эффект действует на игрока либо на всех противников.
// Может давать/отнимать деньги, карты, шпионов.
class Effect
{
public:
	Effect();
	~Effect();

	void applyEffect();

	enum GameStuff { gameCard, coin, spy, point };

private:
	bool act_on_owner; // действие: true - на себя, false - на всех противников
	bool add;          // true - эффект добавляет что-либо, false - забирает
	// Иногда эффект добавляет что-либо в зависимости от количества чего-либо другого.
	// Например, добавляет монету за каждую карту в руке.
	// Соответственно, true - такой эффект, false - нет.
	bool dependent;
	// Набор того, что добавляет или забирает эффект.
	// Повторяющиеся элементы добавляются соответствующее коичество раз.
	list<GameStuff> to_add_or_decrease;
	// Тип, от которого зависит количество добавляемого
	GameStuff on_which_depend;
	int card; // номер карты, к которой принадежит эффект
};

//создаем игровые карты
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

//очистка памяти, выделенной на карты
static void deleteGameCards()
{
	for (vector<Card*>::iterator it = DECK->begin(); it != DECK->end(); it++)
		delete *it;

	delete DECK;
}

//мешаем колоду
static void shuffleDeck()
{
	srand(time(0));
	random_shuffle(DECK->begin(), DECK->end());
	CURRENT_DECK_INDEX = 0;
}