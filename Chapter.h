#pragma once

#include <string>

using namespace std;

class Chapter {
  public:
    int number;
    string title;
    string body;

    // Constructor for a Chapter.
    Chapter(int num, string tit, string bod) {
      this->number = num;
      this->title = tit;
      this->body = bod;
    }

    // Comparitor for sorting a vector of Chapter objects.
    bool operator < (const Chapter& ch2) const{
      return(number < ch2.number);
    }
  private:
};
