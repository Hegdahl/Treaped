#include <bitset>
#include <iostream>
#include <sstream>
#include <vector>

#include "../src/unicode.cpp"

int main() {
  const char s[]{"😎 결합 문자가 올바르게 처리됩니까?"};
  std::vector<UniCodePoint> u;

  {
  std::stringstream ss(s);
    UniCodePoint p;
    while (ss >> p)
      u.push_back(p);
  }

  for (auto x : u)
    std::cout << x;
  std::cout << '\n';

  assert(u.size() == 20);
}
