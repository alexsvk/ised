#ifndef PATTERN_QUALIFIER_H
#define PATTERN_QUALIFIER_H

/*!
  \class pattern_qualifier's responsibility is to define a region of a pattern.
  \author Alexander Syvak.
  \date July, 2013.
*/

// PROJECT
//#include <QWidget> // INHERITS
#include <QPen> // USES
#include <QRegion> // COMPOSITION
#include <QMouseEvent> // USES
#include <QDialog> // INHERITS
#include <QApplication> // USES
#include <QDesktopWidget> // USES
// LOCALS
#include "ised_stream_parser.h" // USES
#include "event.h" // USES

class QPaintEvent;
class QMouseEvent;
class QPainter;

namespace ised
{

class pattern_qualifier : public QDialog//QWidget
{
    Q_OBJECT
public:
    pattern_qualifier(QDialog* parent = 0);

    inline void set_pattern_path(QString const & ptrn_path)
    {
        pattern_file_path = ptrn_path;
    }
    inline QString get_pattern_path() const
    {
        return pattern_file_path;
    }
    inline bool is_saved() const
    {
        return saved;
    }
    bool load_pattern(QString const & ptrn_path);
    inline void set_trigger_path(QString const & trigger_path_)
    {
        trigger_path = trigger_path_;
    }

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent* evt_raw_ptr);
    void mouseMoveEvent(QMouseEvent* evt_raw_ptr);
    void mouseReleaseEvent(QMouseEvent* evt_raw_ptr);
    void keyPressEvent(QKeyEvent* evt_raw_ptr);

private:
    inline QRect primary_screen_geomtry() const
    {
        return QApplication::desktop()->screenGeometry( QApplication::desktop()->primaryScreen() );
    }
    void draw_trigger();
    void on_validation_failure(QString const & text, ised::event::errors_container const & errors);
    inline bool at_empty_state() const
    {
        return region.rectCount() == 0;
    }
    void save();
    void grab_whole_screen_region();
    void store_pattern_into_temp_file();
    void receive_pattern_file_name();
    ised_stream_parser::ised_pattern_container build_ised_pattern();
    void save_region();
    void draw_region(QPainter*);
    void draw_region();
    inline int get_x(QMouseEvent* evt_raw_ptr) const
    {
        return evt_raw_ptr->x();
    }
    inline int get_y(QMouseEvent* evt_raw_ptr) const
    {
        return evt_raw_ptr->y();
    }
    void install_palette();
    bool ltop_is_set;
    QRegion region;
    int x1, y1, x2, y2, next_x, next_y;
    QPen elastic_rect_pen, region_pen;
    bool on_release, msg;
    QString pattern_file_path;
    bool saved;
    QString trigger_path;
};

} // namespace ised

#endif // PATTERN_QUALIFIER_H
