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

// ����� �������� ����������
// �� ������ ������� �������� � ����, ��������� ���������
class GameController // : public IGameController
{
public:
	// ������� - �����.
	GameController();
	~GameController();
	void sendMessage(string message) { std::cout << message; }
	// ������������� ������� ����
	void initGameCards();
	// ������� ������, ���������� �� �����
	void deleteGameCards();
	int createPlayers(int players_num);
	// �������� �������� �������
	void getActions(vector<PlayerAction>& actions);
	// ��������� �������� �������
	void performActions(vector<PlayerAction> actions);
	// ��������, ����������� �� ����������
	bool weHaveWinner();

	enum DeckType { mainDeck, throwDeck };
	// ������ ������
	void shuffleDeckAndSetIndexToZero(DeckType);

	// ������� �����
	vector<Card*> deck;
	// ������ ������
	vector<Card*> throw_deck;
	// ������ � ������ � ������ ������
	int current_deck_index;

private:
	// ��� �������
	void getCardsToPlayerFromDeck(int player, int cards_num);
	void payProfitForSpies(vector<PlayerAction> actions);
	void performPlayerAction(int player_number, PlayerAction action, vector<Effect*>& effects_of_turn);
	void performEffect(Effect* eff);
	void printStatistics();

	vector<Player*> players;
	int players_number;
	SpiesObserver* spiesObs;
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
	// ��������� �������� �������
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

// ������� observer
// � ������, ���� ����� ������ �� ������������ ����, �������� �� ���� ��� ������
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

// ������� �������
// ������ �����
class CardFactory
{
public:
	CardFactory() {}
	~CardFactory() {}

	enum cardType { noEffectType1, noEffectType2, noEffectType3 };
	enum creationFlag { noEffects };

	Card* createCard(creationFlag);
};
