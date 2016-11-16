// PROJECT
#include <QDebug>
#include <QApplication> // USES
#include <QDesktopWidget> // USES
#include <QPainter> // USES
#include <QWidget> // USES
#include <QEvent> // USES
#include <QPalette> // USES
#include <QAction> // AGGREGATES
#include <QMessageBox> // USES
#include <QFileDialog> // USES
#include <QTemporaryFile> // -//-
#include <QInputDialog> // -//-
#include <QLineEdit> // -//-
#include <QGridLayout>
// LOCAL
#include "pattern_qualifier.h" // IMPLEMENTS
#include "simple_event.h" // USES

namespace ised
{

pattern_qualifier::pattern_qualifier(QDialog* parent): /*QWidget*/QDialog(parent), ltop_is_set(false), x1(0), x2(0), y1(0), y2(0), on_release(false), msg(false), saved(false)
{
    install_palette();
    elastic_rect_pen.setStyle(Qt::PenStyle::DashLine);
    elastic_rect_pen.setColor(Qt::black);
    region_pen.setStyle(Qt::PenStyle::SolidLine);
    region_pen.setColor(Qt::black);

//    QAction* save_action = new QAction(this);
//    save_action->setShortcut(QKeySequence::Save);
//    connect(save_action,SIGNAL(triggered()), this, SLOT(save()));
}

void pattern_qualifier::on_validation_failure(QString const & text, ised::event::errors_container const & errors)
{
    QMessageBox msg_wdg(QMessageBox::Icon::Warning, tr("Validator detected violation!"), text, QMessageBox::Ok, this);
    if ( errors.size() > 0 ) msg_wdg.setDetailedText(errors.at(0));
    msg_wdg.exec();
    pattern_file_path.resize(0);
    saved = false;
    reject();
}

bool pattern_qualifier::load_pattern(const QString &ptrn_path)
{
    set_pattern_path(ptrn_path);
    ised::event::errors_container errors;
    try
    {
        simple_event sevt(0, ised::event::event_interval(), QString(), pattern_file_path);
        if ( !sevt.validate_pattern_picture(errors) ) on_validation_failure(tr("The information is in details. Below."), errors);
        else
        {
            ised_stream_parser::ised_pattern_container ptrn;
            ised_stream_parser(pattern_file_path) >> ptrn;
            auto const screen_geom = QApplication::desktop()->screenGeometry( QApplication::desktop()->primaryScreen() );
            if ( screen_geom.width() != ptrn.width || screen_geom.height() != ptrn.height ) return false;
            region = ptrn.region;
            saved = true;
        }
    }
    catch( std::runtime_error & e )
    {
        on_validation_failure(e.what(), errors);
    }
    return true;
}

void pattern_qualifier::mousePressEvent(QMouseEvent * evt_raw_ptr)
{
    if ( evt_raw_ptr != nullptr && !ltop_is_set
#if defined(__gnu_linux__) || defined(__GNU__)
         )
#else
         && evt_raw_ptr->button() == Qt::LeftButton )
#endif
    {
        x1 = x2 = get_x(evt_raw_ptr), y1 = y2 = get_y(evt_raw_ptr); // A : To avoid a shaking motion (pos() yields a position relative to a widget).
        ltop_is_set = true;
    }
    else QWidget::mousePressEvent(evt_raw_ptr);
}

void pattern_qualifier::mouseMoveEvent(QMouseEvent* evt_raw_ptr_)
{
    if ( hasMouseTracking() == false && evt_raw_ptr_ != nullptr && evt_raw_ptr_->button() == Qt::NoButton )
    {
        on_release = false, msg = true;
        next_x = evt_raw_ptr_->x(), next_y = evt_raw_ptr_->y();
        repaint();
        msg = false;
    }
    else QWidget::mouseMoveEvent(evt_raw_ptr_);
}

void pattern_qualifier::mouseReleaseEvent(QMouseEvent* evt_raw_ptr_)
{
    if ( evt_raw_ptr_ != nullptr
#if defined(__gnu_linux__) || defined(__GNU__)
         )
#else
         && evt_raw_ptr_->button() == Qt::LeftButton )
#endif
    {
        on_release = true, msg = true;
        next_x = evt_raw_ptr_->x(), next_y = evt_raw_ptr_->y();
        repaint();
        msg = false;
    }
    else QWidget::mouseReleaseEvent(evt_raw_ptr_);
}

void pattern_qualifier::install_palette()
{
    QPalette palette(pattern_qualifier::palette());
    palette.setColor(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, Qt::white);
    setPalette(palette);
}

void pattern_qualifier::draw_region(QPainter* painter)
{
    for ( auto const & rect : region.rects() ) painter->drawRect(rect);
}

void pattern_qualifier::draw_region()
{
    QPainter painter(this);
    auto const prev_pen = painter.pen();
    painter.setPen(region_pen);
    auto const prev_brush = painter.brush();
    painter.setBrush(QBrush(QColor(0,0,0,128)));
    draw_region(&painter);
    painter.setPen(prev_pen);
    painter.setBrush(prev_brush);
}

