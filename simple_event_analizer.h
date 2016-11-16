#ifndef SIMPLE_EVENT_ANALIZER_H
#define SIMPLE_EVENT_ANALIZER_H


// SYSTEM
#include <string>

// PROJECT
#include <QSharedPointer> // AGGREGATES
#include <QRegion> // USES
#include <QVector> // COMPOSITION
#include <QPixmap> // AGGREGATES
#include <QObject> // INHERITS
#include <QStringList>

// LOCAL
#include "event_analizer.h" // INHERITS
#include "../ised_event_dlg/simple_event.h" // AGGREGATES
//#include <boost/lexical_cast.hpp>

//class QTimer;

namespace ised
{

class simple_event_analizer : public event_analizer //event_analizer
{
    Q_OBJECT
public:

    // TYPES

    enum state {waiting, match};
    typedef QVector<QImage> screenshots_container;
    typedef QSharedPointer<simple_event> event_ptr;
    typedef QStringList errors_container;

    // CTOR

    explicit simple_event_analizer(event_ptr const & event_ptr_, frequency freq_, QString const & dest_path_, errors_container & errors_): // dest_path_ is a path constituting the user defined directory and subdir. named as the current datetime.
        event_analizer(freq_,dest_path_),
        dest_path(dest_path_),
        event_p(event_ptr_),
        errors(errors_),
//        stop(false),
        matched(false),
        sec_counter(0U),
        log_file_str("log.txt"),
        state_(waiting)
    {
    }
    
public:

    inline void set_screenshot(QImage const & screenshot)
    {
        screenshot_buf = screenshot;
    }

    // GETTERS

    inline screenshots_container const & get_screenshots() const
    {
        return screenshots;
    }
    inline state const & get_state() const
    {
        return state_;
    }
    //void run() override;

    // SIGNALS

signals:
    //void kill_tmr();
    void done();
    void need_screenshot(ised::simple_event_analizer*);

protected:
    void timerEvent(QTimerEvent *event_raw_ptr);

    // SLOTS

public slots:
    bool begin();
    void stop();

private slots:
    void on_screenshot_timeout();
    void on_analize_timeout();


private:

    // OPERATIONS

    inline QString mk_screenshot_path(unsigned const & idx)
    {
        return dest_path + "/" /*+ event_p->get_id() + "/"*/ + /*::boost::lexical_cast<std::string>( idx + 1 ).c_str()*/QString::number(idx + 1) + "." + event_analizer::trigger_format;
    }
    QRegion pattern();

    void collect_ram_garbage(int const & glut_secs_count);

    void dump_screenshots_to_hd();

    void notify_usr();

    // FIELDS

    static int const state_analizer_tmr_interval = 1000;
    QString const log_file_str;
    event_ptr event_p;
    int state_analizer_tmr_id, screenshot_tmr_id;
    bool matched;
    screenshots_container screenshots;
    state state_;
    typedef quint32 counter_type;
    counter_type sec_counter;
    errors_container & errors;
    QString dest_path;
    QImage screenshot_buf;

    //QTimer* screenshot_qtmr, *anal_qmr;
}; // class simple_event_analizer

} // namespace ised

#endif // SIMPLE_EVENT_ANALIZER_H
