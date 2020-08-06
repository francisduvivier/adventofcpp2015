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
    int capacity = 0;   // (how well it helps the cookie absorb milk)
    int durability = 0; // (how well it keeps the cookie intact when full of milk)
    int flavor = 0;     // (how tasty it makes the cookie)
    int texture = 0;    // (how it improves the feel of the cookie)
    int calories = 0;   // (how many calories it adds to the cookie)
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
pair<vector<pair<string, int>>, IngredientInfo> getBestTasteRec(IngredientInfoMap &ingredientInfoMap, IngredientInfo &ingredientStatus, vector<string> &ingredientsLeft, int remainingSpoons, int calories)
{
    if (ingredientsLeft.size() == 0)
    {
        throw "Illegal arguments";
    }
    if (ingredientsLeft.size() == 1)
    {
        auto newIngredientStatus = mergeIngredients(ingredientStatus, ingredientInfoMap[ingredientsLeft[0]], remainingSpoons);
        return pair<vector<pair<string, int>>, IngredientInfo>(vector<pair<string, int>>({ pair<string, int>(ingredientsLeft[0], remainingSpoons) }), newIngredientStatus);
    }
    else
    {
        pair<vector<pair<string, int>>, IngredientInfo> bestMerge;
        int bestTaste = -1;
        vector<string> newIngredientsLeft = ingredientsLeft;
        string poppedIngredient = ingredientsLeft.back();
        IngredientInfo currInfo = ingredientInfoMap[poppedIngredient];
        newIngredientsLeft.pop_back();
        int poppedSpoons = -1;
        for (int currSpoons = 0; currSpoons <= remainingSpoons; currSpoons++)
        {
            int newRemainingSpoons = remainingSpoons - currSpoons;
            auto newIngredientStatus = mergeIngredients(ingredientStatus, currInfo, currSpoons);
            auto currBest = getBestTasteRec(ingredientInfoMap, newIngredientStatus, newIngredientsLeft, newRemainingSpoons, calories);
            int currBestTaste = calcTaste(currBest.second);
            if (currBestTaste > bestTaste && (calories == -1 || currBest.second.calories == calories))
            {
                bestTaste = currBestTaste;
                bestMerge = currBest;
                poppedSpoons = currSpoons;
            }
        }
        bestMerge.first.push_back(pair<string, int>(poppedIngredient, poppedSpoons));
        return bestMerge;
    }
}
void fillIngredientInfoMap(vector<string> &lines, IngredientInfoMap &infoMap)
{
    for (int i = 0; i < lines.size(); i++)
    {
        cmatch matchGroups;
        regex re("(.+): capacity (-?[0-9]+), durability (-?[0-9]+), flavor (-?[0-9]+), texture (-?[0-9]+), calories (-?[0-9]+)");
        regex_match(lines[i].c_str(), matchGroups, re);
        IngredientInfo properties;
        int cg = 1;
        string name = matchGroups[cg++];
        properties.capacity = stoi(matchGroups[cg++]);
        properties.durability = stoi(matchGroups[cg++]);
        properties.flavor = stoi(matchGroups[cg++]);
        properties.texture = stoi(matchGroups[cg++]);
        properties.calories = stoi(matchGroups[cg++]);
        if (DEBUG_I)
        {
            cout << "Adding name[" << name << "], capacity[" << properties.capacity << "], durability[" << properties.durability << "], flavor[" << properties.flavor << "], texture[" << properties.texture << "], calories[" << properties.calories << "]\n";
        }
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
    auto bestIngredients = getBestTasteRec(ingredientInfoMap, newIngredientStatus, newIngredientsLeft, maxSpoons, -1);
    int part1BestTaste = calcTaste(bestIngredients.second);
    if (DEBUG_I)
    {
        for (int i = 0; i < bestIngredients.first.size(); i++)
        {
            auto nameSpoons = bestIngredients.first[i];
            cout << "Best Ingredient [" << nameSpoons.first << "], spoons [" << nameSpoons.second << "]\n";
        }
    }
    cout << "Part 1 solution is < " << part1BestTaste << " >\n";

    auto bestIngredientsPart2 = getBestTasteRec(ingredientInfoMap, newIngredientStatus, newIngredientsLeft, maxSpoons, 500);
    int part2BestTaste = calcTaste(bestIngredientsPart2.second);
    if (DEBUG_I)
    {
        for (int i = 0; i < bestIngredientsPart2.first.size(); i++)
        {
            auto nameSpoons = bestIngredientsPart2.first[i];
            cout << "Best Ingredient [" << nameSpoons.first << "], spoons [" << nameSpoons.second << "]\n";
        }
    }

    cout << "Part 2 solution is < " << part2BestTaste << " >\n";
}
