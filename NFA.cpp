#include "NFA.h"
#include <iostream>
#include <algorithm>

void NFA::addTransition(int state, char symbol, const std::set<int>& nextStates) {
    if (states.find(state) != states.end() && (alphabet.find(symbol) != alphabet.end() || symbol == '\0')) {
        transitionTable[{state, symbol}].insert(nextStates.begin(), nextStates.end());
    } else {
        std::cerr << "Invalid transition: check states or symbols" << std::endl;
    }
}

void NFA::newState() {
    states.insert(*std::prev(states.end()) + 1);
}

bool NFA::runHelper(int currentState, const std::string &input, int index) {
    if (index == input.size()) {
        if (acceptStates.find(currentState) != acceptStates.end()) {
            return true;
        }
        auto epsilonTransition = transitionTable.find({currentState, '\0'});
        if (epsilonTransition != transitionTable.end()) {
            for (int nextState : epsilonTransition->second) {
                if (runHelper(nextState, input, index)) return true;
            }
        }
        return false;
    }

    char currentSymbol = input[index];
    auto transition = transitionTable.find({currentState, currentSymbol});
    if (transition != transitionTable.end()) {
        for (int nextState : transition->second) {
            if (runHelper(nextState, input, index + 1)) return true;
        }
    }

    auto epsilonTransition = transitionTable.find({currentState, '\0'});
    if (epsilonTransition != transitionTable.end()) {
        for (int nextState : epsilonTransition->second) {
            if (runHelper(nextState, input, index)) return true;
        }
    }

    return false;
}

bool NFA::run(const std::string &input) {
    return runHelper(startState, input, 0);
}

std::string NFA::convertToDot() {
    std::string dot;

    dot += "digraph NFA {\n";
    dot += "\trankdir=LR;\n";
    dot += "\tnode [shape=circle];\n";

    for (const auto &[key, value] : transitionTable) {
        int fromState = key.first;
        char transitionChar = key.second;

        for (int toState : value) {
            dot += "\tS" + std::to_string(fromState) + " -> S" + std::to_string(toState);
            dot += " [label=\"" + std::string(1, transitionChar) + "\"];\n";
        }
    }

    dot += "}\n";
    return dot;
}

NFA NFA::concatenate(const NFA& nfa1, const NFA& nfa2) {
    std::set<int> newStates;
    std::set<char> newAlphabet = nfa1.alphabet;
    newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

    std::map<std::pair<int, char>, std::set<int>> newTransitions;
    std::set<int> newAcceptStates;

    int max1 = *std::max_element(nfa1.states.begin(), nfa1.states.end());
    int min2 = *std::min_element(nfa2.states.begin(), nfa2.states.end());
    int offset = max1 - min2 + 1;
    if (offset < 0) {
        offset = 0;
    }

    for (int state : nfa1.states) newStates.insert(state);
    for (int state : nfa2.states) newStates.insert(state + offset);
    newStates.erase(nfa2.startState + offset);

    for (const auto& [key, value] : nfa1.transitionTable) {
        newTransitions[key] = value;
    }

    for (const auto& [key, value] : nfa2.transitionTable) {
        if (key.first == nfa2.startState) continue;

        std::set<int> newValue;
        for (const auto &state : value) {
            if (state == nfa2.startState) {
                newValue.insert(nfa1.acceptStates.begin(), nfa1.acceptStates.end());
            } else {
                newValue.insert(state + offset);
            }
        }

        auto newKey = std::make_pair(key.first + offset, key.second);
        newTransitions[newKey] = newValue;
    }

    for (int acceptState : nfa1.acceptStates) {
        for (const char &symbol : nfa2.alphabet) {
            auto it = nfa2.transitionTable.find({nfa2.startState, symbol});
            if (it != nfa2.transitionTable.end()) {
                auto oldStates = it->second;
                std::set<int> newStates;
                for (int s : oldStates) {
                    if (s == nfa2.startState) {
                        newStates.insert(nfa1.acceptStates.begin(), nfa1.acceptStates.end());
                    } else {
                        newStates.insert(s + offset);
                    }
                }
            
                newTransitions[{acceptState, symbol}].insert(newStates.begin(), newStates.end());
            }
        }
    }

    for (int state : nfa2.acceptStates) {
        newAcceptStates.insert(state + offset);
    }

    NFA newNfa = NFA(newStates, newAlphabet, nfa1.startState, newAcceptStates);
    newNfa.transitionTable = newTransitions;

    return newNfa;
}

NFA NFA::alternative(const NFA &nfa1, const NFA &nfa2) {
    std::set<int> newStates;
    std::set<char> newAlphabet = nfa1.alphabet;
    newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

    std::map<std::pair<int, char>, std::set<int>> newTransitions;
    std::set<int> newAcceptStates;

    int max1 = *std::max_element(nfa1.states.begin(), nfa1.states.end());
    int min2 = *std::min_element(nfa2.states.begin(), nfa2.states.end());
    int offset = max1 - min2 + 1;
    if (offset < 0) {
        offset = 0;
    }

    int newStartState = std::max(max1, *std::max_element(nfa2.states.begin(), nfa2.states.end())) + 1;

    for (int state : nfa1.states) newStates.insert(state);
    for (int state : nfa2.states) newStates.insert(state + offset);
    newStates.erase(nfa1.startState);
    newStates.erase(nfa2.startState + offset);

    for (const auto& [key, value] : nfa1.transitionTable) {
        if (key.first == nfa1.startState) continue;

        newTransitions[key] = value;
    }
    for (const auto& [key, value] : nfa2.transitionTable) {
        if (key.first == nfa2.startState) continue;

        std::set<int> newValue;
        for (const auto &state : value) {
            newValue.insert(state + offset);
        }

        auto newKey = std::make_pair(key.first + offset, key.second);
        newTransitions[newKey] = newValue;
    }

    for (const char &symbol : nfa1.alphabet) {
        auto it = nfa1.transitionTable.find({nfa1.startState, symbol});
        if (it != nfa1.transitionTable.end()) {
            auto oldStates = it->second;
            std::set<int> newStates;
            for (int s : oldStates) {
                if (s == nfa1.startState) {
                    newStates.insert(newStartState);
                } else {
                    newStates.insert(s);
                }
            }
        
            newTransitions[{newStartState, symbol}].insert(newStates.begin(), newStates.end());
        }
    }
    
    for (const char &symbol : nfa2.alphabet) {
        auto it = nfa2.transitionTable.find({nfa2.startState, symbol});
        if (it != nfa2.transitionTable.end()) {
            auto oldStates = it->second;
            std::set<int> newStates;
            for (int s : oldStates) {
                if (s == nfa2.startState) {
                    newStates.insert(newStartState);
                } else {
                    newStates.insert(s + offset);
                }
            }
        
            newTransitions[{newStartState, symbol}].insert(newStates.begin(), newStates.end());
        }
    }

    for (int state : nfa1.acceptStates) newAcceptStates.insert(state);
    for (int state : nfa2.acceptStates) newAcceptStates.insert(state + offset);

    NFA newNfa = NFA(newStates, newAlphabet, newStartState, newAcceptStates);
    newNfa.transitionTable = newTransitions;

    return newNfa;
}

