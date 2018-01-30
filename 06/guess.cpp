#include<iostream>
using namespace std;

bool check_guess(const int guess, const bool new_answer) {
  static int answer;

  if(new_answer) {
    // Pick a number between 1 and 10
    answer = 1 + rand() % 10;
  }

  return guess == answer;
}

void play_game() {
  int guess;
  bool got_it = true;

  do {
    cout << "Enter your guess: ";
    cin >> guess;

    got_it = check_guess(guess, got_it);

    if(got_it) {
      cout << "You got it!" << endl;
    }
    else {
      cout << "NRRRRRR wrong! Try again!" << endl;
    }
  } while(!got_it);
}

int main() {
  char again;

  do {
    play_game();

    cout << "Want to play again? (y/n) ";
    cin >> again;
  } while(again == 'y' || again == 'Y');

  return 0;
}
