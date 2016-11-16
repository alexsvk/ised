#include "event_pages_updating_visitor.h" // IMPLEMENTS
#include "event_dlg.h" // AGGREGATES

namespace ised
{

void event_pages_updating_visitor::visit_simple_event(simple_event const & sevt)
{
    event_dlg_p->append_simple_event_page(sevt);
}

void event_pages_updating_visitor::visit_complex_event(complex_event const & cevt)
{
    event_dlg_p->append_complex_event_page(cevt);
}

} // namespace ised
