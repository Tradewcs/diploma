// #include <graphviz/gvc.h>
#include "NFA.h"
#include <fstream>
#include <iostream>
#include <cstring>

void generateGraphImage(const std::string& dotCode, const std::string& outputFile) {
    std::string tempDotFile = "temp_graph.dot";
    std::ofstream dotFile(tempDotFile);
    if (!dotFile) {
        std::cerr << "Error: Unable to create temporary file." << std::endl;
        return;
    }
    dotFile << dotCode;
    dotFile.close();

    std::string command = "dot -Tpng " + tempDotFile + " -o " + outputFile;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Graph image generated: " << outputFile << std::endl;
    } else {
        std::cerr << "Error: Failed to generate graph image." << std::endl;
    }

    std::remove(tempDotFile.c_str());
}

int main1() {
    std::set<int> states1 = {0, 1};
    std::set<char> alphabet1 = {'a', 'b'};
    int startState1 = 0;
    std::set<int> acceptStates1 = {1};

    NFA nfa1(states1, alphabet1, startState1, acceptStates1);
    nfa1.addTransition(0, 'a', {0});
    nfa1.addTransition(0, 'b', {1});

    std::set<int> states2 = {2, 3};
    std::set<char> alphabet2 = {'c'};
    int startState2 = 2;
    std::set<int> acceptStates2 = {3};

    NFA nfa2(states2, alphabet2, startState2, acceptStates2);
    nfa2.addTransition(2, 'c', {3});


    // NFA nfa_res = NFA::iteration(nfa1);
    NFA nfa_res = NFA::alternative(nfa1, nfa2);
    nfa_res.removeUnreachable();
    
    generateGraphImage(nfa_res.convertToDot(), "out.png");

    return 0;
}

int main() {
    std::set<int> states = {1, 2, 3, 4, 5};
    std::set<char> alphabet = {'a', 'b', 'c', 'd'};
    int startState = 1;
    std::set<int> acceptStates = {3, 4};

    NFA nfa(states, alphabet, startState, acceptStates);
    nfa.addTransition(1, 'a', {2});
    nfa.addTransition(1, 'b', {3});
    nfa.addTransition(4, 'c', {3});
    nfa.addTransition(5, 'd', {3, 4});

    nfa.removeUnreachable();
    generateGraphImage(nfa.convertToDot(), "out.png");


    return 0;
}