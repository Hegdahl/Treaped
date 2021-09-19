#pragma once
#include <termios.h>

#include "unicode.cpp"

class KeyCapture {
 public:

  static UniCodePoint next() {
    UniCodePoint res;
    while (!res.valid()) {
      int c = getchar();

      if (c == -1) {
        assert(res.empty());
        res = UniCodePoint::eof();
        break;
      }

      assert(res.feed((char)c));
    }
    return res;
  }

 private:
  static KeyCapture * const lifetime;

  auto operator=(KeyCapture) = delete;
  auto operator=(KeyCapture&) = delete;
  auto operator=(KeyCapture&&) = delete;

  KeyCapture() {
    tcgetattr(0, &old_termios);
    current_termios = old_termios;

    // disable buffering (waiting for newline)
    current_termios.c_lflag &= ~ICANON;

    // disable echoing
    current_termios.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &current_termios);
  }

  ~KeyCapture() {
    tcsetattr(0, TCSANOW, &old_termios);
  }

  struct termios old_termios, current_termios;
};
KeyCapture * const KeyCapture::lifetime = new KeyCapture();
