#pragma once
#include <bitset>
#include <cassert>
#include <termios.h>
#include <fcntl.h>

#include "singleton.cpp"
#include "unicode.cpp"

class KeyCapture : Singleton<KeyCapture> {

 public:
  KeyCapture() : pending_scroll(0) {
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

  UniCodePoint next_raw(bool blocking = true) {
    UniCodePoint res;
    while (!res.valid()) {
      int c = (blocking ? getchar() : getchar_if_available());

      if (c == -1) {
        assert(res.empty());
        res = UniCodePoint::eof();
        break;
      }

      bool ok = res.feed((char)c);
      assert(ok);
    }

    return res;
  }

  UniCodePoint next() {
    UniCodePoint res = next_raw();
    if (res == UniCodePoint('\033'))
      if (read_escaped())
        return UniCodePoint();
    return res;
  }

  int get_scroll() {
    return std::exchange(pending_scroll, 0);
  }

 private:
  struct termios old_termios, current_termios;

  int pending_scroll;
  int pending_move_ver;
  int pending_move_hor;

  int getchar_if_available() {
    int old_flag = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, old_flag | O_NONBLOCK);
    int res = getchar();
    fcntl(0, F_SETFL, old_flag);
    return res;
  }

  bool read_escaped() {
    UniCodePoint category_mark = next_raw(false);
    if (category_mark.raw() == -1U)
      return false;

    switch(category_mark.raw()) {
      case UniCodePoint('O').raw(): {
        auto direction_chr = next();
        if (direction_chr == UniCodePoint('A'))
          ++pending_scroll;
        else if (direction_chr == UniCodePoint('B'))
          --pending_scroll;
        else
          assert(0 && "unknown escape sequence");
      } break;

      case UniCodePoint('[').raw(): {
        read_csi();
      } break;

      default: {
        std::cerr << "unknown escape sequence: " << std::bitset<32>(category_mark.raw()) << std::endl;
      } break;
    }

    return true;
  }

  void read_csi() {

  }
};
