# Grammar Analyzer: FIRST and FOLLOW Sets

This C++ program calculates the FIRST and FOLLOW sets for a given context-free grammar. It takes grammar productions as input and provides the FIRST and FOLLOW sets for each non-terminal symbol in the grammar.

## Features

- **Calculate FIRST Sets:** Determines the set of terminal symbols that can appear at the start of any string derived from a non-terminal.
- **Calculate FOLLOW Sets:** Determines the set of terminal symbols that can appear immediately to the right of any non-terminal in some sentential form.

## example
Enter the number of productions: 4
Enter the productions (Format: A->BC|d):
S->ACB|CbB|Ba
A->da|BC
B->g|e
C->h|e
/ /output/ /
FIRST sets:
FIRST(S) = { d g h e a }
FIRST(A) = { d g h e }
FIRST(B) = { g h }
FIRST(C) = { h e }

FOLLOW sets:
FOLLOW(S) = { $ }
FOLLOW(A) = { h g $ }
FOLLOW(B) = { a h g $ }
FOLLOW(C) = { g b h $ }

