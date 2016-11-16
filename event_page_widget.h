#ifndef EVENT_PAGE_WIDGET_H
#define EVENT_PAGE_WIDGET_H

// PROJECT
#include <QWidget> // INHERITS
// LOCAL
#include "page_visitor.h" // USES

namespace ised
{

class event_page_widget : public QWidget
{
    Q_OBJECT
public:
    event_page_widget(QWidget* parent = 0): QWidget(parent)
    {
    }
    virtual void apply_visitor(page_visitor &&) = 0;
};

} // namespace ised

#endif // EVENT_PAGE_WIDGET_H
