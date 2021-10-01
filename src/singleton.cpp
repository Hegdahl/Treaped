#pragma once
#include <atomic>
#include <exception>
#include <string>

template<class T>
class Singleton {
 public:
  Singleton() {
    if (alive().exchange(true))
      throw std::runtime_error(
          std::string("More than 1 instance of singleton class \"") +
          std::string(typeid(T).name()) + 
          std::string("\" alive at the same time."));
  }

  ~Singleton() {
    alive() = false;
  }

 private:
  static std::atomic<bool> &alive() {
    static std::atomic<bool> value = false;
    return value;
  }
};
