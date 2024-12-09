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

NFA dummy({0}, {'0'}, 0, {0});

NFA nfa1 = dummy;
NFA nfa2 = dummy;

void concatenation_setup() {
    std::set<int> states1 = {1, 2, 3};
    std::set<char> alphabet1 = {'a', 'b'};
    int startState1 = 1;
    std::set<int> acceptStates1 = {2, 3};

    nfa1 = NFA(states1, alphabet1, startState1, acceptStates1);
    nfa1.addTransition(1, 'a', {2});
    nfa1.addTransition(1, 'b', {3});


    std::set<int> states2 = {4, 5};
    std::set<char> alphabet2 = {'c', 'd'};
    int startState2 = 4;
    std::set<int> acceptStates2 = {5};

    nfa2 = NFA(states2, alphabet2, startState2, acceptStates2);
    nfa2.addTransition(4, 'c', {4});
    nfa2.addTransition(4, 'd', {5});
}

void concatenation_test() {
    concatenation_setup();

    NFA c_nfa = NFA::concatenation(nfa1, nfa2);
    writeDotToFile(c_nfa, "main.gv");

    std::string input = "acccd";
    if (c_nfa.run(input)) {
        std::cout << "Input accepted." << std::endl;
    } else {
        std::cout << "Input rejected." << std::endl;
    }
}

void alternative_setup() {
    std::set<int> states1 = {1, 2, 3};
    std::set<char> alphabet1 = {'a', 'b', 'c'};
    int startState1 = 1;
    std::set<int> acceptStates1 = {1, 3};

    nfa1 = NFA(states1, alphabet1, startState1, acceptStates1);
    nfa1.addTransition(1, 'a', {1});
    nfa1.addTransition(1, 'b', {2});
    nfa1.addTransition(2, 'c', {3});


    std::set<int> states2 = {4, 5};
    std::set<char> alphabet2 = {'0', '1'};
    int startState2 = 4;
    std::set<int> acceptStates2 = {5};

    nfa2 = NFA(states2, alphabet2, startState2, acceptStates2);
    nfa2.addTransition(4, '1', {5});
    nfa2.addTransition(5, '0', {5});
}

void alternative_test() {
    alternative_setup();

    NFA c_nfa = NFA::alternative(nfa1, nfa2);
    writeDotToFile(c_nfa, "main.gv");

    std::string input = "acccd";
    if (c_nfa.run(input)) {
        std::cout << "Input accepted." << std::endl;
    } else {
        std::cout << "Input rejected." << std::endl;
    }
}

void iteration_setup() {
    std::set<int> states2 = {4, 5};
    std::set<char> alphabet2 = {'c', 'd'};
    int startState2 = 4;
    std::set<int> acceptStates2 = {5};

    nfa2 = NFA(states2, alphabet2, startState2, acceptStates2);
    nfa2.addTransition(4, 'c', {4});
    nfa2.addTransition(4, 'd', {5});
}

void iteration_test() {
    iteration_setup();

    NFA c_nfa = NFA::iteration_plus(nfa2);
    writeDotToFile(c_nfa, "main.gv");

    std::string input = "acccd";
    if (c_nfa.run(input)) {
        std::cout << "Input accepted." << std::endl;
    } else {
        std::cout << "Input rejected." << std::endl;
    }
}

int main() {
    // concatenation_test();
    // alternative_test();
    iteration_test();

    return 0;
}