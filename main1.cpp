#include "NFA.h"
#include <iostream>

using namespace std;

int main() {
    // std::set<int> states1 = {0, 1};
    // std::set<char> alphabet1 = {'a'};
    // int startState1 = 0;
    // std::set<int> acceptStates1 = {1};

    // NFA nfa1(states1, alphabet1, startState1, acceptStates1);
    // nfa1.addTransition(0, 'a', {1});



    // std::set<int> states2 = {0, 1};
    // std::set<char> alphabet2 = {'b'};
    // int startState2 = 0;
    // std::set<int> acceptStates2 = {1};

    // NFA nfa2(states2, alphabet2, startState2, acceptStates2);
    // nfa2.addTransition(0, 'b', {1});

    std::set<int> states1 = {0, 1, 2};
    std::set<char> alphabet1 = {'a', 'b'};
    int startState1 = 0;
    std::set<int> acceptStates1 = {1, 2};

    NFA nfa1(states1, alphabet1, startState1, acceptStates1);
    nfa1.addTransition(0, 'a', {1});
    nfa1.addTransition(0, 'b', {1});
    nfa1.addTransition(0, 'b', {2});



    std::set<int> states2 = {0, 1, 2};
    std::set<char> alphabet2 = {'n', 'a', 'b'};
    int startState2 = 0;
    std::set<int> acceptStates2 = {1, 2};

    NFA nfa2(states2, alphabet2, startState2, acceptStates2);
    nfa2.addTransition(0, 'a', {1});
    nfa2.addTransition(0, 'n', {1});
    nfa2.addTransition(0, 'b', {2});



    NFA c_nfa = NFA::concatenate(nfa1, nfa2);

    cout << c_nfa.transitionTable.size() << endl;
    cout << endl;

    for (const auto& [key, value] : c_nfa.transitionTable) {
        cout << key.first << " " << key.second << " - ";
        for (auto s : value) {
            cout << s << " ";
        }
        cout << endl;
    }
    cout << endl;

    for (auto st : c_nfa.states) {
        cout << st << " ";
    }
    cout << endl;

    std::string input = "bb";
    if (c_nfa.run(input)) {
        std::cout << "Input accepted." << std::endl;
    } else {
        std::cout << "Input rejected." << std::endl;
    }

    return 0;
}
