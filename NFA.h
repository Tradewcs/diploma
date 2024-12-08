#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

class NFA {
public:
    NFA(const std::set<int>& states, const std::set<char>& alphabet, int startState, const std::set<int>& acceptStates) 
        : states(states), alphabet(alphabet), startState(startState), acceptStates(acceptStates) {}

    void addTransition(int state, char symbol, const std::set<int> &nextStates);
    void newState();
    bool run(const std::string &input);
    
    
    static NFA concatenate(const NFA &nfa1, const NFA &nfa2);
    static NFA alternative(const NFA &nfa1, const NFA &nfa2);
    static NFA iteration(const NFA &nfa);

private:
    bool runHelper(int currentState, const std::string &input, int index);

private:
public:
    std::set<int> states;
    std::set<char> alphabet;
    std::map<std::pair<int, char>, std::set<int>> transitionTable;
    int startState;
    std::set<int> acceptStates;

    
public:
    std::string convertToDot();
};