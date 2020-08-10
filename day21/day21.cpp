#include <iostream>
#include <vector>
#include <map>

const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
using namespace std;

struct Character {
    string id;
    int hitPts;
    int dmg;
    int armor;
};
struct Game {
    vector<Character> &characters;
    int turn;
};
struct Accessory {
    string name;
    int cost;
    int dmg;
    int armor;
};
struct Shop {
    vector<Accessory> weapons;
    vector<Accessory> armors;
    vector<Accessory> rings;
};
struct ShopChoice {
    int w;
    int a;
    int r1;
    int r2;
};
Accessory &toItem(vector<Accessory> &options, int choice, Accessory &fallback) {
    if (choice == -1 || choice >= options.size()) {
        return fallback;
    }
    else {
        return options[choice];
    }
}
vector<Accessory> getItems(Shop shop, ShopChoice choice) {
    vector<Accessory> result;
    Accessory fallback = { "", 0, 0, 0 };
    result.push_back(toItem(shop.weapons, choice.w, fallback));
    result.push_back(toItem(shop.armors, choice.a, fallback));
    result.push_back(toItem(shop.rings, choice.r1, fallback));
    result.push_back(toItem(shop.rings, choice.r2, fallback));
    return result;
}

int getCost(vector<Accessory> items) {
    int cost = 0;
    for (int i = 0; i < items.size(); i++) {
        cost += items[i].cost;
    }
    return cost;
}


Character equipFighter(Character player, vector<Accessory> &items) {
    for (int i = 0; i < items.size(); i++) {
        player.armor += items[i].armor;
        player.dmg += items[i].dmg;
    }
    return player;
}
void doAttack(Character &attacker, Character &receiver) {
    if (DEBUG_V) {
        cout << "Char [" << attacker.id << "] attacks Char [" << receiver.id << "]\n";
    }
    int resultingDmg = attacker.dmg - receiver.armor;
    if (resultingDmg <= 0) {
        resultingDmg = 1;
    }

    receiver.hitPts -= resultingDmg;
    if (DEBUG_V) {
        cout << "Char [" << receiver.id << "] hitpoints reduced to [" << receiver.hitPts << "]\n";
    }
}
bool getWinner(Character &player, Character &boss) {
    bool bossTurn = false;
    while (player.hitPts > 0 && boss.hitPts > 0) {

        if (bossTurn) {
            doAttack(boss, player);
        }
        else {
            doAttack(player, boss);
        }
        bossTurn = !bossTurn;
    }
    return (boss.hitPts <= 0);
}
pair<int, int> findPrices(Character &boss, Character &player, Shop &shop) {
    int minWinCost = INT32_MAX;
    int maxLoseCost = 0;
    for (int w = 0; w < shop.weapons.size(); w++) {
        for (int a = 0; a < shop.armors.size() + 1; a++) {
            for (int r1 = 0; r1 < shop.rings.size() + 1; r1++) {
                for (int r2 = r1 + 1; r2 < shop.rings.size() + 2; r2++) {
                    vector<Accessory> items = getItems(shop, { w, a, r1, r2 });
                    if (DEBUG_V) {
                        cout << "w [" << w << "] a [" << a << "] r1 [" << r1 << "] r2 [" << r2 << "]\n";
                    }
                    int newCost = getCost(items);
                    Character equippedFighter = equipFighter(player, items);
                    Character bossCopy = boss;
                    if (getWinner(equippedFighter, bossCopy)) {
                        if (minWinCost > newCost) {
                            if (DEBUG_I) {
                                cout << "CurrBest minWinCost [" << newCost << "]: ";
                                cout << "w [" << items[0].name << "] a [" << items[1].name << "] r1 [" << items[2].name << "] r2 [" << items[3].name << "]\n";
                            }
                            minWinCost = newCost;
                        }
                    } else {
                        if (maxLoseCost < newCost) {
                            if (DEBUG_I) {
                                cout << "CurrBest maxLoseCost [" << newCost << "]: ";
                                cout << "w [" << items[0].name << "] a [" << items[1].name << "] r1 [" << items[2].name << "] r2 [" << items[3].name << "]\n";
                            }
                            maxLoseCost = newCost;
                        }
                    }
                }
            }
        }

    }
    return { minWinCost, maxLoseCost };
}
void doPart1And2(Character & boss, Character & figther, Shop & shop) {
    pair<int, int> res = findPrices(boss, figther, shop);
    cout << "Part 1 solution is " << res.first << "\n";
    cout << "Part 2 solution is " << res.second << "\n";

}

int main()
{
    cout << "--- Day 21: RPG Simulator 20XX ---\n";
    vector<Accessory> weapons = {

        { "Dagger", 8, 4, 0 },
        { "Shortsword", 10, 5, 0 },
        { "Warhammer", 25, 6, 0 },
        { "Longsword", 40, 7, 0 },
        { "Greataxe", 74, 8, 0 },
    };
    vector<Accessory> armors = {
        { "Leather", 13, 0, 1 },
        { "Chainmail", 31, 0, 2 },
        { "Splintmail", 53, 0, 3 },
        { "Bandedmail", 75, 0, 4 },
        { "Platemail", 102, 0, 5 },
    };
    vector<Accessory> rings = {
        { "Damage +1", 25, 1, 0 },
        { "Damage +2", 50, 2, 0 },
        { "Damage +3", 100, 3, 0 },
        { "Defense +1", 20, 0, 1 },
        { "Defense +2", 40, 0, 2 },
        { "Defense +3", 80, 0, 3 },
    };
    Shop shop{
        weapons, armors, rings
    };
    Character boss{ "!boss!", 100, 8, 2 };
    Character player{ "player", 100, 0, 0 };

    doPart1And2(boss, player, shop);

}
