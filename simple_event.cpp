// PROJECT
#include <QRect> // USES
#include <QApplication> // USES
#include <QDesktopWidget> // USES
#include <QPixmap> // USES

// LOCAL
#include "simple_event.h" // IMPLEMENTS
#include "ised_stream_parser.h" // USES

namespace ised
{

simple_event::simple_event(QString const & id, event_interval const & interval_, QString const & trigger_pattern_path__, QString const & pattern_path__):
    event(id, interval_), trigger_path_(trigger_pattern_path__), pattern_path_(pattern_path__), trigger_pic_format("PNG")
{
}

bool simple_event::validate(errors_container & errors_str)
{
    bool validation_res = true;//false;
    if ( trigger_path_.size() ) if ( !validate_trigger_picture(errors_str) && validation_res ) validation_res = false;
    if ( pattern_path_.size() ) if ( !validate_pattern_picture(errors_str) && validation_res ) validation_res = false;
    return validation_res;
}

bool simple_event::compare_with_screen_rect(QRect const & rhs_rect)
{
    auto desktop_raw_ptr = QApplication::desktop();
    if ( desktop_raw_ptr == nullptr ) throw std::runtime_error("Failed to compare screen rectangle with a rectangle : The desktop pointer is invalid");
    QRect const screen_rect = desktop_raw_ptr->screenGeometry();
    return screen_rect == rhs_rect;
}

bool simple_event::validate_pattern_picture(errors_container & errors_str)
{
    ised_stream_parser ised_stream_rdr(pattern_path_);
    ised_stream_rdr >> ised_ptrn;
    try
    {
        if ( !compare_with_screen_rect( static_cast<QRect>(ised_ptrn) ) )
        {
            errors_str << QString("Invalid pattern resolution : %1").arg(pattern_path_);
            pattern_path_.resize(0);
            return false;
        }
    }
    catch(std::runtime_error & )
    {
        throw;
    }
    return true;
}

bool simple_event::validate_trigger_picture(errors_container & errors_str)
{
    try
    {
        QPixmap trigger_png(trigger_path_, trigger_pic_format.toStdString().c_str());
        if ( trigger_png.isNull() ) throw std::runtime_error("Failed to validate trigger picture : The picture does not exist or the format is uknown");
        if ( !compare_with_screen_rect( trigger_png.rect() ) )
        {
            errors_str << QString("Invalid trigger resolution : %1").arg(trigger_path_);
            trigger_path_.resize(0);
            return false;
        }
    }
    catch(std::runtime_error & )
    {
        throw;
    }
    return true;
}

} // namespace ised
