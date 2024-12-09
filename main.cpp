#include "NFA.h"
#include <iostream>
#include <utility>
#include <fstream>


using namespace std;

void writeDotToFile(NFA nfa, const std::string filename) {
    ofstream outFile(filename);

    if (outFile.is_open()) {
        outFile << nfa.convertToDot();
        outFile.close();
    } else {
        cerr << "Failed to open file: " << filename << endl;
    }
}

int main1() {
    std::set<int> states1 = {1, 2};
    std::set<char> alphabet1 = {'0', '1'};
    int startState1 = 1;
    std::set<int> acceptStates1 = {2};

    NFA nfa1(states1, alphabet1, startState1, acceptStates1);
    nfa1.addTransition(1, '1', {2});
    // nfa1.addTransition(2, '0', {2});



    std::set<int> states2 = {3, 4};
    std::set<char> alphabet2 = {'0', '1'};
    int startState2 = 3;
    std::set<int> acceptStates2 = {4};

    NFA nfa2(states2, alphabet2, startState2, acceptStates2);
    nfa2.addTransition(3, '1', {3});
    nfa2.addTransition(3, '0', {4});
    nfa2.addTransition(4, '1', {4});



    NFA c_nfa = NFA::alternative(nfa1, nfa2);
    writeDotToFile(c_nfa, "main.gv");

    // for (const auto& [key, value] : c_nfa.transitionTable) {
    //     cout << key.first << " " << key.second << " - ";
    //     for (auto s : value) {
    //         cout << s << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    // cout << endl;
    // for (auto st : c_nfa.states) {
    //     cout << st << " ";
    // }
    // cout << endl;

    std::string input = "10111";
    if (c_nfa.run(input)) {
        std::cout << "Input accepted." << std::endl;
    } else {
        std::cout << "Input rejected." << std::endl;
    }

    return 0;
}


int main2() {
    std::set<int> states = {13, 7, 8, 10, 11};
    std::set<char> alphabet = {'0', '1'};
    int startState = 13;
    std::set<int> acceptStates = {11};

    NFA nfa1(states, alphabet, startState, acceptStates);

    nfa1.addTransition(13, '0', {7, 10});
    nfa1.addTransition(7, '1', {8});
    nfa1.addTransition(8, '0', {7, 10});
    nfa1.addTransition(10, '0', {11});

    cout << 123 << endl;
    writeDotToFile(nfa1, "main.gv");

    return 0;
}


int main_example() {
    std::set<int> states = {1, 2, 3};
    std::set<char> alphabet = {'a', 'b', 'c'};
    int startState = 1;
    std::set<int> acceptStates = {2};

    NFA nfa(states, alphabet, startState, acceptStates);

    nfa.addTransition(1, 'a', {2});
    nfa.addTransition(2, 'b', {3});
    nfa.addTransition(3, 'c', {2});

    NFA nfa_i = NFA::iteration(nfa);

    writeDotToFile(nfa_i, "main.gv");

    return 0;
}



int main() {
    std::set<int> states = {1, 2, 3};
    std::set<char> alphabet = {'a', 'b', 'c'};
    int startState = 1;
    std::set<int> acceptStates = {2};

    NFA nfa(states, alphabet, startState, acceptStates);

    nfa.addTransition(1, 'a', {2});
    nfa.addTransition(2, 'b', {3});
    nfa.addTransition(3, 'c', {2});

    NFA nfa_i = NFA::iteration(nfa);

    writeDotToFile(nfa_i, "main.gv");

    return 0;
}


