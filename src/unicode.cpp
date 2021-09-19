#pragma once
#include <cassert>
#include <cstdint>
#include <istream>
#include <ostream>

class UniCodePoint {
 public:
  UniCodePoint() : value(-2U) {}
  UniCodePoint(char c) : UniCodePoint() {
    assert(feed(c));
  }

  static UniCodePoint eof() {
    UniCodePoint p;
    p.value = -1U;
    return p;
  }

  bool empty() const {
    return *this == UniCodePoint();
  }

  bool feed(char c) {
    uint_least32_t x = (uint8_t)c;

    if (empty()) {
      if (byte_type(x) <= 0)
        return false;

      value = x << 3*8;
      return true;
    }

    if (byte_type(x) != 0)
      return false;

    int w = byte_type(value >> 3*8);
    if (w <= 1)
      return false;

    for (int i = 1; i < w; ++i) {
      uint_least32_t already = value >> (3-i)*8 & 0b11111111;
      if (already == 0) {
        value |= x << (3-i)*8;
        return true;
      } else {
        assert(byte_type(already) == 0);
      }
    }

    return false;
  }

  bool valid() const {
    if (*this == UniCodePoint::eof()) return true;

    int w = byte_type(value >> 3*8);
    if (w <= 0) return false;
    for (int i = 1; i < w; ++i)
      if (byte_type(value >> (3-i)*8) != 0)
        return false;
    return true;
  }

  friend bool operator==(const UniCodePoint &lhs, const UniCodePoint &rhs) {
    return lhs.value == rhs.value;
  }
  friend bool operator!=(const UniCodePoint &lhs, const UniCodePoint &rhs) {
    return lhs.value != rhs.value;
  }

  friend std::ostream &operator<<(std::ostream &os, const UniCodePoint &p) {
    assert(p.valid());
    assert(p != UniCodePoint::eof());
    int w = byte_type(p.value >> 3*8);
    for (int i = 0; i < w; ++i)
      os << char(p.value >> (3-i)*8);
    return os;
  }

  friend std::istream &operator>>(std::istream &is, UniCodePoint &p) {
    p = UniCodePoint();
    while (!p.valid()) {
      int c = is.get();

      if (c == -1) {
        if (!p.empty())
          is.setstate(is.failbit);
        p = UniCodePoint::eof();
        break;
      }

      if (!p.feed((char)c)) {
        is.putback((char)c);
        is.setstate(is.failbit);
        break;
      }
    }
    return is;
  }

 private:
  uint_least32_t value;

  static constexpr int byte_type(uint_least32_t x) {
    if ((x & 0b10000000) == 0b0'0000000) return 1;
    if ((x & 0b11000000) == 0b10'000000) return 0;
    if ((x & 0b11100000) == 0b110'00000) return 2;
    if ((x & 0b11110000) == 0b1110'0000) return 3;
    if ((x & 0b11111000) == 0b11110'000) return 4;
    return -1;
  }
};
