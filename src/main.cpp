#include <iostream>
#include <cstdlib>

int main( int argc, char const * argv[] )
{
  int n = argc<2 ? 100 : std::atoi(argv[1]);
  std::cout << n << std::endl;
#if defined DEBUG
  std::cout <<   "DEBUG!" << std::endl;
#endif
#if defined NDEBUG
  std::cout <<   "NOT DEBUG!" << std::endl;
#endif
}