#ifndef PAGE_VISITOR_H
#define PAGE_VISITOR_H

/*!
 * \brief The page_visitor class defines an abstract interface for every page visitor.
 * \author Alexander Syvak.
 * \date July, 2013.
 */
#include <QSharedPointer>
#include "event.h" // AGGREGATES

namespace ised
{

class simple_event_page_widget;
class complex_event_page_widget;

class page_visitor
{
public:
    typedef QSharedPointer<event> event_ptr;
    page_visitor(event_ptr const & evt_ptr_): evt_ptr(evt_ptr_)
    {
    }
    virtual void visit_simple_event_page(simple_event_page_widget const &) = 0;
    virtual void visit_complex_event_page(complex_event_page_widget const &) = 0;
protected:
    event_ptr evt_ptr;

}; // class event_visitor

} // namespace ised

#endif // PAGE_VISITOR_H
