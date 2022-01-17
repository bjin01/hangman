# hangman - c code
This is a hangman sample c code for c programing exercise.

The c gnu linux code can be compiled on Linux OS.

**The game works as below:**

* it has two optional input parameters:
  - -i - for input file which holds a list of words from which the program will choose one randomly.
    if not input file is given then the user will be asked to enter a word that should be guessed prior each round start.
  - -r is for a given number of play rounds user whish to play. e.g. -r 5 will play 5 rounds.
  
Each word's number of characters will be times 2 calculed as max guessing repeats. 
For instance if "hello" is the word, it has 5 characters and will be multiplied with 2 = 10 guess attempts max.
After 10 missed guesses the round is lost.

If the user guessed one character correctly then this attempt will be credited to the max number of guesses.

The user word input is hideen on stdin as you would enter a password.

Additionally a log file called hangman.log will be written in the same directory of hangman binary.

