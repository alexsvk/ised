#ifndef ISED_WIN_H
#define ISED_WIN_H

// PROJECT
#include <QMainWindow> // INHERITS
#include <QString> // COMPOSITION
#include <QVector> // COMPOSITION
#include <QSharedPointer> // AGGREGATES
// LOCAL
#include "event_dlg.h" // USES
#include "simple_event_analizer_wrapper.h" // AGGREGATES
#include "simple_event_analizer.h"

class QThread;
class QLineEdit;

class ised_win : public QMainWindow
{
    Q_OBJECT
    
public:
    ised_win(QWidget *parent = 0);
    ~ised_win();
private slots:
    void accept_config();
    void choose_config();
    void start_or_stop();
    void set_freq();
    void set_dir();
    void mk_screenshot(ised::simple_event_analizer*);
private:
    void on_started();
    void find_child_and_set_title(QString const & child_str, QString const & title);
    void analize();
    void terminate_analizers();
    QString mk_destination() const;

    QString config_file_str;
    ised::event_dlg event_dlg_;
    QString freq;
    QString dir;
    bool started;
    int answer;
    QString const start_btn_title, stop_btn_title;
    //typedef QSharedPointer<QThread> thread_ptr;
    //typedef QVector<thread_ptr> threads_container;
    //threads_container threads;
    typedef QSharedPointer<ised::simple_event_analizer_wrapper> thread_wrapper_ptr;
    typedef QVector<thread_wrapper_ptr> thread_wrapper_ptr_container;
    thread_wrapper_ptr_container thread_wrapper_ptrs;
    ised::simple_event_analizer::errors_container errors;
    QLineEdit* config;
    QPushButton* choose_config_raw_ptr;
    QPushButton* start_or_stop_raw_ptr;
};

#endif // ISED_WIN_H
