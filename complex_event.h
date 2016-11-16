#ifndef COMPLEX_EVENT_H
#define COMPLEX_EVENT_H

/*!
  \brief The complex_event class defines a complex event in accordance with the Tris CTC specification.
  \author Alexander Syvak.
  \date July, 2013.
*/

// PROJECT
#include <QLinkedList> // COMPOSITION
//#include <QMetaType>
// LOCAL
#include "event.h" // INHERITS
#include "simple_event.h" // COMPOSITION

namespace ised
{
class simple_event;

class complex_event : public event
{
public:
    typedef QLinkedList<simple_event> events_container;
    complex_event(QString const & id = "", event_interval const & interval = event_interval(), events_container const & events__ = events_container());
    inline events_container const & get_events() const
    {
        return events_;
    }
    inline events_container & events()
    {
        return events_;
    }
    inline QString type()
    {
        return "C";
    }

    virtual inline void print()
    {
        event::print();
        auto it = events_.begin();
        for ( ; it != events_.end(); ++it ) it->print();
    }
    inline void apply_visitor(event_visitor && visitor) override
    {
        visitor.visit_complex_event(*this);
    }
    bool validate(errors_container & errors_str);
    inline QString print_name() override
    {
        return "Complex event";
    }
private:
    events_container events_;

}; // complex_event

} // namespace ised

//Q_DECLARE_METATYPE(ised::complex_event)

#endif // COMPLEX_EVENT_H
