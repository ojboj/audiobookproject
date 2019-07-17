#pragma once

#include <string>
#include <vector>

using namespace std;

class Character {
  string name;
  vector<string> dialogue;

  public:

    // Constructor for a Character.
    Character(string nom, vector<string> dia) {
      this->name = nom;
      this->dialogue = dia;
    }

    // Comparitor for sorting a vector of Character objects.
    bool operator < (const Character& ch2) const{
      return(dialogue.size() < ch2.dialogue.size());
    }
  private:

};
