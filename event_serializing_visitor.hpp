#ifndef EVENT_SERIALIZING_VISITOR_H
#define EVENT_SERIALIZING_VISITOR_H

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

template<class parser>
class event_serializing_visitor : public event_visitor
{
public:
    typedef QSharedPointer<parser> parser_ptr;
    event_serializing_visitor(parser_ptr const & parser_p_) : parser_ptr_(parser_p_)
    {
    }
    void visit_simple_event(simple_event const &) override;
    void visit_complex_event(complex_event const &) override;
private:
    parser_ptr parser_ptr_;

}; // class event_serializing_visitor

template<class parser>
void event_serializing_visitor<parser>::visit_simple_event(simple_event const & sevent)
{
    if ( parser_ptr_ )
    {
        parser_ptr_->store_simple_event_beginning();

        parser_ptr_->store_interval(sevent.get_interval());
        // TODO: it's not parser's business to check the values.
        if ( sevent.get_trigger_path().size() ) parser_ptr_->store_trigger_path(sevent.get_trigger_path());
        if ( sevent.get_pattern_path().size() ) parser_ptr_->store_pattern_path(sevent.get_pattern_path());

        parser_ptr_->store_simple_event_ending();
    }
    else throw std::runtime_error("Failed to visit a simple event : The parser was not allocated dynamic memory");

} // visit_simple_event

template<class parser>
void event_serializing_visitor<parser>::visit_complex_event(complex_event const & cevent)
{
    if ( parser_ptr_ )
    {
        parser_ptr_->store_complex_event_beginning();

        parser_ptr_->store_interval(cevent.get_interval());
        for ( auto const & sevent : cevent.get_events() ) visit_simple_event(sevent);

        parser_ptr_->store_complex_event_ending();
    }
    else throw std::runtime_error("Failed to visit a complex event : The parser was not allocated dynamic memory");

} // visit_complex_event

} // namespace ised

#endif // EVENT_SERIALIZING_VISITOR_H
