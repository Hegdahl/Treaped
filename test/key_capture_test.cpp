#include <iostream>
#include <vector>
#include <unistd.h>

#include "../src/key_capture.cpp"

int main() {
  KeyCapture kc;
  std::vector<UniCodePoint> v;

  int n = 10;
  while ((int)v.size() < n) {
    v.push_back(kc.next());
    std::cout << n-(int)v.size()+1 << std::endl;
  }

  for (auto x : v)
    std::cout << x << '\n';
}
