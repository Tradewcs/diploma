#pragma once

#include <map>
#include <set>
#include <string>

class NFA {
private:
    std::set<int> states;
    std::set<char> alphabet;
    std::map<std::pair<int, char>, std::set<int>> transitionTable;
    int startState;
    std::set<int> acceptStates;

public:
    NFA(const std::set<int>& states, const std::set<char>& alphabet, int startState, const std::set<int>& acceptStates) 
        : states(states), alphabet(alphabet), startState(startState), acceptStates(acceptStates) {}


    void addTransition(int state, char symbol, const std::set<int> &nextStates);
    void newState();
    void addSymbol(char c);

    static NFA concatenation(const NFA &nfa1, const NFA &nfa2);
    static NFA alternative(const NFA &nfa1, const NFA &nfa2);

    static NFA iteration(const NFA &nfa);
    static NFA iterationPlus(const NFA &nfa);

    void removeUnreachable();
    std::string convertToDot();

    bool run(const std::string &input);

private:
    int calculateOffset(const NFA& nfa) const;
    bool runHelper(int currentState, const std::string &input, int index);
};