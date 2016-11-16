#ifndef EVENT_H
#define EVENT_H

/*!
 *\brief The event class defines the base class for every event.
 *\author Alexander Syvak.
 *\date July, 2013.
*/

// PROJECT
#include <QPair> // event_interval
#include <QString> // id
#include <QStringList>
#include <QDebug>
// LOCAL
#include "event_visitor.h"

namespace ised
{

class event
{
public:
    typedef quint16 event_ram_max_buf_atom;
    typedef quint16 event_disk_max_buf_atom;
    typedef quint16 event_interval_atom;
    typedef QPair<event_interval_atom,event_interval_atom> event_interval;
    event(QString const & id__ = "", event_interval const & interval_ = event_interval());
    inline event_interval const & get_interval() const
    {
        return interval_;
    }
    inline QString const & get_id() const
    {
        return id_;
    }
    inline QString & id()
    {
        return id_;
    }

    inline event_interval & interval()
    {
        return interval_;
    }

    inline event_ram_max_buf_atom const & get_max_ram_buf_size() const
    {
        return max_ram_buf_size_;
    }

    inline event_disk_max_buf_atom const & get_max_disk_buf_size() const
    {
        return max_disk_buf_size_;
    }

    inline event_ram_max_buf_atom & max_ram_buf_size()
    {
        return max_ram_buf_size_;
    }

    inline event_disk_max_buf_atom & max_disk_buf_size()
    {
        return max_disk_buf_size_;
    }

    virtual inline QString type() = 0;

    virtual inline void print()
    {
        qDebug() << interval_;
    }

    virtual inline QString print_name()
    {
        return "Event";
    }

    virtual inline void apply_visitor(event_visitor &&) = 0;
    typedef QStringList errors_container;
    virtual bool validate(errors_container &) = 0;
private:
    event_interval interval_;
    QString id_;
    event_ram_max_buf_atom max_ram_buf_size_;
    event_disk_max_buf_atom max_disk_buf_size_;

}; // class event

} // namespace ised

#endif // EVENT_H
