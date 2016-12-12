#include <stdio.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <time.h>
#include <algorithm>
#include <new>

using namespace std;

class Effect;
class Player;
class Card;
class SpiesObserver;

// Random seed
static const unsigned RAND_SEED = 432;

enum PlayerAction {spying, research, work, invention};

// Класс игрового диспетчера
// Он отдает команды действий в игре, руководит процессом
class GameController // : public IGameController
{
public:
	// Паттерн - фасад.
	GameController();
	~GameController();
	void sendMessage(string message) { std::cout << message; }
	// Инициализация игровых карт
	void initGameCards();
	// Очистка памяти, выделенной на карты
	void deleteGameCards();
	int createPlayers(int players_num);
	// Получить действия игроков
	void getActions(vector<PlayerAction>& actions);
	// Выполнить действия игроков
	void performActions(vector<PlayerAction> actions);
	// Проверка, определился ли победитель
	bool weHaveWinner();

	enum DeckType { mainDeck, throwDeck };
	// Мешаем колоду
	void shuffleDeckAndSetIndexToZero(DeckType);

	// Игровые карты
	vector<Card*> deck;
	// Колода сброса
	vector<Card*> throw_deck;
	// Индекс в колоде в данный момент
	int current_deck_index;

private:
	// Под фасадом
	void getCardsToPlayerFromDeck(int player, int cards_num);
	void payProfitForSpies(vector<PlayerAction> actions);
	void performPlayerAction(int player_number, PlayerAction action, vector<Effect*>& effects_of_turn);
	void performEffect(Effect* eff);
	void printStatistics();

	vector<Player*> players;
	int players_number;
	SpiesObserver* spiesObs;
};

// Класс карты
// Игровая карта изобретений. Атрибуты: стоимость, количество очков, эффекты.
class Card
{
public:
	Card();
	Card(int cost, int points, set<Effect*>* effects = NULL) : cost(cost), points(points) , effects(effects) {}
	int getCost() const { return cost; }
	int getPoints() const { return points; }
	void setCardOwner(Player*);
	set<Effect*>* getEffects() const { return effects; }
	~Card();
private:
	int cost;
	int points;
	Player* player;
	set<Effect*>* effects;
};

// Класс игрок
// У него есть идентификатор, количество шпионов, набор(лист) карт
class Player //: public IPlayer
{
public:
	Player();
	Player(int id, int money, list<Card*>* cards, bool inGame);
	~Player();

	struct Spies
	{
		// Отображение: игровое поле -> количество шпионов игрока на нем
		map<PlayerAction, int> spies_map;
		// Шпионы не находящиеся на игровых полях
		int free_spies;
	};

	// Действие игрока во время хода
	PlayerAction getAction();
	// Увеличить количество денег игрока на n единиц
	void increaseMoney(int n) { money += n; }
	// Уменьшить количество денег игрока на n единиц
	void decreaseMoney(int n) { money = money - n > 0 ? money - n : 0; }
	// Увеличить количество очков игрока на n единиц
	void increasePoints(int n) { points += n; }
	// Посадить шпиона на поле
	void putSpyOnField(PlayerAction field);
	// Взять n карт из колоды
	void getCardsFromDeck(int cards_num);
	// Взять карту номер card_number из имеющихся у игрока
	Card* getCard(int card_number);
	// Добавить карту из колоды
	void addCard(Card* card);
	// Привязать обсервер шпионов
	void addSpiesObserver(SpiesObserver* spiesObserver) { spiesObs = spiesObserver; }
	// Getters
	Spies getSpies() { return spies; }
	int getMoney() { return money; }
	int getPoints() { return points; }
	int getCardsNumber() { return cards->size(); }

	bool isInGame() { return inGame; }

private:
	int id;
	int money;
	list<Card*>* cards;
	Spies spies;
	bool inGame;
	int points;
	SpiesObserver* spiesObs;
};

// Эффект на карте изобретения.
// Бывают разными. Эффект действует на игрока либо на всех противников.
// Может давать/отнимать деньги, карты, шпионов.
class Effect
{
public:
	Effect();
	~Effect();

	void setOwner(int player_number) { owner = player_number; }

	enum GameStuff { gameCard, coin, spy, point };

private:
	// Номер игрока, который инициировал этот эффект
	int owner;
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
};

// Паттерн observer
// В случае, если игрок сходил на определенное поле, сообщает об этом его соседу
class SpiesObserver
{
public:
	SpiesObserver() { players = vector<Player*>(0); }
	~SpiesObserver() {}

	void subscribe(Player* player) { players.push_back(player); actions.push_back((PlayerAction)0); }
	void registerAction(int playerId, PlayerAction action) { actions[playerId] = action; }
	void handleAction(int playerId);

private:
	vector<Player*> players;
	vector<PlayerAction> actions;
};

// Паттерн фабрика
// Содает карты
class CardFactory
{
public:
	CardFactory() {}
	~CardFactory() {}

	enum cardType { noEffectType1, noEffectType2, noEffectType3 };
	enum creationFlag { noEffects };

	Card* createCard(creationFlag);
};
