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

int main() {
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
    writeDotToFile(c_nfa, "c.gv");

    for (const auto& [key, value] : c_nfa.transitionTable) {
        cout << key.first << " " << key.second << " - ";
        for (auto s : value) {
            cout << s << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << endl;
    for (auto st : c_nfa.states) {
        cout << st << " ";
    }
    cout << endl;

    std::string input = "10111";
    if (c_nfa.run(input)) {
        std::cout << "Input accepted." << std::endl;
    } else {
        std::cout << "Input rejected." << std::endl;
    }

    return 0;
}
