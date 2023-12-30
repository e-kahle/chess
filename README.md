# Welcome to the Official GitHub of Thomas the Chess Engine (TTCE).
TTCE is a simple chess engine written in C. You can play against TTCE online at https://lichess.org/@/ThomasTCE1 \
ThomasTCE1 is a lichess-bot account that runs TTCE to play against humans and/or other lichess-bot accounts.

# Features
TTCE implements the following features:\
-- BitMaps\
-- Piece Square evaluation\
-- Bishop Pair evaluation\
-- Passed & Isolated Pawn evaluation\
-- Material evaluation\
-- Move Generation & Degeneration\
-- Null Move Pruning\
-- Global Principle Variation HashTable\
-- Polyglot Opening Book (modifiable)\
-- PERFT Testing\
-- Multithreading for 'stop thinking' commands\
-- Symmetric MultiProcessing (Lazy SMP)\
-- Smart Move Ordering\
-- Alpha Beta search\
-- Quiescence search\
-- Niche Chess Rules (En-Passant, Fifty Move Rule, Threefold Repetition, Insufficient Material)\
-- Variable (user-specified) HashTable size for RAM limitations\
-- Variable (user-specified) Thread Number for Lazy SMP (default to true)\

# Statistics
Speed:\
-- TTCE leverages the features listed above to search through positions at a default speed of ~9 million moves/second \
-- The speed of TTCE can be improved further by increasing the Thread Number for Lazy SMP up to 32 \
--- Increasing the Thread Number does not guarantee improved performance; It depends on your computing power \
Strength:\
-- Bullet: 2206\
-- Blitz: 2008\
--- These are the current rankings of the ThomasTCE1 lichess-bot account and are subject to change.\
Testing:\
-- TTCE passes all tested PERFT test positions, many of which can be found inside defs.h and ttce.c

# Protocols
TTCE implements the following protocols:\
-- Universal Chess Interface (UCI - this is how TTCE interacts with lichess.org)\
-- Xboard/Winboard Protocol (Will not be updated for future commits)\
-- Custom Console Protocol (Useful for debugging / in place of GUI)\

# Running TTCE
TTCE is very simple to run:
1. First, make sure you have GCC installed 
2. Type your installed make command (i.e. mingw32-make)
3. Run the compiled executable (i.e. ./ttce1p2) (this is for console mode)
4. Install the executable to a GUI (to better utilize UCI and/or Xboard protocols)
5. Enjoy 

# Acknowledgements
1. TTCE was inspired by BlueFever Software's video series on a C chess engine, which can be found here: https://www.youtube.com/watch?v=bGAfaepBco4&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg
2. TTCE utilizes the multithreading implementation provided by Marcus Johnson in the public GitHub repository found here: https://github.com/tinycthread/tinycthread
3. TTCE uses by default the codekiddy.bin Polyglot Opening Book, and this repository also includes the Komodo.bin and Performance.bin books, all of which can be found here: https://chess.stackexchange.com/questions/35448/looking-for-polyglot-opening-books




