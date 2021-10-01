#include "../src/screen.cpp"

int main() {
  Screen screen;

  auto [h, w] = screen.size();

  screen.set_cursor(0, 0);
  std::cout << 'a' << std::flush;
  usleep(5e5);
  screen.set_cursor(1, 1);
  std::cout << 'b' << std::flush;
  usleep(5e5);
  screen.set_cursor(h-1, 0);
  std::cout << 'c' << std::flush;
  usleep(5e5);
  screen.set_cursor(h-2, 1);
  std::cout << 'd' << std::flush;
  usleep(5e5);
  screen.set_cursor(h-1, w-1);
  std::cout << 'e' << std::flush;
  usleep(5e5);
  screen.set_cursor(h-2, w-2);
  std::cout << 'f' << std::flush;
  usleep(5e5);
  screen.set_cursor(0, w-1);
  std::cout << 'g' << std::flush;
  usleep(5e5);
  screen.set_cursor(1, w-2);
  std::cout << 'h' << std::flush;
  usleep(5e5);

}
