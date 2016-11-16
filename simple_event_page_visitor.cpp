#include "simple_event_page_visitor.h" // IMPLEMENTS
#include "simple_event_page_widget.h" // USES
//#include "complex_event_page_widget.h" // USES
#include "simple_event.h" // AGGREGATES
#include "complex_event.h" // AGGREGATES

namespace ised
{
    void simple_event_page_visitor::visit_simple_event_page(simple_event_page_widget const & page)
    {
        simple_event* se_raw_ptr = static_cast<simple_event*>(static_cast<void*>(evt_ptr.data()));
        se_raw_ptr->interval() = page.get_interval();
        se_raw_ptr->trigger_path() = page.get_trigger_path();
        se_raw_ptr->pattern_path() = page.get_pattern_path();
        se_raw_ptr->max_ram_buf_size() = page.get_max_ram_buf_size();
        se_raw_ptr->max_disk_buf_size() = page.get_max_disk_buf_size();
    }

    void simple_event_page_visitor::visit_complex_event_page(complex_event_page_widget const & page)
    {
        //todo
    }

} // namespace ised
