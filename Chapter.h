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
  private:

};
