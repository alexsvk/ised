#ifndef NULL_DELETER_H
#define NULL_DELETER_H

namespace ised
{

struct null_deleter
{
    void operator()(void const *) const
    {
    }
};

} // namespace ised

#endif // NULL_DELETER_H
