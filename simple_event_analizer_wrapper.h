#ifndef SIMPLE_EVENT_ANALIZER_WRAPPER_H
#define SIMPLE_EVENT_ANALIZER_WRAPPER_H

#include <QThread>
#include "simple_event_analizer.h"
#include "event_analizer.h"

namespace ised
{
    class simple_event_analizer_wrapper : public QObject
    {
        Q_OBJECT

    public:
        simple_event_analizer_wrapper(
                                        simple_event_analizer::errors_container & errors,
                                        QString const & destination,
                                        event_analizer::frequency const & freq,
                                        simple_event_analizer::event_ptr const & evt_p,
                                        QObject const* ised_win_raw_ptr
                                     ) : done_(false)
        {
            simple_event_analizer *analizer = new simple_event_analizer(evt_p, freq, destination, errors);
            analizer->moveToThread(&thread); // Pass event handling and slots/signals into the thread context.
            connect(&thread, SIGNAL(finished()), analizer, SLOT(deleteLater()));
            connect(this, SIGNAL(begin()), analizer, SLOT(begin()));
            //connect(this, SLOT(begin_timing()), analizer, SLOT(begin()));
            //connect(this, SIGNAL(stop()), analizer, SLOT(stop()));
            connect(analizer, SIGNAL(done()), SLOT(done()));
            thread.start(); // Enter the event loop.
            //analizer->begin(); // Start timers.
            qDebug() << "Connection => " <<
            connect(
                            analizer,
                            SIGNAL(need_screenshot(ised::simple_event_analizer*)),
                            ised_win_raw_ptr,
                            SLOT(mk_screenshot(ised::simple_event_analizer*)),
                            Qt::BlockingQueuedConnection

                            );
            emit begin();
        }
        ~simple_event_analizer_wrapper()
        {
            stop();
        }
        bool is_done()
        {
            return done_;
        }

    public slots:
        void stop()
        {
            thread.quit();
            thread.wait();
        }

//        void begin_timing()
//        {
//            emit begin();
//        }

    private slots:
        void done()
        {
            done_ = true;
            //emit done();
        }

    signals:
        void begin();
        //void stop();
        //void done();

    private:
        QThread thread;
        bool done_;
    };

} // namespace ised

#endif // SIMPLE_EVENT_ANALIZER_WRAPPER_H
