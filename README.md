# Five in a Row Gaming AI
This is a Five in a Row game with a very powerful AI built with search based techniques. Five in a Row is a turn based game like go. Two place moves in turn and whoever manages to place 5 stones of the same color in a row wins the game. The AI player implemented here is able to search ahead 4 rounds in a tolerable amount of time and can therefore easily beat myself as well as most other similar program I found on the internet.

### Technical Details
- Minimax search and alpha-beta pruning is used to explore all the future possible moves for several turns ahead.
- History heuristic is used to reorder the search for alpha-beta pruning to improve pruning efficiency.
- 3 difficulties are provided, namely Easy, Normal and Hard, which corresponds to searching ahead for 2, 3 and 4 turns.
- Simple function like Undo is also implemented.
