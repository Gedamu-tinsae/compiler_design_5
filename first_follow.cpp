#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

// Function to split a string by a delimiter
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    size_t start = 0, end;
    while ((end = s.find(delimiter, start)) != string::npos) {
        token = s.substr(start, end - start);
        tokens.push_back(token);
        start = end + 1;
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

void findFollow(char symbol, map<char, vector<string>> &productions, map<char, set<char>> &first, map<char, set<char>> &follow, char startSymbol) {
    if (follow[symbol].empty()) {
        if (symbol == startSymbol) {
            follow[symbol].insert('$');
        }

        for (auto &prod : productions) {
            for (auto &production : prod.second) {
                size_t pos = production.find(symbol);
                while (pos != string::npos) {
                    if (pos + 1 < production.size()) {
                        char nextSym = production[pos + 1];
                        if (isupper(nextSym)) {
                            set<char> firstSet = first[nextSym];
                            follow[symbol].insert(firstSet.begin(), firstSet.end());
                            follow[symbol].erase('e'); // Epsilon representation
                            if (firstSet.find('e') != firstSet.end() && symbol != prod.first) {
                                findFollow(prod.first, productions, first, follow, startSymbol);
                                follow[symbol].insert(follow[prod.first].begin(), follow[prod.first].end());
                            }
                        } else {
                            follow[symbol].insert(nextSym);
                        }
                    } else if (symbol != prod.first) {
                        findFollow(prod.first, productions, first, follow, startSymbol);
                        follow[symbol].insert(follow[prod.first].begin(), follow[prod.first].end());
                    }
                    pos = production.find(symbol, pos + 1);
                }
            }
        }
    }
}

set<char> findFirst(char symbol, map<char, vector<string>> &productions, map<char, set<char>> &first) {
    if (!first[symbol].empty()) {
        return first[symbol];
    }

    set<char> result;
    for (auto &production : productions[symbol]) {
        for (size_t i = 0; i < production.size(); ++i) {
            char sym = production[i];
            if (!isupper(sym)) {  // Terminal
                result.insert(sym);
                break;
            } else {  // Non-terminal
                set<char> firstSet = findFirst(sym, productions, first);
                result.insert(firstSet.begin(), firstSet.end());
                if (firstSet.find('e') == firstSet.end()) {  // Epsilon representation
                    break;
                }
            }
        }
    }
    first[symbol] = result;
    return result;
}
 

int main() {
    map<char, vector<string>> productions;
    char startSymbol;

    int numProductions;
    cout << "Enter the number of productions: ";
    cin >> numProductions;
    cin.ignore();

    cout << "Enter the productions (Format: A->BC|d) !! 'e' is used as the representation for epsilon !! :" << endl;
    for (int i = 0; i < numProductions; i++) {
        string input;
        getline(cin, input);
        char nonTerminal = input[0];
        if (i == 0) {
            startSymbol = nonTerminal;
        }
        string rules = input.substr(3);
        vector<string> productionsList = split(rules, '|');
        productions[nonTerminal] = productionsList;
    }

    map<char, set<char>> first;
    map<char, set<char>> follow;

    // Calculate FIRST for all non-terminals
    for (auto &prod : productions) {
        findFirst(prod.first, productions, first);
    }

    // Calculate FOLLOW for all non-terminals
    for (auto &prod : productions) {
        findFollow(prod.first, productions, first, follow, startSymbol);
    }

    // Display FIRST sets
    cout << "\nFIRST sets:" << endl;
    for (auto &f : first) {
        cout << "FIRST(" << f.first << ") = { ";
        for (auto &sym : f.second) {
            cout << sym << " ";
        }
        cout << "}" << endl;
    }

    // Display FOLLOW sets
    cout << "\nFOLLOW sets:" << endl;
    for (auto &f : follow) {
        cout << "FOLLOW(" << f.first << ") = { ";
        for (auto &sym : f.second) {
            cout << sym << " ";
        }
        cout << "}" << endl;
    }

    return 0;
}


/*
Enter the number of productions: 4
Enter the productions (Format: A -> B C | d):
S->ACB|CbB|Ba
A->da|BC
B->g|e(here e is epsilon)
C->h|e(here e is epsilon)

FIRST sets:
FIRST(S) = { d,g,h,b,e(here e is epsilon),a }
FIRST(A) = {d,g,h,e(here e is epsilon) }
FIRST(B) = { g,h }
FIRST(C) = { h,e}

FOLLOW sets:
FOLLOW(S) = { $ }
FOLLOW(A) = { h,g,$ }
FOLLOW(B) = { a,h,g,$ }
FOLLOW(C) = { g,b,h,$ } 

g++ -o myProgram first_Follow.cpp
./myProgram.exe
*/