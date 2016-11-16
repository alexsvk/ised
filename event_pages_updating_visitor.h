#ifndef EVENT_PAGES_UPDATING_VISITOR_H
#define EVENT_PAGES_UPDATING_VISITOR_H

/*!
  \brief Defines templated visitor using a configuration parser to serialize visitable events.
  \author Alexander Syvak.
  \date July, 2013.
  \attention The configuration parser must support the configuration format specific operations.
*/

// PROJECT
#include <QSharedPointer> // parser_ptr
// LOCAL
#include "simple_event.h" // USES
#include "complex_event.h" // USES
#include "event_visitor.h" // INHERITS

namespace ised
{

class event_dlg;

class event_pages_updating_visitor : public event_visitor
{
public:
    typedef event_dlg* event_dlg_ptr;
    //typedef QSharedPointer<event_dlg> event_dlg_ptr;
    event_pages_updating_visitor(event_dlg_ptr const & event_dlg_p_) : event_dlg_p(event_dlg_p_)
    {
    }
    void visit_simple_event(simple_event const & sevt) override;
    void visit_complex_event(complex_event const & cevt) override;
private:
    event_dlg_ptr event_dlg_p;

}; // class event_serializing_visitor

} // namespace ised


#endif // EVENT_PAGES_UPDATING_VISITOR_H
