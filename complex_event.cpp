#include "complex_event.h" // // IMPLEMENTS

namespace ised
{

complex_event::complex_event(QString const & id, event_interval const & interval, events_container const & events__):
    event(id, interval), events_(events__)
{
}

bool complex_event::validate(errors_container & errors_str)
{
    bool validation_res = false;
    try
    {
        for ( auto & sevt : events_ ) if ( !sevt.validate(errors_str) && validation_res ) validation_res = false;
    }
    catch(std::runtime_error & )
    {
        throw;
    }
    return validation_res;
}

} // namespace ised
