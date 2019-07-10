#pragma once

#include <string>

using namespace std;

class Chapter {
  public:
    int number;
    string title;
    string disclaimer;
    string body;

    // Constructor for a chapter.
    Chapter(int num, string tit, string disc, string bod) {
      this->number = num;
      this->title = tit;
      this->disclaimer = disc;
      this->body = bod;
    }

    // Comparitor for sorting a vector of Chapter objects.
    bool operator < (const Chapter& ch2) const{
      return(number < ch2.number);
    }
  private:
};
