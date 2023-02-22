#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits.h>
struct Player
{
    int hp;
    int mana;
    int cardsRemaining;
    int rune;
};
enum class CardLocation
{
    Oppenent = -1,
    InHand = 0,  
    Mine = 1
};
struct Card
{
    int id;
    int cardID;
    CardLocation location;
    int cardType;
    int cost;
    int attack;
    int defense;
    int hpChange;
    int hpChangeEnemy;
    int cardDraw;
};
struct State
{
    Player players[2];
    int oppenentHand;
    std::vector <Card> cards;

    bool isInDraft();
};
bool State::isInDraft()
{
    return (players[0].mana == 0);
};

enum class ActionType
{
    Pass,
    Summon,
    Attack,
    Pick
};
struct Action
{
    ActionType type= ActionType::Pass;
    int id = -1;
    int idTarget = -1;

    void pass();
    void summon(int id);
    void attack(int id,int targetId = -1);
    void pick(int id);
    void print(std::ostream& os);
};
void Action::pass()
{
    type = ActionType::Pass;
}
void Action::summon(int id)
{
    type = ActionType::Summon;
    this->id = id;
}
void Action::attack(int id,int targetId)
{
    type = ActionType::Attack;
    this->id = id;
    this->idTarget = targetId;
}
void Action::pick(int id)
{
    type = ActionType::Pick;
    this->id = id;
}
void Action::print(std::ostream& os)
{
    if (type == ActionType::Pass)
    {
        os << "PASS";
    }
    else if (type == ActionType::Summon)
    {
        os << "SUMMON " << id;
    }
    else if (type == ActionType::Attack)
    {
        os << "ATTACK " << id << " " << idTarget;
    }
    else if (type == ActionType::Pick)
    {
        os << "Pick " << id;
    }
    else
    {
        throw "Undefined Action";
    }
}
struct Turn
{
    std::vector<Action> actions;

    void clear() { actions.clear(); };
    void print(std::ostream& os);
};
void Turn::print(std::ostream& os)
{   
    if (actions.size() == 0)
    {
        os << "PASS";
        return;
    }

    for (Action& action : actions)
    {
        action.print(os);
        os << ";";
    }
}
struct Agent
{
    State state;
    Turn bestTurn;

    void think();
    void read();
    void print();
};
void Agent::read()
{
    for (int i = 0; i < 2; i++) {
        Player player = state.players[i];
        int player_health;
        int player_mana;
        int player_deck;
        int player_rune;
        int player_draw;
        std::cin >> player_health >> player_mana >> player_deck >> player_rune >> player_draw; std::cin.ignore();
        player.hp = player_health;
        player.mana = player_mana;
        player.cardsRemaining = player_deck;
        player.rune = player_rune;
    }
    int opponent_hand;
    int opponent_actions;
    std::cin >> opponent_hand >> opponent_actions; std::cin.ignore();
    state.oppenentHand = opponent_hand;
     
    for (int i = 0; i < opponent_actions; i++) {
        std::string card_number_and_action;
        getline(std::cin, card_number_and_action);
    }
    int card_count;
    std::cin >> card_count; std::cin.ignore();
    state.cards.clear();
    for (int i = 0; i < card_count; i++) {
        state.cards.emplace_back();
        Card& card = state.cards.back();
        int card_number;
        int instance_id;
        int location;
        int card_type;
        int cost;
        int attack;
        int defense;
        std::string abilities;
        int my_health_change;
        int opponent_health_change;
        int card_draw;
        std::cin >> card_number >> instance_id >> location >> card_type >> cost >> attack >> defense >> abilities >> my_health_change >> opponent_health_change >> card_draw; std::cin.ignore();
        card.cardID = card_number;
        card.id = instance_id;
        card.location = (CardLocation)location;
        card.cardType = card_type;
        card.cost = cost;
        card.attack = attack;
        card.defense = defense;
        card.hpChange = my_health_change;
        card.hpChangeEnemy = opponent_health_change;
        card.cardDraw = card_draw;
    }
}
void Agent::think()
{   
    bestTurn.clear();

    if (state.isInDraft())
    {
        return;
    }
    
    //Battle Phase
    int myMana = state.players[0].mana;
    Card* bestCard = nullptr;
    int bestScore = -INT_MAX;
    for (Card& card : state.cards)
    {
        if (card.location != CardLocation::InHand) continue;
        if (card.cost > myMana) continue;
        int score = card.cost;
        if (score > bestScore)
        {
            bestScore = score;
            bestCard = &card;
        }
        if (bestCard != nullptr)
        {
            bestTurn.actions.emplace_back();
            Action& action = bestTurn.actions.back();
            action.summon(bestCard->id);
        }
    }
}
void Agent::print()
{
    bestTurn.print(std::cout);
    std::cout << std::endl;
}
using namespace std;

int main()
{
    Agent agent;
    while (1) {
        agent.read();
        agent.print();
    }
}