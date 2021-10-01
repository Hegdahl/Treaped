#include <iostream>
#include <vector>

#include "../src/key_capture.cpp"

int main() {
  KeyCapture kc;

  std::vector<UniCodePoint> v;
  int n = 10;
  for (int i = 0; i < n; ++i) {
    std::cout << n-i << std::endl;
    v.push_back(kc.next());
    if (v.back() == UniCodePoint::eof()) {
      v.pop_back();
      break;
    }
  }

  for (auto x : v)
    std::cout << x << '\n';
}
