#include <iostream>
#include <vector>

#include "../src/key_capture.cpp"

int main() {
  std::vector<UniCodePoint> v;
  int n = 10;
  for (int i = 0; i < n; ++i) {
    std::cout << n-i << std::endl;
    v.push_back(KeyCapture::next());
    if (v.back() == UniCodePoint::eof()) {
      v.pop_back();
      break;
    }
  }

  for (auto x : v)
    std::cout << x << '\n';
}
