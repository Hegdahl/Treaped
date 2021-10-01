#include <bitset>
#include <iostream>
#include <vector>
#include <unistd.h>

#include "../src/key_capture.cpp"
#include "../src/screen.cpp"

int main() {
  Screen screen;
  KeyCapture kc;

  std::cout << "Press q to exit..." << std::endl;
  UniCodePoint p;
  while ((p = kc.next()) != UniCodePoint('q')) {
    if (!p.empty())
      std::cout << std::bitset<32>(p.raw()) << std::flush;

    int scroll = kc.get_scroll();
    if (scroll > 0)
      std::cout << "\033[" << scroll << "T" << std::flush;
    else if (scroll < 0)
      std::cout << "\033[" << -scroll << "S" << std::flush;
  }
}
