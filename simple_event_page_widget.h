#ifndef SIMPLE_EVENT_PAGE_WIDGET_H
#define SIMPLE_EVENT_PAGE_WIDGET_H

// PROJECT
//#include <QWidget> // INHERITS
#include <QString>
// LOCAL
#include "event.h" // USES
#include "page_visitor.h" // USES
#include "event_page_widget.h" // INHERITS

class QLabel;
class QLineEdit;
class QLayout;
class QGroupBox;
class QHBoxLayout;
class QVBoxLayout;

namespace ised
{

class simple_event;

class simple_event_page_widget : public event_page_widget
{
    Q_OBJECT
public:

    // TYPES

    typedef QSharedPointer<simple_event> sevent_ptr;

    // CTOR

    explicit simple_event_page_widget(
                                        sevent_ptr const & sevt_ptr_,
                                        QString const & interval_grp_label_title_ = tr("Interval"),
                                        QString const & interval_low_bound_label_title_ = tr("Low bound [sec]"),
                                        QString const & interval_upr_bound_label_title_ = tr("Upper bound [sec]"),
                                        QString const & trigger_path_label_title_ = tr("Trigger path"),
                                        QString const & pattern_path_label_title_ = tr("Pattern path"),
                                        QString const & max_buf_size_label_title_ = tr("Max. RAM buffer size [Mb]"),
                                        QString const & max_disk_buf_size_label_title_ = tr("Max. hard disk buffer size [Mb]"),
                                        QWidget *parent = 0
                                      );

    // OPERATIONS

    static QLayout* lay_out_label_and_line_edit(QString const & label, QString const & content, QLabel** label_raw_dptr, QLineEdit** lineedit_raw_dptr, Qt::Orientation const & direction = Qt::Orientation::Vertical);
    void build();
    static ised::event::event_ram_max_buf_atom calculate_appropriate_ram_buf_size();
    static ised::event::event_disk_max_buf_atom calculate_appropriate_disk_buf_size();

    void paintEvent(QPaintEvent *);
    void update_();

    void apply_visitor(page_visitor &&);

    // SETTERS

    void set_interval(ised::event::event_interval const & interval_);
    void set_interval_low_bound(ised::event::event_interval_atom const & lower_bnd);
    void set_interval_upper_bound(ised::event::event_interval_atom const & upper_bnd);
    void set_trigger_path(QString const & trigger_path);
    void set_pattern_path(QString const & pattern_path);
    void set_max_ram_buf_size(ised::event::event_ram_max_buf_atom const & max_ram_buf_size);
    void set_max_disk_buf_size(ised::event::event_disk_max_buf_atom const & max_disk_buf_size);

    // GETTERS

    ised::event::event_interval get_interval() const;
    ised::event::event_interval_atom get_interval_low_bound() const;
    ised::event::event_interval_atom get_interval_upper_bound() const;
    QString get_trigger_path() const;
    QString get_pattern_path() const;
    ised::event::event_ram_max_buf_atom get_max_ram_buf_size() const;
    ised::event::event_disk_max_buf_atom get_max_disk_buf_size() const;

    // QUERIES

signals:
    
public slots:

private:

    // OPERATIONS

    QWidget* make_interval_widget();
    QLayout* make_interval_bound_layout(QString const & bound_label, QString const & content, QLabel** label_raw_dptr, QLineEdit** linedit_raw_dptr);
    //void make_trigger_and_pattern_layout();
   // void make_ram_and_disk_bufs_layout();
    void make_trigger_and_ram_buf_layout();
    void make_pattern_and_disk_bufs_layout();
    void make_main_layout();

    // FIELDS

    QLabel  *interval_low_bnd_label_raw_ptr,
            *interval_up_bnd_label_raw_ptr,
            *trigger_path_label_raw_ptr,
            *pattern_path_label_raw_ptr,
            *ram_buf_label_raw_ptr,
            *disk_buf_label_raw_ptr;
    QLineEdit  *interval_wdg_raw_ptr,
               *interval_low_bnd_wdg_raw_ptr,
               *interval_up_bnd_wdg_raw_ptr,
               *trigger_path_wdg_raw_ptr,
               *pattern_path_wdg_raw_ptr,
               *ram_buf_wdg_raw_ptr,
               *disk_buf_wdg_raw_ptr;
    QHBoxLayout  *interval_layout_raw_ptr,
                 *page_bottom_layout_raw_ptr;
    QLayout  *intrv_low_bnd_layout_raw_ptr,
             *intrv_up_bnd_layout_raw_ptr,
             *trigger_layout_raw_ptr,
             *pattern_layout_raw_ptr,
             *ram_buf_layout_raw_ptr,
             *disk_buf_layout_raw_ptr;
    QVBoxLayout *main_layout_raw_ptr,
                *trigger_and_ram_buf_layout_raw_ptr,
                *pattern_and_disk_buf_layout_raw_ptr;
    QGroupBox* intrv_grp_raw_ptr;
    QString const interval_grp_label_title,
                  interval_low_bound_label_title,
                  interval_upr_bound_label_title,
                  trigger_path_label_title,
                  pattern_path_label_title,
                  max_buf_size_label_title,
                  max_disk_buf_size_label_title;
    sevent_ptr sevt_ptr;

}; // class simple_event_page_widget

} // namespace ised

#endif // SIMPLE_EVENT_PAGE_WIDGET_H
