// PROJECT
#include <QVBoxLayout> // USES
#include <QLineEdit> // USES
#include <QLabel> // USES
#include <QPushButton> // USES
#include <QFileDialog> // USES
#include <QMenuBar> // AGGREGATES
#include <QInputDialog> // USES
#include <QMessageBox> // USES
#include <QAction> // AGGREGATES
#include <QThread> // AGGREGATES
#include <QDateTime> // USES
#include <QApplication>
#include <QDesktopWidget>
#include <QMutex>
//#include <boost/lexical_cast.hpp> // USES
// LOCAL
#include "ised_win.h" // IMPLEMENTS
#include "simple_event_analizer.h" // USES
#include "simple_event.h" // AGGREGATES
#include "event_dlg.h"
#include "null_deleter.h"

ised_win::ised_win(QWidget *parent)
    : QMainWindow(parent), dir("."), freq("1"), started(false), answer(ised::event_dlg::Rejected), start_btn_title(tr("Start")), stop_btn_title(tr("Stop"))
{
    connect(&event_dlg_, SIGNAL(config_was_accepted()), this, SLOT(accept_config()));

    // Install the layout.

    QVBoxLayout* main_layout_raw_ptr = new QVBoxLayout;
    QLabel* config_label = new QLabel("Configuration");
    config = new QLineEdit;
    config->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    config_label->setBuddy(config);
    QLayout* config_layout_raw_ptr = new QVBoxLayout;
    config_layout_raw_ptr->addWidget(config_label);
    config_layout_raw_ptr->addWidget(config);
    main_layout_raw_ptr->addLayout(config_layout_raw_ptr);
    choose_config_raw_ptr = new QPushButton(tr("Choose a configuration"));
    connect(choose_config_raw_ptr, SIGNAL(clicked()), SLOT(choose_config()));
    main_layout_raw_ptr->addWidget(choose_config_raw_ptr);
    start_or_stop_raw_ptr = new QPushButton(start_btn_title);
    connect(start_or_stop_raw_ptr, SIGNAL(clicked()), SLOT(start_or_stop()));
    main_layout_raw_ptr->addWidget(start_or_stop_raw_ptr);
    QWidget* central_wdg_raw_ptr = new QWidget;
    central_wdg_raw_ptr->setLayout(main_layout_raw_ptr);
    setCentralWidget(central_wdg_raw_ptr);

    // Install the menu bar.

//    QAction* freq_action_raw_ptr = new QAction(tr("Frequency"), this);
//    freq_action_raw_ptr->setToolTip(tr("Set the global frequency [picture/sec]"));
//    freq_action_raw_ptr->setShortcut(tr("Ctrl+1"));
//    connect(freq_action_raw_ptr, SIGNAL(triggered()), SLOT(set_freq()));
//   //menuBar()->addMenu(tr("&Frequency"))->addAction(freq_action_raw_ptr);

//    QAction* dir_action_raw_ptr = new QAction(tr("Destination directory"), this);
//    dir_action_raw_ptr->setToolTip(tr("Set the global destination directory"));
//    dir_action_raw_ptr->setShortcut(tr("Ctrl+2"));
//    connect(dir_action_raw_ptr, SIGNAL(triggered()), SLOT(set_dir()));
   // menuBar()->addMenu(tr("&Destination"))->addAction(dir_action_raw_ptr);

    auto freq_action_rp = menuBar()->addAction(tr("Frequency"), this, SLOT(set_freq()));
    freq_action_rp->setShortcut(tr("Ctrl+1"));
    freq_action_rp->setToolTip(tr("Set the global frequency [picture/sec]"));
    auto dest_dir_action_rp = menuBar()->addAction(tr("Destination"), this, SLOT(set_dir()));
    dest_dir_action_rp->setShortcut(tr("Ctrl+2"));
    dest_dir_action_rp->setToolTip(tr("Set the global destination directory"));
//    auto freq_menu = new QMenu;
//    freq_menu->addAction(freq_action_raw_ptr);
//    this->menuBar()->addMenu(freq_menu);
//    auto dir_menu = new QMenu;
//    dir_menu->addAction(dir_action_raw_ptr);
//    this->menuBar()->addMenu(dir_menu);
//    this->menuBar()->setVisible(true);
    setFixedWidth(400);
    setFixedHeight(150);
    //setWindowIcon(":/images");
}

ised_win::~ised_win()
{
    
}

void ised_win::accept_config()
{
//    if ( answer == QDialog::Accepted )
//    {
        qDebug() << QString("Accepted configuration %1").arg(event_dlg_.get_config_path());
        config->setText(event_dlg_.get_config_path());
//    }
}

void ised_win::set_dir()
{
    bool ok = true;
    QFileInfo dir_ = QFileDialog::getOpenFileName(this, tr("Choose a file in a destination directory"), ".");//QInputDialog::getText(this, tr("Set the global destination directory"), tr("Enter the directory"), QLineEdit::Normal, dir, &ok );
    if ( ok && dir_.size() ) dir = dir_.absoluteDir().canonicalPath();
}

void ised_win::set_freq()
{
    bool ok = true;
    auto freq_ = QInputDialog::getText(this, tr("Set the global frequency [picture/sec]"), tr("Enter the frequency"), QLineEdit::Normal, freq, &ok );
    if ( ok && freq_.size() ) freq = freq_;
}

