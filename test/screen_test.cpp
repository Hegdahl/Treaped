#include "../src/screen.cpp"

int main() {

  auto [h, w] = Screen::size();

  Screen::set_cursor(0, 0);
  std::cout << 'a';
  usleep(5e5);
  Screen::set_cursor(1, 1);
  std::cout << 'b';
  usleep(5e5);
  Screen::set_cursor(h-1, 0);
  std::cout << 'c';
  usleep(5e5);
  Screen::set_cursor(h-2, 1);
  std::cout << 'd';
  usleep(5e5);
  Screen::set_cursor(h-1, w-1);
  std::cout << 'e';
  usleep(5e5);
  Screen::set_cursor(h-2, w-2);
  std::cout << 'f';
  usleep(5e5);
  Screen::set_cursor(0, w-1);
  std::cout << 'g';
  usleep(5e5);
  Screen::set_cursor(1, w-2);
  std::cout << 'h';
  usleep(5e5);

}
