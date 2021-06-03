#ifndef __MODULE_PIXEL__
#define __MODULE_PIXEL__

#include <cstdint>
#include <cstddef>

template <typename T> class pixel
{
public:
  T r, g, b;

  T operator[](size_t i) const
  {
    switch (i)
    {
      case 0: return r;
      case 1: return g;
      default:
      case 2: return b;
    }
  }

  T &operator[](size_t i)
  {
    switch (i)
    {
      case 0: return r;
      case 1: return g;
      default:
      case 2: return b;
    }
  }

  bool operator==(const pixel &o) const { return (r == o.r) && (g == o.g) && (b == o.b); }
  bool operator!=(const pixel &o) const { return (r != o.r) || (g != o.g) || (b != o.b); }
  pixel operator+(const pixel &o) const { return pixel(r + o.r, g + o.g, b + o.b); }
  pixel operator-(const pixel &o) const { return pixel(r - o.r, g - o.g, b - o.b); }
  template <typename P> pixel operator*(const P &o) const { return pixel(r * o, g * o, b * o); }
  template <typename P> pixel operator/(const P &o) const { return pixel(r / o, g / o, b / o); }

  template <typename P> operator pixel<P>() const { return pixel<P>(r, g, b); }

  pixel(T r_ = 0, T g_ = 0, T b_ = 0) : r(r_), g(g_), b(b_)
  {
  }

  ~pixel() = default;
};

typedef pixel<int16_t> RGB16;
typedef pixel<uint8_t> RGB8;
typedef pixel<float> RGBF;
typedef pixel<double> RGBD;
typedef RGB8 RGB;

extern const RGB white;
extern const RGB black;

#endif
// __MODULE_PIXEL__
