#include "comp.h"

Comp::Comp()
{
}
template <typename E> bool Comp::prior(E h1, E h2)
{
    if(h1.weight()<h2.weight())
        return true;
    return false;
}
