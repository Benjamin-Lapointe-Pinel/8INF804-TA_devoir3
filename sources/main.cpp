#include <iostream>
#include <list>

#include <bitmap.hpp>
#include <options.hpp>
#include <pixel.hpp>

////////////////////////////////////////
size_t first_of(const std::list<bool> &l)
{
  size_t c = 0;
  for (bool b : l)
    if (b)
      return c;
    else
      c++;
  return c;
}

bool is_mask(const bitmap<RGB> &mask, size_t x, size_t y)
{
  const RGB &m = mask.pixel(x, y);
  return m.r == 255 && m.g == 255 && m.b == 255;
}

bool box_blur(const bitmap<RGB> &input, const bitmap<RGB> &mask, bitmap<RGB> &output, size_t x, size_t y)
{
  RGBD o(0, 0, 0);
  double divider = 0;
  for (int i = -1; i < 3; i++)
  {
    for (int j = -1; j < 3; j++)
    {
      if (!(i == 0 && j == 0) && !is_mask(mask, x + i, y + j))
      {
        o = o + input.pixel(x + i, y + j);
        divider++;
      }
    }
  }

  if (divider != 0)
  {
    output.pixel(x, y) = o / divider;
    return true;
  }
  return false;
}

bool sharpen(const bitmap<RGB> &input, const bitmap<RGB> &mask, bitmap<RGB> &output, size_t x, size_t y)
{
  RGBD o(0, 0, 0);
  double divider = 0;

  if (!is_mask(mask, x - 1, y))
  {
    o = o + input.pixel(x - 1, y);
    divider++;
  }
  if (!is_mask(mask, x + 1, y))
  {
    o = o + input.pixel(x + 1, y);
    divider++;
  }
  if (!is_mask(mask, x, y - 1))
  {
    o = o + input.pixel(x, y - 1);
    divider++;
  }
  if (!is_mask(mask, x, y + 1))
  {
    o = o + input.pixel(x, y + 1);
    divider++;
  }

  if (divider != 0)
  {
    output.pixel(x, y) = o / divider;
    return true;
  }
  return false;
}

bool filter_inpaint_pass(const bitmap<RGB> &input, const bitmap<RGB> &mask, bitmap<RGB> &new_mask, bitmap<RGB> &output,
                         const options &opt)
{
  bool inpainted = false;
  for (size_t x = 1; x < mask.width() - 1; x++)
  {
    for (size_t y = 1; y < mask.height() - 1; y++)
    {
      if (is_mask(mask, x, y))
      {
        bool ip = false;
        switch (opt.filter)
        {
        case filter_type::X:
          ip = box_blur(input, mask, output, x, y);
          break;
        case filter_type::Y:
          ip = sharpen(input, mask, output, x, y);
          break;
        }

        if (ip)
        {
          new_mask.pixel(x, y) = RGB(0, 0, 0);
          inpainted = true;
        }
      }
    }
  }
  return inpainted;
}

void filter_inpaint(bitmap<RGB> &input, const bitmap<RGB> &mask, bitmap<RGB> &output, const options &opt)
{
  bitmap<RGB> m1(mask);
  bitmap<RGB> m2(mask);
  if (opt.magic)
  {
    while (filter_inpaint_pass(input, m1, m2, output, opt))
    {
      input = output;
      m1 = m2;
    }
  }
  else
  {
    filter_inpaint_pass(input, m1, m2, output, opt);
  }
}

////////////////////////////////////////
int main(int argc, char *argv[])
{
  try
  {
    options opt(argc, argv);

    switch (first_of({
      opt.help,
      opt.version,
    }))
    {
    case 0:
      options::show_help();
      break;
    case 1:
      options::show_version();
      break;

    default:
      // toutes les autres options

      bitmap<RGB> i(opt.input); // input
      bitmap<RGB> m(opt.mask);  // mask
      bitmap<RGB> o(opt.input); // output!

      if ((i.width() == m.width()) && (i.height() == m.height()))
      {
        filter_inpaint(i, m, o, opt);
        o.save(opt.output);
      }
      else
      {
        std::cerr << "source and mask size mismatch" << std::endl;
        return 1;
      }
    }
  }

  catch (boost::program_options::error &this_exception)
  {
    std::cerr << this_exception.what() << std::endl;
    return 1;
  }
  return 0;
}