void pattern_qualifier::draw_trigger()
{
    QPainter painter(this);
    auto trigger_pixmap = QPixmap(trigger_path);
    painter.drawPixmap(0, 0, trigger_pixmap.scaled(this->size()));
}

void pattern_qualifier::paintEvent(QPaintEvent * evt_raw_ptr)
{
    Q_UNUSED(evt_raw_ptr)
    draw_trigger();
    if ( msg )
    {
        QPainter painter(this);
        painter.eraseRect( x1, y1,  x2 - x1 , y2 - y1 );
        painter.setRenderHint(QPainter::Antialiasing);
        if ( !on_release )
        {
            //qDebug() << "on_move" << "x1:" << x1 << "y1:" << y1 << "x2:" << x2 << "y2:" << y2;
            auto const prev_pen = painter.pen();
            painter.setPen(elastic_rect_pen);
            x2 = next_x, y2 = next_y;
            painter.drawRect( x1, y1, x2 - x1, y2 - y1 );
            painter.setPen(prev_pen);
        }
        else
        {
            ltop_is_set = false;
            //qDebug() << "on_release" << "x1:" << x1 << "y1:" << y1 << "x2:" << x2 << "y2:" << y2;
            if ( x1 > x2 ) std::swap(x1,x2);
            if ( y1 > y2 ) std::swap(y1,y2);
            region |= QRect( x1, y1, x2 - x1, y2 - y1 );
            saved = false;
        }
     }
    draw_region();
}

void pattern_qualifier::save()
{
    saved = true;
    save_region();
}

ised_stream_parser::ised_pattern_container pattern_qualifier::build_ised_pattern()
{
    ised_stream_parser::ised_pattern_container ised_ptrn;
    auto const screen_rect = primary_screen_geomtry();
    ised_ptrn.width = screen_rect.width(), ised_ptrn.height = screen_rect.height();
    ised_ptrn.region = region;
    return ised_ptrn;
}

void pattern_qualifier::store_pattern_into_temp_file()
{
    QTemporaryFile temp_file;
    if ( temp_file.open() ) // N.B. !!! In Reade/Write mode.
        set_pattern_path(temp_file.fileName());
    else throw std::runtime_error(
                                    QString(tr("Failed to receive the pattern file name : %1"))
                                        .arg(temp_file.errorString())
                                        .toStdString()
                                        .c_str()
                                 );
    QFile actual_ptrn_file(pattern_file_path);
    if ( !actual_ptrn_file.open(QFile::WriteOnly) )
        throw std::runtime_error(
                                    QString(tr("Failed to create the unique pattern file : %1"))
                                        .arg(actual_ptrn_file.errorString())
                                        .toStdString()
                                        .c_str()
                                );
    actual_ptrn_file.close();
}

void pattern_qualifier::receive_pattern_file_name()
{
    QMessageBox msg_wdg(QMessageBox::Question, tr("The pattern file name receival."), tr("Do you need to store the pattern into a file?"), QMessageBox::Yes|QMessageBox::No, this);
    if ( msg_wdg.exec() == QMessageBox::Yes )
    {
        if ( !pattern_file_path.size() )
        {

            auto const ptrn_file_str = QFileDialog::getSaveFileName( this, tr("Choose the pattern file name"), ".", tr("ISED files (*.ised)") );
            if ( ptrn_file_str.size() ) set_pattern_path(ptrn_file_str);
        }
    }
    else
        try
        {
            store_pattern_into_temp_file();
        }
        catch ( std::runtime_error & ) // N.B. !!! Either temp. file or the duplicate temp. file was not created in the current dir.
        {
            throw;
        }
}

void pattern_qualifier::save_region()
{
    receive_pattern_file_name();
    ised_stream_parser(pattern_file_path) << build_ised_pattern();
}

void pattern_qualifier::grab_whole_screen_region()
{
    region = primary_screen_geomtry();
    saved = false;
}

void pattern_qualifier::keyPressEvent(QKeyEvent* evt_raw_ptr)
{
    if ( evt_raw_ptr->key() == Qt::Key_Enter || evt_raw_ptr->key() == Qt::Key_Return ) // The user accepts the pattern.
    {
        if ( at_empty_state() ) grab_whole_screen_region();
        if ( !saved ) save();
        emit finished(Accepted);
    }
    else if  ( evt_raw_ptr == QKeySequence::Save ) // The user saves the pattern into a file.
    {
        if ( at_empty_state() ) QMessageBox(QMessageBox::Warning, tr("A rule violation!"), tr("There's nothing to save : No region was registered"), QMessageBox::Ok, this).exec();
        else if ( !saved ) save();
    }
    else QDialog::keyPressEvent(evt_raw_ptr);
}

} // namespace ised
