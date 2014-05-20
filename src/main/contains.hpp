#ifndef h5346BC81_3A56_4805_A4BF_5B5B3EAA681A
#define h5346BC81_3A56_4805_A4BF_5B5B3EAA681A

#include <algorithm>

template< typename C , typename V >
bool contains( C const& c , V v )
{
  return std::find(c.begin(),c.end(),v) != c.end();
}

#endif
