#pragma once
#include <csignal>
#include <iostream>
#include <utility>
#include <sys/ioctl.h>
#include <unistd.h>

class Screen {
 public:
  static auto size() {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return std::make_pair(ws.ws_row, ws.ws_col);
  }

  static void set_cursor(unsigned short row, unsigned short col) {
    std::cout << "\033[" << row+1 << ";" << col+1 << "H";
  }

 private:
  static Screen * const lifetime;

  Screen() {
    std::cout << "\033[?1049h\033[2J";
    set_cursor(0, 0);
    std::cout << std::flush;

    // Cleanup is in atexit instead of in destructor,
    // because the destructor might get called after
    // stdout is closed.
    atexit([](){
      cleanup();
    });

    signal(SIGINT, [](int signum){
      cleanup();
      exit(signum);
    });
  }

  static void cleanup() {
    std::cout << "\033[?1049l";
  }
};
Screen * const Screen::lifetime = new Screen();