void ised_win::choose_config()
{
    config_file_str = QFileDialog::getOpenFileName(this, tr("Choose a configuration"), ".", tr("XML files (*.xml);;All files(*.*)"));
    event_dlg_.set_config_file(config_file_str);
    try
    {
        event_dlg_.parse_config_file();
        event_dlg_.validate_events(); // N.B. !!! Each event trigger and pattern paths may be deleted.
        event_dlg_.reset_stacked_event_pages();
        event_dlg_.update_view(); // Sets view into back state. Hence, it queries events from the model and redraws itself.
        answer = event_dlg_.exec();
//        qDebug() << answer;
//        if ( answer == QDialog::Accepted )
//        {
//            qDebug() << QString("Acceped configuration %1").arg(event_dlg_.get_config_path());
//            config->setText(event_dlg_.get_config_path());
//        }
    }
    catch ( std::runtime_error & e )
    {
        QMessageBox::warning(this, tr("Choosing configuration related error!"),tr(e.what()));
    }
}

void ised_win::find_child_and_set_title(QString const & child_str, QString const & title)
{
    (void)child_str;
    (void)title;
//    auto start_btn = this->findChild<QPushButton*>(child_str);
//    if ( start_btn != nullptr ) start_btn->setText(title);
    //else throw std::runtime_error( "Failed to find the window child : The pointer received is null" );
}

void ised_win::on_started()
{
    started = false;
    terminate_analizers();
    start_or_stop_raw_ptr->setText(tr("Start"));
    menuBar()->setDisabled(false);
    choose_config_raw_ptr->setDisabled(false);
}

void ised_win::start_or_stop()
{
    try
    {
        if ( started ) on_started();
        else if ( config->text().size() )
        {
            started = true;
            menuBar()->setDisabled(true);
            choose_config_raw_ptr->setDisabled(true);
            analize();
            start_or_stop_raw_ptr->setText(tr("Stop"));
            while ( started && thread_wrapper_ptrs.size() )
            {
                QApplication::processEvents(); // N.B. !!! Thereafter an additional check on the state of the app. should be made in this case, e.g. for access to an event pointer !!!
                for ( int idx = 0; idx < thread_wrapper_ptrs.size(); ++idx )
                {
                    QApplication::processEvents();
                    if ( started && thread_wrapper_ptrs[idx]->is_done() ) thread_wrapper_ptrs.remove(idx);
                }
            }
            qDebug() << "Out of start event!";
            if ( started )
            {
                start_or_stop();
                QMessageBox(QMessageBox::Information, tr("Analysis' results"), tr("The events' analysis are finished!"), QMessageBox::Ok, this).exec();
            }
        }
        else QMessageBox::information(this, tr("Start/Stop policy"), tr("A rule was violated : No configuration was accepted yet"));
    }
    catch ( std::runtime_error & e )
    {
        on_started();
        QMessageBox::warning(this, tr("Internal error!"), tr(e.what()));
    }
}

QString ised_win::mk_destination() const
{
    auto const date = QDateTime::currentDateTime().date().toString("yy_M_d");
    auto const time = QDateTime::currentDateTime().time().toString("h_m_s_zzz");
    qDebug() << date << time;
    return dir + "/" + date + "_" + time;
}

void ised_win::mk_screenshot(ised::simple_event_analizer* analizer_raw_ptr)
{
    QMutex mutex;
    mutex.lock();
    analizer_raw_ptr->set_screenshot(QPixmap::grabWindow( QApplication::desktop()->winId() ).toImage());
    mutex.unlock();
}

void ised_win::analize()
{
    if ( !QFileInfo(dir).permission( QFile::WriteUser) )
    {
        QMessageBox( QMessageBox::Warning, tr("A rule violation!"), tr("No 'write' right is given in the destination folder") ).exec();
        return;
    }
    QString const destination = mk_destination();
    if ( !QDir().mkpath(destination) ) throw std::runtime_error(QString("Failed to analize : Failed to create %1").arg(destination).toStdString());
    for ( auto const & evt_ptr : event_dlg_.get_events() )
    {
        auto const evt_ptr_ = ised::simple_event_analizer::event_ptr(static_cast<ised::simple_event*>(static_cast<void*>(evt_ptr.data())), ised::null_deleter());
        auto const evt_dest = destination + "/" + evt_ptr->get_id();
        if ( !QDir().mkpath(evt_dest) ) throw std::runtime_error(QString("Failed to analize : Failed to create %1").arg(evt_dest).toStdString());
        auto const evt_analizer_raw_ptr =
                new ised::simple_event_analizer_wrapper(
                                                            errors,
                                                            evt_dest,
                                                            freq.toULong(),
                                                            //::boost::lexical_cast<ised::event_analizer::frequency>(freq.toStdString()),
                                                            evt_ptr_,
                                                            this
                                                       );
        thread_wrapper_ptrs.append(
                                    thread_wrapper_ptr( evt_analizer_raw_ptr )
                                  );

//        threads.append(
//                        thread_ptr(
//                                    new ised::simple_event_analizer(
//                                                                        ised::simple_event_analizer::event_ptr(
//                                                                                                                static_cast<ised::simple_event*>(
//                                                                                                                                                    static_cast<void*>(evt_ptr.data())
//                                                                                                                                                 )
//                                                                                                               ),
//                                                                        ::boost::lexical_cast<ised::event_analizer::frequency>(freq.toStdString()),
//                                                                        destination
//                                                                    )
//                                  )
//                      );
//        threads.back()->start();
    }
}

void ised_win::terminate_analizers()
{
    for ( auto & thread_wrapper_ptr : thread_wrapper_ptrs )
    {
        thread_wrapper_ptr->stop();
        thread_wrapper_ptr.clear(); // N.B. !!! The destructor of every event will call quit() and wait()!
    }
    thread_wrapper_ptrs.resize(0); // N.B. !!! The destructor of every event will call quit() and wait()!
}

