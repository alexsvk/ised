#ifndef SIMPLE_EVENT_PAGE_VISITOR_H
#define SIMPLE_EVENT_PAGE_VISITOR_H

#include "page_visitor.h" // INHERITS

namespace ised
{

class simple_event_page_visitor : public page_visitor
{
public:
    simple_event_page_visitor(event_ptr const & evt_ptr): page_visitor(evt_ptr)
    {
    }
    void visit_simple_event_page(simple_event_page_widget const &);
    void visit_complex_event_page(complex_event_page_widget const &);
};

} // namespace ised

#endif // SIMPLE_EVENT_PAGE_VISITOR_H
