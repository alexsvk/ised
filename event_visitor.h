#ifndef EVENT_VISITOR_H
#define EVENT_VISITOR_H

/*!
 * \brief The event_visitor class defines an abstract interface for every event visitor.
 * \author Alexander Syvak.
 * \date July, 2013.
 */

namespace ised
{
class simple_event;
class complex_event;

class event_visitor
{
public:
    virtual void visit_simple_event(simple_event const &) = 0;
    virtual void visit_complex_event(complex_event const &) = 0;

}; // class event_visitor

} // namespace ised

#endif // EVENT_VISITOR_H
