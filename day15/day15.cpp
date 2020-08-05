#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "../util/processing.h"
using namespace std;
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
struct IngredientInfo
{
    int capacity;   // (how well it helps the cookie absorb milk)
    int durability; // (how well it keeps the cookie intact when full of milk)
    int flavor;     // (how tasty it makes the cookie)
    int texture;    // (how it improves the feel of the cookie)
    int calories;   // (how many calories it adds to the cookie)
};
using IngredientInfoMap = map<string, IngredientInfo>;
int calcTaste(IngredientInfo mergedIngredients)
{
    return max(0, mergedIngredients.capacity) * max(0, mergedIngredients.durability) * max(0, mergedIngredients.flavor) * max(0, mergedIngredients.texture);
}

IngredientInfo mergeIngredients(IngredientInfo &start, IngredientInfo &newIngredient, int newIngredientAmount)
{
    IngredientInfo result = start;
    result.capacity += newIngredient.capacity * newIngredientAmount;
    result.durability += newIngredient.durability * newIngredientAmount;
    result.flavor += newIngredient.flavor * newIngredientAmount;
    result.texture += newIngredient.texture * newIngredientAmount;
    result.calories += newIngredient.calories * newIngredientAmount;
    return result;
}
IngredientInfo getBestTasteRec(IngredientInfoMap &ingredientInfoMap, IngredientInfo &ingredientStatus, vector<string> &ingredientsLeft, int remainingSpoons)
{
    if (ingredientsLeft.size() == 1)
    {
        auto newIngredientStatus = mergeIngredients(ingredientStatus, ingredientInfoMap[ingredientsLeft[0]], remainingSpoons);
        return newIngredientStatus;
    }
    else
    {
        pair<string, int> lastIngredient(ingredientsLeft[0], remainingSpoons);
        IngredientInfo bestMerge;
        int bestTaste = -1;
        vector<string> newIngredientsLeft = ingredientsLeft;
        string currIngredient = ingredientsLeft.back();
        IngredientInfo currInfo = ingredientInfoMap[currIngredient];
        newIngredientsLeft.pop_back();
        for (int currSpoons = 0; currSpoons <= remainingSpoons; currSpoons++)
        {
            int newRemainingSpoons = remainingSpoons - currSpoons;
            auto newIngredientStatus = mergeIngredients(ingredientStatus, currInfo, currSpoons);
            auto currBest = getBestTasteRec(ingredientInfoMap, newIngredientStatus, newIngredientsLeft, newRemainingSpoons);
            int currBestTaste = calcTaste(currBest);
            if (currBestTaste > bestTaste)
            {
                bestTaste = currBestTaste;
                bestMerge = currBest;
            }
        }
        return bestMerge;
    }
}
void fillIngredientInfoMap(vector<string> &lines, IngredientInfoMap infoMap)
{
    for (int i = 0; i < lines.size(); i++)
    {
        cmatch matchGroups;
        regex re("(.+): capacity (-?[0-9]+), durability (-?[0-9]+), flavor (-?[0-9]+), texture (-?[0-9]+), calories (-?[0-9]+)");
        regex_match(lines[i].c_str(), matchGroups, re);
        int cg = 1;
        IngredientInfo properties;
        string name = matchGroups[cg++];
        properties.capacity = stoi(matchGroups[cg++]);
        properties.durability = stoi(matchGroups[cg++]);
        properties.flavor = stoi(matchGroups[cg++]);
        properties.texture = stoi(matchGroups[cg++]);
        properties.calories = stoi(matchGroups[cg++]);
        pair<string, IngredientInfo> infoPair(name, properties);
        infoMap.insert(infoPair);
    }
}
int main()
{
    cout << "Day 15\n";
    string input = getInput(15);
    vector<string> lines;
    splitString(input, "\n", lines);
    IngredientInfoMap ingredientInfoMap;
    fillIngredientInfoMap(lines, ingredientInfoMap);
    IngredientInfo newIngredientStatus;
    int maxSpoons = 100;
    vector<string> newIngredientsLeft;
    for (auto iter = ingredientInfoMap.begin(); iter != ingredientInfoMap.end(); ++iter)
    {
        newIngredientsLeft.push_back(iter->first);
    }
    auto bestIngredients = getBestTasteRec(ingredientInfoMap, newIngredientStatus, newIngredientsLeft, maxSpoons);

    int currBestTaste = calcTaste(bestIngredients);
    cout << "Part 1 solution is < " << currBestTaste << " >\n";
}