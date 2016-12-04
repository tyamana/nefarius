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
class GameField;
class Card;

// Random seed
static const unsigned RAND_SEED = 432;

enum PlayerAction {spying, research, work, invention};

// ����� �������� ����������
// �� ������ ������� �������� � ����, ��������� ���������
class GameController // : public IGameController
{
public:
	GameController() {}
	~GameController() {}
	void sendMessage(string message) { std::cout << message; }
	int createPlayers(int players_num);
	void getCardsToPlayerFromDeck(int player, int cards_num);
	void getActions(vector<PlayerAction>* actions);
	void performActions(vector<PlayerAction> actions);
	void payProfitForSpies(vector<PlayerAction> actions);
	void performPlayerAction(int player_number, PlayerAction action, vector<Effect*>& effects_of_turn);
	void performEffect(Effect* eff);
	bool weHaveWinner();
	void printStatistics();
	int play();

	// ������� �����
	vector<Card*> deck;
	// ������ ������
	vector<Card*> throw_deck;
	// ������ � ������ � ������ ������
	int current_deck_index;

	enum DeckType { mainDeck, throwDeck };

	void initGameCards();
	// ������� ������, ���������� �� �����
	void deleteGameCards();
	// ������ ������
	void shuffleDeckAndSetIndexToZero(DeckType);

private:
	vector<Player*> players;
	int players_number;
};

// ����� �����
// ������� ����� �����������. ��������: ���������, ���������� �����, �������.
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

// ����� �����
// � ���� ���� �������������, ���������� �������, �����(����) ����
class Player //: public IPlayer
{
public:
	Player();
	Player(int id, int money, list<Card*>* cards, bool inGame);
	~Player();

	struct Spies
	{
		// �����������: ������� ���� -> ���������� ������� ������ �� ���
		map<PlayerAction, int> spies_map;
		// ������ �� ����������� �� ������� �����
		int free_spies;
	};

	// �������� ������ �� ����� ����
	PlayerAction getAction();
	// ��������� ���������� ����� ������ �� n ������
	void increaseMoney(int n) { money += n; }
	// ��������� ���������� ����� ������ �� n ������
	void decreaseMoney(int n) { money = money - n > 0 ? money - n : 0; }
	// ��������� ���������� ����� ������ �� n ������
	void increasePoints(int n) { points += n; }
	// �������� ������ �� ����
	void putSpyOnField(PlayerAction field);
	// ����� n ���� �� ������
	void getCardsFromDeck(int cards_num);
	// ����� ����� ����� card_number �� ��������� � ������
	Card* getCard(int card_number);
	// �������� ����� �� ������
	void addCard(Card* card);
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
};

// ������ �� ����� �����������.
// ������ �������. ������ ��������� �� ������ ���� �� ���� �����������.
// ����� ������/�������� ������, �����, �������.
class Effect
{
public:
	Effect();
	~Effect();

	void setOwner(int player_number) { owner = player_number; }

	enum GameStuff { gameCard, coin, spy, point };

private:
	// ����� ������, ������� ����������� ���� ������
	int owner;
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
};
