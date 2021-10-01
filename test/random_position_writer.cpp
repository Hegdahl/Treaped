#include <iostream>
#include <random>

#include "../src/unicode.cpp"
#include "../src/screen.cpp"
#include "../src/key_capture.cpp"

int main() {
  Screen screen;
  KeyCapture kc;

  std::mt19937 rng(1337);
  std::cout << "press q to exit";

  UniCodePoint p;
  while ((p = kc.next()) != UniCodePoint('q')) {
    auto [h, w] = screen.size();
    auto y = std::uniform_int_distribution<unsigned short>(0, h-1)(rng);
    auto x = std::uniform_int_distribution<unsigned short>(0, w-1)(rng);

    screen.set_cursor(y, x);
    std::cout << p << std::flush;
  }

}
