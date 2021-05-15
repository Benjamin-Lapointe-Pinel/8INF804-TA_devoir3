#include <iostream>
#include <list>

#include <options.hpp>
#include <pixel.hpp>
#include <bitmap.hpp>


////////////////////////////////////////
size_t first_of(const std::list<bool> & l)
 {
  size_t c=0;
  for (bool b: l)
   if (b)
    return c;
   else c++;
  return c;
 }

////////////////////////////////////////
int main(int argc, char * argv[])
 {
  try
   {
    options opt(argc,argv);

    switch (first_of({ opt.help,
                       opt.version,
                      }))
     {
      case 0: options::show_help(); break;
      case 1: options::show_version(); break;

      default:
       // toutes les autres options

       bitmap<RGB> i(opt.input); // input
       bitmap<RGB> m(opt.mask);  // mask
       bitmap<RGB> o;            // output!

       if ( (i.width()==m.width())
            && (i.height()==m.height()))
        {
         //inpaint(i,m,o,opt); //IMPLEMENTEZ MOI
         o.save(opt.output);
        }
       else
        {
         std::cerr << "source and mask size mismatch" << std::endl;
         return 1;
        }
     }
   }

  catch (boost::program_options::error & this_exception)
   {
    std::cerr << this_exception.what() << std::endl;
    return 1;
   }
  return 0;
 }
