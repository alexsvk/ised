// SYSTEM
#include <string> // (*)

// PROJECT
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>

#include <boost/lexical_cast.hpp> // instead of QString::number() (*)

// LOCAL
#include "simple_event_page_widget.h" // IMPLEMENTS
#include "simple_event.h" // AGGREGATES

namespace ised
{

simple_event_page_widget::simple_event_page_widget
(
    sevent_ptr const & sevt_ptr_,
    QString const & interval_grp_label_title_,
    QString const & interval_low_bound_label_title_,
    QString const & interval_upr_bound_label_title_,
    QString const & trigger_path_label_title_,
    QString const & pattern_path_label_title_,
    QString const & max_buf_size_label_title_,
    QString const & max_disk_buf_size_label_title_,
    QWidget *parent
):  sevt_ptr(sevt_ptr_),
    interval_grp_label_title(interval_grp_label_title_),
    interval_low_bound_label_title(interval_low_bound_label_title_),
    interval_upr_bound_label_title(interval_upr_bound_label_title_),
    trigger_path_label_title(trigger_path_label_title_),
    pattern_path_label_title(pattern_path_label_title_),
    max_buf_size_label_title(max_buf_size_label_title_),
    max_disk_buf_size_label_title(max_disk_buf_size_label_title_),
    event_page_widget(parent)
{
}

// OPERATIONS

void simple_event_page_widget::build()
{
    try
    {
        make_interval_widget();
        make_trigger_and_ram_buf_layout();
        make_pattern_and_disk_bufs_layout();
        make_main_layout();
    }
    catch ( std::bad_alloc & )
    {
        // todo
    }
    setLayout(main_layout_raw_ptr);
}

void simple_event_page_widget::apply_visitor(page_visitor && visitor)
{
    visitor.visit_simple_event_page(*this);
}

void simple_event_page_widget::make_main_layout()
{
    try
    {
        main_layout_raw_ptr = new QVBoxLayout;
        main_layout_raw_ptr->addWidget(intrv_grp_raw_ptr);
        page_bottom_layout_raw_ptr = new QHBoxLayout;
        page_bottom_layout_raw_ptr->addLayout(trigger_and_ram_buf_layout_raw_ptr);
        page_bottom_layout_raw_ptr->addLayout(pattern_and_disk_buf_layout_raw_ptr);
        main_layout_raw_ptr->addLayout(page_bottom_layout_raw_ptr);
    }
    catch ( std::bad_alloc & )
    {
        // todo
    }
}

void simple_event_page_widget::make_trigger_and_ram_buf_layout()
{
    try
    {
        // Trigger.
        trigger_layout_raw_ptr = lay_out_label_and_line_edit(
                                                                trigger_path_label_title,
                                                                sevt_ptr ? sevt_ptr->get_trigger_path() : QString(),
                                                                &trigger_path_label_raw_ptr,
                                                                &trigger_path_wdg_raw_ptr
                                                            );
        // Max. RAM buffer size.
        auto const low_bnd_str = ::boost::lexical_cast<std::string>(static_cast<unsigned int>(calculate_appropriate_ram_buf_size())); // A : QString::number()'s job is not safe.
        ram_buf_layout_raw_ptr = lay_out_label_and_line_edit(
                                                                max_buf_size_label_title,
                                                                low_bnd_str.c_str(),
                                                                &ram_buf_label_raw_ptr,
                                                                &ram_buf_wdg_raw_ptr
                                                            );
        ram_buf_layout_raw_ptr->setContentsMargins(0,10,0,0);
        // Max. RAM and disk buffers' sizes layout.
        trigger_and_ram_buf_layout_raw_ptr = new QVBoxLayout;
        trigger_and_ram_buf_layout_raw_ptr->addLayout(trigger_layout_raw_ptr);
        trigger_and_ram_buf_layout_raw_ptr->addLayout(ram_buf_layout_raw_ptr);
    }
    catch ( std::bad_alloc & )
    {
        //todo
    }
}

void simple_event_page_widget::make_pattern_and_disk_bufs_layout()
{
    try
    {
        // Pattern.
        pattern_layout_raw_ptr = lay_out_label_and_line_edit(
                                                                pattern_path_label_title,
                                                                sevt_ptr ? sevt_ptr->get_pattern_path() : QString(),
                                                                &pattern_path_label_raw_ptr,
                                                                &pattern_path_wdg_raw_ptr
                                                            );
        // Max. disk buffer size.
        auto const up_bnd_str = ::boost::lexical_cast<std::string>(static_cast<unsigned int>(calculate_appropriate_disk_buf_size())); // A : QString::number()'s job is not safe.
        disk_buf_layout_raw_ptr = lay_out_label_and_line_edit(
                                                                max_disk_buf_size_label_title,
                                                                up_bnd_str.c_str(),
                                                                &disk_buf_label_raw_ptr,
                                                                &disk_buf_wdg_raw_ptr
                                                            );
        disk_buf_layout_raw_ptr->setContentsMargins(0,10,0,0);
        // Max. RAM and disk buffers' sizes layout.
        pattern_and_disk_buf_layout_raw_ptr = new QVBoxLayout;
        pattern_and_disk_buf_layout_raw_ptr->addLayout(pattern_layout_raw_ptr);
        pattern_and_disk_buf_layout_raw_ptr->addLayout(disk_buf_layout_raw_ptr);
    }
    catch ( std::bad_alloc & )
    {
        //todo
    }
}

//void simple_event_page_widget::make_ram_and_disk_bufs_layout()
//{
//    try
//    {
//        // Max. RAM buffer size.
//        auto const low_bnd_str = ::boost::lexical_cast<std::string>(static_cast<unsigned int>(calculate_appropriate_ram_buf_size())); // A : QString::number()'s job is not safe.
//        ram_buf_layout_raw_ptr = lay_out_label_and_line_edit(
//                                                                max_buf_size_label_title,
//                                                                low_bnd_str.c_str(),
//                                                                &ram_buf_label_raw_ptr,
//                                                                &ram_buf_wdg_raw_ptr
//                                                            );
//        // Max. disk buffer size.
//        auto const up_bnd_str = ::boost::lexical_cast<std::string>(static_cast<unsigned int>(calculate_appropriate_disk_buf_size())); // A : QString::number()'s job is not safe.
//        disk_buf_layout_raw_ptr = lay_out_label_and_line_edit(
//                                                                max_disk_buf_size_label_title,
//                                                                up_bnd_str.c_str(),
//                                                                &disk_buf_label_raw_ptr,
//                                                                &disk_buf_wdg_raw_ptr
//                                                            );
//        // Max. RAM and disk buffers' sizes layout.
//        ram_and_disk_bufs_layout_raw_ptr = new QHBoxLayout;
//        ram_and_disk_bufs_layout_raw_ptr->addLayout(ram_buf_layout_raw_ptr);
//        ram_and_disk_bufs_layout_raw_ptr->addLayout(disk_buf_layout_raw_ptr);
//    }
//    catch( std::bad_alloc & )
//    {
//        // todo
//    }
//}

ised::event::event_ram_max_buf_atom simple_event_page_widget::calculate_appropriate_ram_buf_size()
{
    // todo
    return 0;
}

ised::event::event_disk_max_buf_atom simple_event_page_widget::calculate_appropriate_disk_buf_size()
{
    // todo
    return 0;
}

//void simple_event_page_widget::make_trigger_and_pattern_layout()
//{
//    try
//    {
//        // Trigger.
//        trigger_layout_raw_ptr = lay_out_label_and_line_edit(
//                                                                trigger_path_label_title,
//                                                                sevt_ptr->get_trigger_path(),
//                                                                &trigger_path_label_raw_ptr,
//                                                                &trigger_path_wdg_raw_ptr
//                                                            );
//        // Pattern.
//        pattern_layout_raw_ptr = lay_out_label_and_line_edit(
//                                                                pattern_path_label_title,
//                                                                sevt_ptr->get_pattern_path(),
//                                                                &pattern_path_label_raw_ptr,
//                                                                &pattern_path_wdg_raw_ptr
//                                                            );
//        // Trigger and pattern layout.
//        trigger_and_pattern_layout_raw_ptr = new QHBoxLayout;
//        trigger_and_pattern_layout_raw_ptr->addLayout(trigger_layout_raw_ptr);
//        trigger_and_pattern_layout_raw_ptr->addLayout(pattern_layout_raw_ptr);
//    }
//    catch ( std::bad_alloc & )
//    {
//        // todo
//    }
//}


QLayout* simple_event_page_widget::lay_out_label_and_line_edit(
                                                                QString const & label,
                                                                QString const & content,
                                                                QLabel** label_raw_dptr,
                                                                QLineEdit** lineedit_raw_dptr,
                                                                Qt::Orientation const & direction
                                                              )
{
    QLayout* layout_raw_ptr = nullptr;
    try
    {
        *label_raw_dptr = new QLabel(label);
        *lineedit_raw_dptr = new QLineEdit(content);
        qDebug() << content << (*lineedit_raw_dptr)->text();
        (*label_raw_dptr)->setBuddy(*lineedit_raw_dptr);
        if ( direction == Qt::Orientation::Vertical ) layout_raw_ptr = new QVBoxLayout;
        else layout_raw_ptr = new QHBoxLayout;
        layout_raw_ptr->addWidget(*label_raw_dptr);
        layout_raw_ptr->addWidget(*lineedit_raw_dptr);
    }
    catch ( std::bad_alloc & )
    {
        // todo
    }
    return layout_raw_ptr;
}

QLayout* simple_event_page_widget::make_interval_bound_layout(QString const & bound_label, QString const & content, QLabel** label_raw_dptr, QLineEdit** linedit_raw_dptr)
{
    QLayout* intrv_bound_layout_raw_ptr = nullptr;
    try
    {
        intrv_bound_layout_raw_ptr =
            lay_out_label_and_line_edit
            (
                bound_label,
                content,
                label_raw_dptr,
                linedit_raw_dptr
            );
    }
    catch ( std::bad_alloc & )
    {
        // todo
    }
    return intrv_bound_layout_raw_ptr;
}

QWidget* simple_event_page_widget::make_interval_widget()
{
    try
    {
        std::string low_bnd_str;
        if ( !(sevt_ptr->get_interval().first == sevt_ptr->get_interval().second) )
            // Set the min bound layout.
            low_bnd_str = sevt_ptr ? ::boost::lexical_cast<std::string>(static_cast<unsigned int>(sevt_ptr->get_interval().first)) : ""; // A : QString::number()'s job is not safe.
        intrv_low_bnd_layout_raw_ptr = make_interval_bound_layout(interval_low_bound_label_title, QString::fromAscii(low_bnd_str.c_str()), &interval_low_bnd_label_raw_ptr, &interval_low_bnd_wdg_raw_ptr);
        std::string upr_bnd_str;
        if ( !(sevt_ptr->get_interval().first == sevt_ptr->get_interval().second) )
            // Set the max bound layout.
            upr_bnd_str = sevt_ptr ? ::boost::lexical_cast<std::string>(static_cast<unsigned int>(sevt_ptr->get_interval().second)) : ""; // A : QString::number()'s job is not safe.
        intrv_up_bnd_layout_raw_ptr = make_interval_bound_layout(interval_upr_bound_label_title, QString::fromAscii(upr_bnd_str.c_str()), &interval_up_bnd_label_raw_ptr, &interval_up_bnd_wdg_raw_ptr);
        // Set the interval main layout.
        interval_layout_raw_ptr = new QHBoxLayout;
        interval_layout_raw_ptr->addLayout(intrv_low_bnd_layout_raw_ptr);
        interval_layout_raw_ptr->addLayout(intrv_up_bnd_layout_raw_ptr);
        intrv_grp_raw_ptr = new QGroupBox(interval_grp_label_title);
        intrv_grp_raw_ptr->setLayout(interval_layout_raw_ptr);
    }
    catch ( std::bad_alloc & )
    {
        // todo
    }
    return intrv_grp_raw_ptr;
}

// SETTERS

void simple_event_page_widget::set_interval_low_bound(ised::event::event_interval_atom const & lower_bnd)
{
    if ( sevt_ptr )
    {
        sevt_ptr->interval().first = lower_bnd;
        interval_low_bnd_wdg_raw_ptr->setText(::boost::lexical_cast<std::string>(static_cast<unsigned int>(lower_bnd)).c_str());
    }
}

void simple_event_page_widget::set_interval_upper_bound(ised::event::event_interval_atom const & upper_bnd)
{
    if ( sevt_ptr )
    {
        sevt_ptr->interval().second = upper_bnd;
        interval_up_bnd_wdg_raw_ptr->setText(::boost::lexical_cast<std::string>(static_cast<unsigned int>(upper_bnd)).c_str());
    }
}

void simple_event_page_widget::set_trigger_path(QString const & trigger_path)
{
    if ( sevt_ptr )
    {
        sevt_ptr->trigger_path() = trigger_path;
        trigger_path_wdg_raw_ptr->setText(trigger_path);
    }
}

void simple_event_page_widget::set_pattern_path(QString const & pattern_path)
{
    if ( sevt_ptr )
    {
        sevt_ptr->pattern_path() = pattern_path;
        pattern_path_wdg_raw_ptr->setText(pattern_path);
    }
}

void simple_event_page_widget::set_max_ram_buf_size(ised::event::event_ram_max_buf_atom const & max_ram_buf_size)
{
    if ( sevt_ptr )
    {
        sevt_ptr->max_ram_buf_size() = max_ram_buf_size;
        ram_buf_wdg_raw_ptr->setText(::boost::lexical_cast<std::string>(static_cast<unsigned int>(max_ram_buf_size)).c_str());
    }
}

void simple_event_page_widget::set_max_disk_buf_size(ised::event::event_disk_max_buf_atom const & max_disk_buf_size)
{
    if ( sevt_ptr )
    {
        sevt_ptr->max_disk_buf_size() = max_disk_buf_size;
        disk_buf_wdg_raw_ptr->setText(::boost::lexical_cast<std::string>(static_cast<unsigned int>(max_disk_buf_size)).c_str());
    }
}

void simple_event_page_widget::set_interval(ised::event::event_interval const & interval_)
{
    if ( sevt_ptr ) sevt_ptr->interval() = interval_;
}

// GETTERS

ised::event::event_interval simple_event_page_widget::get_interval() const
{
//    if ( !sevt_ptr ) throw std::runtime_error("Failed to get interval : The simple event pointer was not initialized");
//    return sevt_ptr->get_interval();
    qDebug() << "get_interval()";
   return ised::event::event_interval(get_interval_low_bound(),get_interval_upper_bound());
}

ised::event::event_interval_atom simple_event_page_widget::get_interval_low_bound() const
{
//    if ( !sevt_ptr ) throw std::runtime_error("Failed to get the lower interval bound : The simple event pointer was not initialized");
//    return sevt_ptr->get_interval().first;
    qDebug() << "get_interval_lower_bound()";
    auto const lowr_bnd_str = interval_low_bnd_wdg_raw_ptr->text();
    return lowr_bnd_str.size() ? ::boost::lexical_cast<ised::event::event_interval_atom>(lowr_bnd_str.toStdString()) : 0U;
}

ised::event::event_interval_atom simple_event_page_widget::get_interval_upper_bound() const
{
//    if ( !sevt_ptr ) throw std::runtime_error("Failed to get the upper interval bound : The simple event pointer was not initialized");
//    return sevt_ptr->get_interval().second;
    qDebug() << "get_interval_upper_bound()";
    auto const upr_bnd_str = interval_up_bnd_wdg_raw_ptr->text();
    return upr_bnd_str.size() ? ::boost::lexical_cast<ised::event::event_interval_atom>(upr_bnd_str.toStdString()) : 0U;
}

QString simple_event_page_widget::get_trigger_path() const
{
//    if ( !sevt_ptr ) throw std::runtime_error("Failed to get the trigger path : The simple event pointer was not initialized");
//    return sevt_ptr->get_trigger_path();
    return trigger_path_wdg_raw_ptr->text();
}

QString simple_event_page_widget::get_pattern_path() const
{
//    if ( !sevt_ptr ) throw std::runtime_error("Failed to get the pattern path : The simple event pointer was not initialized");
//    return sevt_ptr->get_pattern_path();
    return pattern_path_wdg_raw_ptr->text();
}

ised::event::event_ram_max_buf_atom simple_event_page_widget::get_max_ram_buf_size() const
{
//    if ( !sevt_ptr ) throw std::runtime_error("Failed to get the maximal RAM buffer size : The simple event pointer was not initialized");
//    return sevt_ptr->get_max_ram_buf_size();
    return ::boost::lexical_cast<ised::event::event_ram_max_buf_atom>(ram_buf_wdg_raw_ptr->text().toStdString());
}

ised::event::event_disk_max_buf_atom simple_event_page_widget::get_max_disk_buf_size() const
{
//    if ( !sevt_ptr ) throw std::runtime_error("Failed to get the maximal disk buffer size : The simple event pointer was not initialized");
//    return sevt_ptr->get_max_disk_buf_size();
    return ::boost::lexical_cast<ised::event::event_disk_max_buf_atom>(disk_buf_wdg_raw_ptr->text().toStdString());
}

void simple_event_page_widget::paintEvent(QPaintEvent *evt_raw_ptr)
{
    QWidget::paintEvent(evt_raw_ptr);
}

void simple_event_page_widget::update_()
{
    if ( sevt_ptr )
    {
        interval_low_bnd_wdg_raw_ptr->setText(::boost::lexical_cast<std::string>(static_cast<unsigned int>(sevt_ptr->get_interval().first)).c_str());
        interval_up_bnd_wdg_raw_ptr->setText(::boost::lexical_cast<std::string>(static_cast<unsigned int>(sevt_ptr->get_interval().second)).c_str());
        trigger_path_wdg_raw_ptr->setText(sevt_ptr->get_trigger_path());
        pattern_path_wdg_raw_ptr->setText(sevt_ptr->get_pattern_path());
        ram_buf_wdg_raw_ptr->setText(::boost::lexical_cast<std::string>(static_cast<unsigned int>(sevt_ptr->get_max_ram_buf_size())).c_str());
        disk_buf_wdg_raw_ptr->setText(::boost::lexical_cast<std::string>(static_cast<unsigned int>(sevt_ptr->get_max_disk_buf_size())).c_str());
    }
}

} // namespace ised
