#pragma once
#include <csignal>
#include <iostream>
#include <utility>
#include <sys/ioctl.h>
#include <unistd.h>

#include "singleton.cpp"

class Screen : Singleton<Screen> {
 public:
  auto size() {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return std::make_pair(ws.ws_row, ws.ws_col);
  }

  void set_cursor(unsigned short row, unsigned short col) {
    std::cout << "\033[" << row+1 << ";" << col+1 << "H";
  }

  static Screen * const lifetime;

  Screen() {
    std::cout << "\033[?1049h\033[2J";
    set_cursor(0, 0);
    std::cout << std::flush;

    std::cerr << "abc\n";
  }

  ~Screen() {
    std::cout << "\033[?1049l";
  }
};
