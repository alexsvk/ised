// PROJECT
#include <QHBoxLayout> // AGGREGATES
#include <QListView> // -//-
#include <QStackedWidget> // -//-
#include <QToolBar> // -//-
#include <QVBoxLayout> // -//-
#include <QLabel> // -//-
#include <QLineEdit> // -//-
#include <QGroupBox> // -//-
#include <QModelIndex> // USES
#include <QtGlobal> // qWarning
#include <QMessageBox> // USES
#include <QAction> // AGGREGATES
#include <QApplication> // USES: desktop()
#include <QDesktopWidget> // USES: availableGeometry()
#include <QFileDialog> // USES
#include <QStatusBar> // AGGREGATES
#include <QLinkedList> // USES
#include <QDebug>
#include <boost/lexical_cast.hpp> // USES: instead of QString::number()
#include <string> // USES: in conversion (see above)
// LOCAL
#include "event_dlg.h" // IMPLEMENTS
#include "events_list_model.h" // AGGREGATES, USES
#include "event_pages_updating_visitor.h" // USES
#include "simple_event_page_widget.h" // USES
#include "null_deleter.h" // USES
#include "pattern_qualifier.h" // USES
#include "imageviewer.h" // USES
//#include "event_selfupdating_visitor.h" // USES
#include "simple_event_page_visitor.h" // USES
#include "event_analizer.h" // USES

