#include <iostream>
#include <vector>
#include <map>

const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
using namespace std;

struct Character {
    string id;
    int hitPts = 0;
    int dmg = 0;
    int armor = 0;
    int mana = 0;
};
struct Game {
    vector<Character> &characters;
    int turn;
};
struct Attack {
    string name;
    int manaCost;
    int dmg = 0;
    int armor = 0;
    int hitPts = 0;
    int duration = -1;
    int manaRestore = 0;
};
// name, cost, dmg, armor, hitPts, duration;
vector<Attack> playerAttacks = {
    // Magic Missile costs 53 mana. It instantly does 4 damage.
    { "Missile", 53, 4, 0, 0, -1, 0 },
    // Drain costs 73 mana. It instantly does 2 damage and heals you for 2 hit points.
    { "Drain--", 73, 2, 0, 2, -1, 0 },
    // Shield costs 113 mana. It starts an effect that lasts for 6 turns. While it is active, your armor is increased by 7.
    { "Shield", 113, 0, 7, 0, 6, 0 },
    // Poison costs 173 mana. It starts an effect that lasts for 6 turns. At the start of each turn while it is active, it deals the boss 3 damage.
    { "Poison", 173, 3, 0, 0, 6, 0 },
    // Recharge costs 229 mana. It starts an effect that lasts for 5 turns. At the start of each turn while it is active, it gives you 101 new mana.
    { "Rechar", 229, 0, 0, 0, 5, 101 },
};
vector<Attack> bossAttacks = { { "Attack" } };
struct ShopChoice {
    int w;
    int a;
    int r1;
    int r2;
};

int getCost(vector<Attack> items) {
    int cost = 0;
    for (int i = 0; i < items.size(); i++) {
        cost += items[i].manaCost;
    }
    return cost;
}

Character equipFighter(Character player, vector<Attack> &items) {
    for (int i = 0; i < items.size(); i++) {
        player.armor += items[i].armor;
        player.dmg += items[i].dmg;
    }
    return player;
}
void doEffect(Attack &effect, Character &boss, Character &player) {
    boss.hitPts -= effect.dmg;
    player.hitPts += effect.hitPts;
    player.mana += effect.manaRestore;
    player.armor += effect.armor;
}

void doEffects(Character &boss, Character &player, vector<Attack> &effects) {
    player.armor = 0;
    for (auto effect = effects.begin(); effect != effects.end(); effect++) {
        if (effect->duration > 0) {
            doEffect(*effect, boss, player);
            effect->duration = effect->duration - 1;
        }
    }

    for (auto effect = effects.begin(); effect != effects.end(); effect++) {
        if (effect->duration <= 0) {
            effects.erase(effect);
            effect--;
        }
    }
}
void doAttack(Attack &attack, Character &attacker, Character &receiver, vector<Attack> &effects) {
    if (DEBUG_V) {
        cout << "Char [" << attacker.id << "] attacks Char [" << receiver.id << "]\n";
    }
    attacker.mana -= attack.manaCost;
    if (attack.duration != -1) {
        effects.push_back(attack);
        return;
    }
    if (attack.dmg != 0) {
        int resultingDmg = attack.dmg - receiver.armor;
        if (resultingDmg <= 0) {
            resultingDmg = 1;
        }
        receiver.hitPts -= resultingDmg;
    }
    attacker.hitPts += attack.hitPts;

    if (DEBUG_V) {
        cout << "Char [" << receiver.id << "] hitPts reduced to [" << receiver.hitPts << "]\n";
    }
}
bool isDead(Character character) {
    return character.hitPts <= 0;
}
bool effectActive(Attack &attack, vector<Attack> &effects) {
    for (auto effect = effects.begin(); effect != effects.end(); effect++) {
        if (effect->name.compare(attack.name) == 0) {
            return true;
        }
    }
    return false;
}
pair<int, vector<Attack>>  doFightRec(vector<Attack> chosen, Character boss, Character player, vector<Attack> effects, int manaUsed, int maxMana, bool bossTurn) {
    Character buffedPlayer = player;
    if (isDead(buffedPlayer)) {
        return { -1, { chosen } };
    }
    doEffects(boss, buffedPlayer, effects);
    if (isDead(boss)) {
        if (manaUsed < maxMana) {
            if (DEBUG_I) {
                cout << "VALID BOSS DEAD! manaUsed [" << manaUsed << "] maxMana [" << maxMana << "]\n";
            }
            return { manaUsed, chosen };
        }
        else {
            if (DEBUG_V) {
                cout << "INVALID BOSS DEAD! manaUsed [" << manaUsed << "] maxMana [" << maxMana << "]\n";
            }
            return { -1, chosen };
        }
    }


    pair<int, vector<Attack>> winningSolution = { -1, chosen };
    vector<Attack> attacks = bossTurn ? bossAttacks : playerAttacks;
    for (auto attack = attacks.begin(); attack != attacks.end(); attack++) {
        Character playerWithAttack = buffedPlayer;
        Character bossWithAttack = boss;
        Character &attacker = bossTurn ? bossWithAttack : playerWithAttack;
        Character &receiver = bossTurn ? playerWithAttack : bossWithAttack;
        int totalManaUsed = attack->manaCost + manaUsed;
        if (totalManaUsed > maxMana ||
            (winningSolution.first != -1 && totalManaUsed >= winningSolution.first) ||
            attack->manaCost > attacker.mana
            ) {
            continue;
        }
        if (effectActive(*attack, effects)) {
            continue;
        }
        vector<Attack> newChosen = chosen;
        newChosen.push_back(*attack);
        vector<Attack> newEffects = effects;

        doAttack(*attack, attacker, receiver, newEffects);
        int currMax = winningSolution.first == -1 ? maxMana : winningSolution.first;

        auto res = doFightRec(newChosen, bossWithAttack, playerWithAttack, newEffects, totalManaUsed, currMax, !bossTurn);
        int lowestWinningManaRec = res.first;
        if (winningSolution.first == -1 || lowestWinningManaRec != -1 && lowestWinningManaRec < winningSolution.first) {
            winningSolution = res;
            if (DEBUG_V) {
                cout << "winningSolution! mana [" << res.first << "]\n";
            }
        }
    }
    if (DEBUG_V) {
        cout << "returned winningSolution! mana [" << winningSolution.first << "]\n";
    }
    return winningSolution;
}

void doPart1(Character & boss, Character &player) {
    int lowestMana = -1;
    for (int currMaxMana = 1000; lowestMana == -1; currMaxMana++) {
        if (DEBUG_I) {
            cout << "Trying with max Mana [" << currMaxMana << "]\n";
        }
        auto res = doFightRec({}, boss, player, {}, 0, currMaxMana, false);

        if (DEBUG_I) {
            cout << "Result with max Mana [" << currMaxMana << "] mana [" << res.first << "]\n";
        }
        lowestMana = res.first;
        int totalCost = 0;
        if (DEBUG_I) {
            for (auto attack = res.second.begin();lowestMana != -1 && attack != res.second.end(); attack++) {
                totalCost += attack->manaCost;
                cout << "attack [" << attack->name << "] cost [" << attack->manaCost << "] total [" << totalCost << "]\n";
            }
        }
    }
    cout << "Part 1 solution is " << lowestMana << "\n";
}

int main()
{
    cout << "--- Day 21: RPG Simulator 20XX ---\n";
    Character boss{ "!boss!", 51, 9, 0, 0 };
    bossAttacks[0].dmg = boss.dmg;
    Character player{ "player", 50, 0, 0, 500 };
    doPart1(boss, player);

}
