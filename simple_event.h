#ifndef SIMPLE_EVENT_H
#define SIMPLE_EVENT_H

/*!
  \brief The simple_event class defines a simple event in accordance with the Tris CTC specification.
  \author Alexander Syvak.
  \date July, 2013.
*/

// PROJECT
#include <QMetaType>
#include <QString>
// LOCAL
#include "event.h" // INHERITS
#include "ised_stream_parser.h" // COMPOSITION

namespace ised
{

class simple_event : public event
{
public:
    simple_event(QString const & id = "", event_interval const & interval_ = event_interval(), QString const & trigger_pattern_path_ = QString(), QString const & pattern_path_ = QString());
    inline QString const & get_trigger_path() const
    {
        return trigger_path_;
    }
    inline ised_stream_parser::ised_pattern_container get_ised_pattern() const
    {
        return ised_ptrn;
    }
    inline QString & trigger_path()
    {
        return trigger_path_;
    }
    inline QString const & get_pattern_path() const
    {
        return pattern_path_;
    }
    inline QString & pattern_path()
    {
        return pattern_path_;
    }
    inline QString type()
    {
        return "S";
    }
    virtual inline void print()
    {
        event::print();
        qDebug() << trigger_path_ << pattern_path_;
    }
    inline void apply_visitor(event_visitor && visitor) override
    {
        visitor.visit_simple_event(*this);
    }

    bool validate(errors_container & errors_str);
    bool validate_trigger_picture(errors_container & errors_str);
    bool validate_pattern_picture(errors_container & errors_str);
    inline QString print_name() override
    {
        return "Simple event";
    }
private:
    bool compare_with_screen_rect(QRect const & rhs_rect);

    QString trigger_path_;
    QString pattern_path_;
    QString const trigger_pic_format;
    ised_stream_parser::ised_pattern_container ised_ptrn;
}; // simple_event

} // namespace ised

Q_DECLARE_METATYPE(ised::simple_event)

#endif // SIMPLE_EVENT_H
