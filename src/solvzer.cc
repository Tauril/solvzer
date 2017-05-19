#include <iostream>

#include <cube/cube.hh>

int main(int argc, char** argv)
{
  if (argc > 1)
    return 1;

  auto cube = cube::Cube();

  std::cout << cube << std::endl;

  return 0;
}
