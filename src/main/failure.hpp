#ifndef h1534B0A6_5DD0_479B_9B06_B6FD0B19312D
#define h1534B0A6_5DD0_479B_9B06_B6FD0B19312D

#include <stdexcept>
#include <string>

class failure
  : public std::runtime_error
{
public:
  failure( int failure_code , std::string message )
    : std::runtime_error(message)
    , failure_code_(failure_code)
  {}

  int failure_code() const { return failure_code_;}

private:
  int failure_code_;
};

#endif
