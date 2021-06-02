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

void inpaint(const bitmap<RGB> &input, const bitmap<RGB> &mask, bitmap<RGB> &output, const options &opt)
{
  for (size_t x = 1; x < mask.width() - 1; x++)
  {
    for (size_t y = 1; y < mask.height() - 1; y++)
    {
      const RGB &m = mask.pixel(x, y);
      if (m.r == 255 && m.g == 255 && m.b == 255)
      {
        RGB i(0, 0, 0);
        i = i + (input.pixel(x - 1, y - 1) * (1.0 / 5.0));
        i = i + (input.pixel(x + 0, y - 1) * (1.0 / 5.0));
        i = i + (input.pixel(x + 1, y - 1) * (1.0 / 5.0));
        i = i + (input.pixel(x - 1, y + 0) * (1.0 / 5.0));
        i = i + (input.pixel(x + 1, y + 0) * (1.0 / 5.0));
        i = i + (input.pixel(x - 1, y + 1) * (1.0 / 5.0));
        i = i + (input.pixel(x + 0, y + 1) * (1.0 / 5.0));
        i = i + (input.pixel(x + 1, y + 1) * (1.0 / 5.0));
        output.pixel(x, y) = i;
      }
    }
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
        inpaint(i, m, o, opt);
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