namespace ised
{

QString const event_dlg::add_simple_event_icon_path = ":/images/add_simple_event_icon.png";
QString const event_dlg::add_complex_event_icon_path = ":/images/add_complex_event_icon.png";
QString const event_dlg::del_event_icon_path = ":/images/del_event_icon.png";
QString const event_dlg::save_configuration_icon_path = ":/images/save_config_icon.png";
QString const event_dlg::accept_configuration_icon_path = ":/images/accept_config_icon.png";
QString const event_dlg::cancel_configuration_icon_path = ":/images/cancel_config_icon.png";
QString const event_dlg::event_dlg_icon_path = ":/images/event_dlg_icon.png";
QString const event_dlg::load_trigger_icon_path = ":/images/load_trigger_icon.png";
//QString const event_dlg::load_or_create_trigger_icon_path = ":/images/load_or_create_pattern_icon.png";
QString const event_dlg::load_pattern_icon_path = ":/images/load_pattern_icon.png";
QString const event_dlg::create_pattern_icon_path = ":/images/create_pattern_icon.png";
QString const event_dlg::open_current_pattern_icon = ":/images/open_current_pattern_icon.png";
QString const event_dlg::open_current_trigger_icon = ":/images/open_current_trigger_icon.png";

// CTOR and DTOR

event_dlg::event_dlg(QWidget *parent) :
    QDialog(parent), events_list_model_raw_ptr( new(std::nothrow)events_list_model(this) ), minimum_warn_msg_hspacer_width(400)
{
    // Set layout with widgets provided that the toolbar actions are created.
    create_actions();
    lay_out();
    // "Comb" the dialog.
    events_listview_raw_ptr->setFixedWidth(75);
    status_bar_raw_ptr = new (std::nothrow) QStatusBar;
    if ( status_bar_raw_ptr != nullptr ) layout()->addWidget(status_bar_raw_ptr);
    setWindowIcon(QIcon(event_dlg_icon_path));
}

event_dlg::~event_dlg()
{
}

// GETTERS

event_dlg::events_container event_dlg::get_events() const
{
    return events_list_model_raw_ptr->get_events();
}


simple_event_page_widget* event_dlg::get_page_at(const int & idx) const
{
    return static_cast<simple_event_page_widget*>(
                                                    static_cast<void*>
                                                    (
                                                        stacked_event_pages_raw_ptr->widget(idx)
                                                    )
                                                  );
}

simple_event_page_widget* event_dlg::get_current_page() const
{
    return static_cast<simple_event_page_widget*>(
                                                    static_cast<void*>
                                                    (
                                                        stacked_event_pages_raw_ptr->currentWidget()
                                                    )
                                                  );
}

// OPERATIONS

void event_dlg::create_toolbar()
{
    toolbars_layout_raw_ptr = new (std::nothrow) QHBoxLayout;

    edit_config_toolbar_raw_ptr = new (std::nothrow) QToolBar(tr("&Configuration"), this);
    edit_config_toolbar_raw_ptr->addAction(add_simple_event_action_raw_ptr);
    edit_config_toolbar_raw_ptr->addAction(add_complex_event_action_raw_ptr);
    edit_config_toolbar_raw_ptr->addAction(del_event_action_raw_ptr);
    edit_config_toolbar_raw_ptr->addAction(load_pattern_action_raw_ptr);
    edit_config_toolbar_raw_ptr->addAction(create_pattern_action_raw_ptr);
    edit_config_toolbar_raw_ptr->addAction(open_current_pattern_action_raw_ptr);
    edit_config_toolbar_raw_ptr->addAction(load_trigger_action_raw_ptr);
    edit_config_toolbar_raw_ptr->addAction(open_current_trigger_action_raw_ptr);
    toolbars_layout_raw_ptr->addWidget(edit_config_toolbar_raw_ptr);

    toolbars_layout_raw_ptr->addStretch(1);

    manage_config_toolbar_raw_ptr = new (std::nothrow) QToolBar(tr("&Manage configuration"), this);
    manage_config_toolbar_raw_ptr->addAction(save_configuration_action_raw_ptr);
    manage_config_toolbar_raw_ptr->addAction(accept_configuration_action_raw_ptr);
    manage_config_toolbar_raw_ptr->addAction(cancel_configuration_action_raw_ptr);
    toolbars_layout_raw_ptr->addWidget(manage_config_toolbar_raw_ptr);
}

void event_dlg::create_central_layout()
{
    central_layout_raw_ptr = new (std::nothrow) QHBoxLayout;
    events_listview_raw_ptr = new (std::nothrow) QListView;
    events_listview_raw_ptr->setModel(events_list_model_raw_ptr);
    central_layout_raw_ptr->addWidget(events_listview_raw_ptr);
    stacked_event_pages_raw_ptr = new (std::nothrow) QStackedWidget;
    central_layout_raw_ptr->addWidget(stacked_event_pages_raw_ptr, 1); // stretch factor is set
    connect(events_listview_raw_ptr, SIGNAL(clicked(QModelIndex const &)), this, SLOT(select_stacked_page(QModelIndex const &)));
}

void event_dlg::lay_out()
{ 
    main_layout_raw_ptr = new (std::nothrow) QVBoxLayout;
    create_toolbar();
    main_layout_raw_ptr->addLayout(toolbars_layout_raw_ptr);
    create_central_layout();
    main_layout_raw_ptr->addLayout(central_layout_raw_ptr);
    this->setLayout(main_layout_raw_ptr);
}

void event_dlg::create_action(QAction** action_double_raw_ptr, QString const & tip, QString const & icon_path, QString const & shortcut, QString const & slot, QString const & status_tip)
{
    *action_double_raw_ptr = new QAction(tip, this);
    QAction* action_raw_ptr = *action_double_raw_ptr;
    action_raw_ptr->setIcon(QIcon(icon_path));
    action_raw_ptr->setShortcut(shortcut);
    action_raw_ptr->setStatusTip(status_tip);
    connect(action_raw_ptr, SIGNAL(triggered()), this, slot.toStdString().c_str());
}

void event_dlg::create_actions()
{
    create_action(&add_simple_event_action_raw_ptr, tr("Add a &simple event"), add_simple_event_icon_path, tr("Ctrl+1"), SLOT(add_simple_event()), tr("Add a simple event - Ctrl+1"));
    create_action(&add_complex_event_action_raw_ptr, tr("Add a &complex event"), add_complex_event_icon_path, tr("Ctrl+2"), SLOT(add_complex_event()), tr("Add a complex event - Ctrl+2"));
    create_action(&del_event_action_raw_ptr, tr("&Delete an event"), del_event_icon_path, tr("Ctrl+3"), SLOT(del_event()), tr("Delete an event - Ctrl+3"));
    create_action(&save_configuration_action_raw_ptr, tr("&Save"), save_configuration_icon_path, tr("Ctrl+S"), SLOT(save_configuration()), tr("Save the configuration - Ctrl+S"));
    create_action(&accept_configuration_action_raw_ptr, tr("&Accept"), accept_configuration_icon_path, tr("Return"), SLOT(accept_configuration()), tr("Accept the configuration and return to ISED main window - Enter"));
    create_action(&cancel_configuration_action_raw_ptr, tr("Ca&ncel"), cancel_configuration_icon_path, tr("Esc"), SLOT(cancel_configuration()), tr("Cancel the configuration and return to ISED main window - Esc"));
    create_action(&load_trigger_action_raw_ptr, tr("&Load a trigger file"), load_trigger_icon_path, tr("Ctrl+T"), SLOT(load_trigger()), tr("Load a trigger file - Ctrl+T"));
    create_action(&load_pattern_action_raw_ptr, tr("Load a &pattern file"), load_pattern_icon_path, tr("Ctrl+P"), SLOT(load_pattern()), tr("Add a pattern file - Ctrl+P"));
    create_action(&create_pattern_action_raw_ptr, tr("&Create a pattern"), create_pattern_icon_path, tr("Ctrl+C"), SLOT(create_pattern()), tr("Create a pattern - Ctrl+C"));
    create_action(&open_current_trigger_action_raw_ptr, tr("Op&en the current trigger"), open_current_trigger_icon, tr("Ctrl+4"), SLOT(open_current_trigger()), tr("Open the current trigger - Ctrl+4"));
    create_action(&open_current_pattern_action_raw_ptr, tr("Open t&he current pattern"), open_current_pattern_icon, tr("Ctrl+5"), SLOT(open_current_pattern()), tr("Open the current pattern - Ctrl+5"));
}

void event_dlg::parse_config_file()
{
    if ( config_file_name.length() > 0 )
    {
        qDebug() << config_file_name;
        // Parse the config. XML file into QLinkedList of event pointers.
        events_list_model_raw_ptr->parse(config_file_name);

        // Set an ID for each event to manage events in GUI.
        quint8 idx = 0U;
        for ( auto & e : events_list_model_raw_ptr->get_events() )
            e->id() = e->type() + QString::number(idx++);
    }
}

void event_dlg::reset_stacked_event_pages()
{
    // Remove all the widgets are in the QStackedWidget.
    for ( int idx = 0; idx < stacked_event_pages_raw_ptr->count(); ++idx ) stacked_event_pages_raw_ptr->removeWidget(stacked_event_pages_raw_ptr->widget(idx));
    // Append a page for each event in sequence.
    for ( auto const & e : events_list_model_raw_ptr->get_events() )
        e->apply_visitor( event_pages_updating_visitor( this ) );
}

void event_dlg::append_simple_event_page(simple_event const & sevt)
{
    // Build the simple event page widget with populated interval, trigger and pattern paths by 'sevt'.
    QWidget* sevt_page_raw_ptr = make_simple_event_page(sevt);
    // Append the page to the QStackedWidget object.
    stacked_event_pages_raw_ptr->addWidget(sevt_page_raw_ptr);
}

void event_dlg::append_complex_event_page(complex_event const & cevt)
{
    // Build the complex event page widget with populated simple events in the QTableWidget.
    QWidget* cevt_page_raw_ptr = make_complex_event_page(cevt);
    // Append the page to the QStackedWidget object.
    stacked_event_pages_raw_ptr->addWidget(cevt_page_raw_ptr);
}

QWidget* event_dlg::make_simple_event_page(simple_event const & sevt)
{
    ised::simple_event_page_widget* sevt_page_raw_ptr = nullptr;
    try
    {
        sevt_page_raw_ptr = new ised::simple_event_page_widget(ised::simple_event_page_widget::sevent_ptr(new simple_event(sevt)));
        sevt_page_raw_ptr->build();
    }
    catch ( std::bad_alloc & )
    {
        return nullptr;
    }
    return sevt_page_raw_ptr;
}

QWidget* event_dlg::make_complex_event_page(complex_event const & )//cevt)
{
    // todo
    return nullptr;
}

void event_dlg::update_view()
{
    events_list_model_raw_ptr->reset_events();
}

//void event_dlg::showEvent(QShowEvent *)
//{
//// todo
//}

void event_dlg::add_simple_event()
{
    try
    {
        auto const sevt = simple_event( QString("S") + QString(::boost::lexical_cast<std::string>(events_list_model_raw_ptr->get_last_id_of_event_type() + 1).c_str()) );
        auto sevt_page_raw_ptr = make_simple_event_page(sevt);
        if ( !sevt_page_raw_ptr )
        {
            do_warn_usr(tr("Internal errror!"), tr("Failed to add a simple event : The simple event page was not built"));
            return;
        }
        stacked_event_pages_raw_ptr->addWidget( sevt_page_raw_ptr );
        events_list_model_raw_ptr->insert_rows( events_list_model_raw_ptr->get_events().size(), 1, events_list_model::event_type::simple );
    }
    catch ( std::runtime_error & e )
    {
        do_warn_usr(tr("Internal error!"), QString("Failed to add a simple event : %1").arg(e.what()));
    }
}

void event_dlg::add_complex_event()
{
// todo
}

void event_dlg::del_event()
{
    if ( events_listview_raw_ptr->currentIndex().row() != -1 )
    {
        if ( events_list_model_raw_ptr->removeRows( events_listview_raw_ptr->currentIndex().row(), 1 ) == false )
            do_warn_usr(tr("The policy violation!"), tr("Failed to delete an event : Wrong parameteres were passed to the model function"));
        stacked_event_pages_raw_ptr->removeWidget( stacked_event_pages_raw_ptr->widget( events_listview_raw_ptr->currentIndex().row() ) );
    }
    else warn_usr(tr("The policy violation!"), tr("<p><h1>No event is selected!</h1></p>"));
}

void event_dlg::receive_config_file_name()
{
/*    if ( !config_file_name.size() )
        while ( !config_file_name.size() ) */
    QFileInfo configf_info(config_file_name);
    do
    {
        config_file_name = QFileDialog::getSaveFileName(
                                                            this,
                                                            tr("Save the configuration"),
                                                            config_file_name.size() ? configf_info.canonicalFilePath() : ".",
                                                            tr("XML files(*.xml)")
                                                        );
    } while( !config_file_name.size() );
}

void event_dlg::save_configuration()
{
    // Synchronize GUI with actual events.
    auto idx = 0;
    for ( auto & evt : events_list_model_raw_ptr->get_events() ) //evt->apply_visitor( event_selfupdating_visitor( stacked_event_pages_raw_ptr->widget(idx++) ) );
        static_cast<event_page_widget*>(static_cast<void*>(stacked_event_pages_raw_ptr->widget(idx++)))->apply_visitor(simple_event_page_visitor(evt));
    // Define the destination file.
    receive_config_file_name();
    // Serialize the events in the XML format.
    events_list_model_raw_ptr->store_events(config_file_name);
}

void event_dlg::accept_configuration()
{
    save_configuration();
    //emit finished(Accepted);
    emit config_was_accepted();
    accept();
}

void event_dlg::cancel_configuration()
{
    //emit finished(Rejected);
    reject();
}

void event_dlg::select_stacked_page(QModelIndex const & idx)
{
    stacked_event_pages_raw_ptr->setCurrentIndex(idx.row());
}

void event_dlg::warn_usr(QString const & title, QString const & text, QString const & warning)
{
    QMessageBox msg_wdg(QMessageBox::Icon::Warning, title, text, QMessageBox::Ok, this, Qt::Dialog);
    if ( warning.size() )
    {
        msg_wdg.setDetailedText(warning);
        msg_wdg.setDefaultButton(QMessageBox::Ok);
        QSpacerItem* horizontalSpacer = new QSpacerItem(minimum_warn_msg_hspacer_width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = static_cast<QGridLayout *>(static_cast<void *>(msg_wdg.layout()));
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    }
    msg_wdg.exec();
}

void event_dlg::do_warn_usr(QString const & title, QString const & text, QString const & warning)
{
    QMessageBox msg_wdg(QMessageBox::Icon::Critical, title, text, QMessageBox::Ok, this, Qt::Dialog);
    if ( warning.size() )
    {
        msg_wdg.setDetailedText(warning);
        msg_wdg.setDefaultButton(QMessageBox::Ok);
        QSpacerItem* horizontalSpacer = new QSpacerItem(minimum_warn_msg_hspacer_width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = static_cast<QGridLayout *>(static_cast<void *>(msg_wdg.layout()));
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    }
    msg_wdg.exec();
}

bool event_dlg::validate_events()
{
    bool validation_res = true;
    ised::event::errors_container errors;
    try
    {
        for ( auto & evt : events_list_model_raw_ptr->get_events() ) if ( !evt->validate(errors) && validation_res ) validation_res = false;
    }
    catch( std::runtime_error & e)
    {
        do_warn_usr("Validation error!", e.what()); // A : The reason is a high probability of the parser mistake.
    }
    if ( !validation_res ) warn_usr( "Validator has detected violation(s)!", "The information is in details", errors.join("\n") );
    return validation_res;
}

QRect event_dlg::primary_screen_geometry() const
{
    auto const screen_rect = QApplication::desktop()->screenGeometry( QApplication::desktop()->primaryScreen() );
    qDebug() << "The primary screen size => " << screen_rect.width() << " x " << screen_rect.height();
    return screen_rect;
}

void event_dlg::load_trigger()
{
    auto const trigger_file_str = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Load a trigger file"),
                                                        ".",
                                                        QString("ISED trigger files (*%1)").arg(event_analizer::trigger_format)
                                                      );
    if ( trigger_file_str.size() )
    {
        try
        {
            if ( QPixmap(trigger_file_str).rect() != primary_screen_geometry() )
            {
                warn_usr("The policy violation","Failed to load the trigger : The trigger picture size does not fit the primary screen size");
                return;
            }
            auto current_event_page_raw_ptr = get_current_page();
            if ( current_event_page_raw_ptr != nullptr ) current_event_page_raw_ptr->set_trigger_path( trigger_file_str ); // Reset the event pattern path.
            else do_warn_usr(tr("Internal error!"), tr("The current event page pointer is invalid!"));
        }
        catch( std::runtime_error & e )
        {
            do_warn_usr( tr("Internal error!"), QString("Loading the trigger failure : %1").arg( e.what() ) );
        }
    }
}

void event_dlg::show_pattern_context_menu()
{
 // deprecated
}

void event_dlg::load_pattern()
{
    auto current_event_page_raw_ptr = get_current_page();
    if ( current_event_page_raw_ptr == nullptr )
    {
        do_warn_usr("Internal error","Failed to load a new pattern : The current event page raw pointer is null");
        return;
    }
    if ( current_event_page_raw_ptr->get_trigger_path().size() == 0 )
    {
        warn_usr("Policy violation","Failed to create a new pattern : No trigger was chosen");
        return;
    }
    auto const ptrn_file_str = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Load a pattern file"),
                                                        ".",
                                                        "ISED pattern files (*.ised)"
                                                      );
    if ( ptrn_file_str.size() )
    {
        try
        {
            qualify_pattern(ptrn_file_str);
        }
        catch( std::runtime_error & e )
        {
            do_warn_usr(tr("Loading the pattern failure!"), e.what());
        }
    }
}

void event_dlg::create_pattern()
{
    qDebug() << "create_pattern() slot's been called!";
    auto current_event_page_raw_ptr = get_current_page();
    if ( current_event_page_raw_ptr == nullptr )
    {
        do_warn_usr("Internal error","Failed to create a new pattern : The current event page raw pointer is null");
        return;
    }
    if ( current_event_page_raw_ptr->get_trigger_path().size() > 0 )
    {
        try
        {
            qualify_pattern();
        }
        catch( std::runtime_error & e)
        {
            do_warn_usr(tr("Pattern creation failure!"), e.what());
        }
    }
    else warn_usr("Policy violation","Failed to create a new pattern : No trigger was chosen");
}

void event_dlg::on_finish_pattern_qualifier(int result)
{
    show(); // set the event dialog visible
    if ( events_listview_raw_ptr->currentIndex().row() == -1) do_warn_usr(tr("The policy violation!"), tr("Failed to qualify the pattern : Current listview index is not set"));
    else if ( result == QDialog::Accepted )
    {
        auto current_event_page_raw_ptr = get_current_page();
        if ( current_event_page_raw_ptr != nullptr )
        {
            current_event_page_raw_ptr->set_pattern_path( ptrn_qualifier_wdg->get_pattern_path() ); // Reset the event pattern path.
            delete ptrn_qualifier_wdg;
        }
        else do_warn_usr(tr("Internal error!"), tr("The current event page pointer is invalid!"));
    }
}

void event_dlg::qualify_pattern(const QString & pattern_path)
{
    ptrn_qualifier_wdg = new (std::nothrow) pattern_qualifier;
    if ( ptrn_qualifier_wdg == nullptr ) throw std::runtime_error( "Failed to qualify the pattern : Failed to allocate memory for the pattern qualifier widget");
    //ptrn_qualifier_wdg->setAttribute(Qt::WA_DeleteOnClose); // the pattern qualifier widget will automatically collect its 'garbage'
    try
    {
        if ( pattern_path.size() )
            if ( !ptrn_qualifier_wdg->load_pattern(pattern_path) )
            {
                warn_usr("The policy violation","Failed to qualify the pattern : The pattern screen size does not fit the current screen size");
                return;
            }
    }
    catch ( std::runtime_error & e )
    {
        do_warn_usr(tr("Loading the pattern failure"), tr(e.what()));
        return;
    }
    auto current_event_page_raw_ptr = get_current_page();
    if ( current_event_page_raw_ptr == nullptr )
    {
        do_warn_usr("Internal error","Failed to qualify the pattern : The current event page raw pointer is null");
        return;
    }
    ptrn_qualifier_wdg->set_trigger_path( current_event_page_raw_ptr->get_trigger_path() );

    connect(ptrn_qualifier_wdg, SIGNAL(finished(int)), this, SLOT(on_finish_pattern_qualifier(int))); // install connection so the event dialog may restore itself after the widget is closed

    // N.B. !!! exec() ignores any setModal() call. Hence, it's always an application modal being called from exec().
    ptrn_qualifier_wdg->setModal(false); // make it window-modal
    ptrn_qualifier_wdg->setWindowFlags(Qt::FramelessWindowHint); // remove the frame so it also gets rid of the title bar
    auto const full_screen_rect = primary_screen_geometry(); // N.B. !!! Returns the geometry of the screen with index screen.                                                          // The default screen is used if screen is -1.
    ptrn_qualifier_wdg->setGeometry(full_screen_rect); // resize the widget to the whole screen
    ptrn_qualifier_wdg->showFullScreen();//showMaximized(); // make it visible
    ptrn_qualifier_wdg->raise(); // put it onto the stack top
    ptrn_qualifier_wdg->activateWindow(); // set the focus to the widget ensured it's on the stack top

    hide(); // hide current event dialog because only one event pattern may be set simultaneously
}

void event_dlg::open_current_pattern()
{
    if ( stacked_event_pages_raw_ptr->currentIndex() != -1 )
    {
        auto current_page_raw_ptr = get_current_page();
        try
        {
            if ( current_page_raw_ptr != nullptr )
            {
                if ( current_page_raw_ptr->get_trigger_path().size() == 0 ) warn_usr("The policy violation","Failed to open current pattern : No trigger was chosen");
                else
                    if ( current_page_raw_ptr->get_pattern_path().size() ) qualify_pattern(current_page_raw_ptr->get_pattern_path());
                    else warn_usr(tr("The policy violation!"), tr("<p><h2>No pattern is set!</h2></p>"));
            }
            else do_warn_usr(tr("Internal error!"), tr("<p><h1>The current event page pointer is invalid!</h1></p>"));
        }
        catch( std::runtime_error & e )
        {
            do_warn_usr(tr("Opening current pattern failure!"), e.what());
        }
    }
    else warn_usr(tr("The policy violation!"), tr("Choose an event at first!"));
}

void event_dlg::open_current_trigger()
{
    if ( stacked_event_pages_raw_ptr->currentIndex() != -1 )
    {
        auto current_page_raw_ptr = get_current_page();
        try
        {
            if ( current_page_raw_ptr != nullptr )
                if ( current_page_raw_ptr->get_trigger_path().size() )
                {
                    ImageViewer* img_viewer = new (std::nothrow) ImageViewer;
                    if ( img_viewer != nullptr )
                    {
                        img_viewer->open(current_page_raw_ptr->get_trigger_path());
                        img_viewer->show();
                    }
                    else do_warn_usr(tr("Interal error!"), tr("<p><h2>Failed to open the trigger picture : Failed to allocate memory for an image viewer!</h2></p>"));
                }
                else warn_usr(tr("The policy violation!"), tr("<p><h2>No trigger is set!</h2></p>"));
            else do_warn_usr(tr("Internal error!"), tr("<p><h1>The current event page pointer is invalid!</h1></p>"));
        }
        catch( std::runtime_error & e )
        {
            do_warn_usr(tr("Opening current trigger failure!"), e.what());
        }
    }
    else warn_usr(tr("The policy violation!"), tr("Choose an event at first!"));
}

bool event_dlg::event(QEvent *evt_raw_ptr)
{
    if ( evt_raw_ptr->type() == QEvent::StatusTip )
    {
        status_bar_raw_ptr->showMessage(static_cast<QStatusTipEvent*>(static_cast<void*>(evt_raw_ptr))->tip());
        return true;
    }
    return QDialog::event(evt_raw_ptr);
}

//void event_dlg::config_was_accepted()
//{
//}

} // namespace ised
