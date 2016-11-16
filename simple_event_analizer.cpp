// PROJECT
#include <QTimerEvent> // USES
#include <QApplication> // USES
#include <QDesktopWidget> // USES
#include <QPixmap> // USES
#include <QDir> // USES
#include <QFile> // USES
#include <QDataStream> // USES
#include <QDateTime> // USES
#include <QFileInfo> // USES
#include <QMessageBox> // USES
//#include <boost/lexical_cast.hpp> // USES
#include <QTimer> // AGGREGATES
// LOCAL
#include "simple_event_analizer.h" // IMPLEMENTS
#include "images_comparator.h" // USES
#include "simple_event.h" // AGGREGATES
#include "ised_stream_parser.h" // USES
#include "logger.h"

namespace ised
{

void simple_event_analizer::notify_usr()
{
    qDebug() << "notify_usr()";
    QApplication::beep();
    try
    {
        auto const cur_datetime = QDateTime::currentDateTime();
        logger::log("MATCH : " + event_p->get_id() + " " + cur_datetime.time().toString(), dest_path + "/" + log_file_str);
    }
    catch ( std::runtime_error & e )
    {
        errors.append( QString("%2 logging failure : %1").arg(e.what(), this->event_p->get_id()) );
    }
}

QRegion simple_event_analizer::pattern()
{
    if ( !event_p ) return QRegion();
    ised_stream_parser region_stream( event_p->get_pattern_path() );
    ised_stream_parser::ised_pattern_container ised_ptrn;
    region_stream >> ised_ptrn;
    return ised_ptrn.region;
}

void simple_event_analizer::on_screenshot_timeout()
{
    qDebug() << "on_screenshot_timeout()" << sec_counter;
    QTime profiler;
    profiler.start();
    // Dump the screenshot in RAM.
//    QPixmap screenshot;
    try
    {
//        auto const this_thread = this->thread();
//        moveToThread(QApplication::instance()->thread());
//        screenshot = QPixmap::grabWindow( QApplication::desktop()->winId() );
//        moveToThread(this_thread);
        emit need_screenshot(this);
        auto const & screenshot = screenshot_buf;
        //screenshot.save(event_p->get_id() + "buf.png");

        if ( screenshot.width() > 0 && screenshot.height() > 0 )
        {
            screenshots.append(screenshot);
            // Compare it with the trigger if there was no match before.
            if ( !matched && event_p && event_p->get_trigger_path().size() && images_comparator::cmp( std::move(screenshot)/*.toImage()*/, QImage( event_p->get_trigger_path() ), pattern() ) )
            {
                qDebug() << "!!! matched !!!";
                matched = true;
                notify_usr();
            }
        }
    }
    catch (std::exception &)
    {
        qDebug() << "on_screenshot_timeout cannot grab window";
    }
    qDebug() << "Elapsed time for comparison: " << profiler.elapsed()/1000.0 << "[sec]";
}

void simple_event_analizer::dump_screenshots_to_hd()
{
    qDebug() << "dump_screenshots_to_hd()";
    unsigned idx = 0U;
    for ( auto const & screenshot : screenshots ) screenshot.save( mk_screenshot_path(idx++), event_analizer::trigger_format.toStdString().c_str() );
}

void simple_event_analizer::collect_ram_garbage(int const & glut_secs_count)
{
    auto deletions_count = glut_secs_count * freq;
    while ( deletions_count-- ) if ( screenshots.size() ) screenshots.remove(0);
}

void simple_event_analizer::on_analize_timeout()
{
    qDebug() <<  event_p->get_id() + " - on_analize_timeout()" << sec_counter;
    sec_counter += static_cast<counter_type>(state_analizer_tmr_interval) / 1000; // 1 second passed
    qDebug() << matched << state_;
    if ( state_ == waiting )
    {
        // Collect garbage.
        auto const glut_secs_count = static_cast<int>(sec_counter) - static_cast<int>(event_p->get_interval().first);
        if ( glut_secs_count > 0 )
        {
            collect_ram_garbage( glut_secs_count );
            sec_counter = static_cast<counter_type>( glut_secs_count );
        }
        // Move to the next state if a screenshot matched.
        if ( matched )
        {
            state_ = match;
            sec_counter = 0U;
            qDebug() <<  event_p->get_id() + " !!! state is changed !!!";
        }
    }
    else if ( state_ == match && sec_counter >= static_cast<counter_type>(event_p->get_interval().second) )
    {
        qDebug() << event_p->get_id() + " !!! stoppping !!!";
        stop();
        sec_counter = 0U;
        dump_screenshots_to_hd();
        emit done();
    }
}

bool simple_event_analizer::begin()
{
    screenshot_tmr_id = startTimer(freq*1000);
    if ( screenshot_tmr_id == 0 ) return false;
    state_analizer_tmr_id = startTimer(state_analizer_tmr_interval);
    if ( state_analizer_tmr_id == 0 ) return false;
    return true;
}

void simple_event_analizer::stop()
{
    killTimer(screenshot_tmr_id);
    killTimer(state_analizer_tmr_id);
}

void simple_event_analizer::timerEvent(QTimerEvent *event_raw_ptr)
{
    //(void)event_raw_ptr;
    if ( event_raw_ptr != nullptr )
    {
        if ( event_raw_ptr->timerId() == screenshot_tmr_id ) on_screenshot_timeout();
        else if ( event_raw_ptr->timerId() == state_analizer_tmr_id ) on_analize_timeout();
    }
}

//void simple_event_analizer::run()
//{
//    QDir dest_dir(dest_path);
//    qDebug() << "run()" << dest_path;
//    if ( !dest_dir.exists() ) // Should exist.
//    {
//        QMessageBox( QMessageBox::Warning, tr("A rule violation!"), QString(tr("Failed to analize the simple event : The %1 directory does not exist")).arg(dest_path) ).exec();
//        return;
//    }
//    qDebug() << QString("%1 exists").arg(dest_path);
//    if ( !QFileInfo(dest_path).permission(QFile::WriteOther) ) // Should have 'write' permission for other.
//    {
//        QMessageBox( QMessageBox::Warning, tr("A rule violation!"), tr("No 'write' right is given for 'other' in the destination folder") ).exec();
//        return;
//    }
//    qDebug() << QString("%1 has 'write' permission for other").arg(dest_path);
//    if ( !dest_dir.mkdir( event_p->get_id() ) ) // Create the simple event own directory for dumping screenshots.
//    {
//        QThread::exit(1); // 1 - Cannot create the subdirectory.
//    }
//    qDebug() << QString("%1 directory was created").arg(dest_path + "/" + event_p->get_id());
//    //screenshot_tmr_id = startTimer(freq*1000);
//    if ( !screenshot_tmr_id ) return;
//    //state_analizer_tmr_id = startTimer(state_analizer_tmr_interval);
//    //moveToThread(this);
//    screenshot_qtmr = new QTimer;//(this);
//    screenshot_qtmr->moveToThread(this);
//    qDebug() << connect(screenshot_qtmr, SIGNAL(timeout()), this, SLOT(on_screenshot_timeout()));
//    qDebug() << connect(this, SIGNAL(kill_tmr()), screenshot_qtmr, SLOT(stop()));
//    screenshot_qtmr->start(freq*1000);
//    anal_qmr = new QTimer;//(this);
//    anal_qmr->moveToThread(this);
//    qDebug() << connect(anal_qmr, SIGNAL(timeout()), SLOT(on_analize_timeout()));
//    qDebug() << connect(this, SIGNAL(kill_tmr()), anal_qmr, SLOT(stop()));
//    anal_qmr->start(state_analizer_tmr_interval);
//    if ( !state_analizer_tmr_id ) return;
//    exec();
//    while ( !stop );
//}

} // namespace ised
