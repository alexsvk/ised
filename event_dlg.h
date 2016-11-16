#ifndef EVENT_DLG_H
#define EVENT_DLG_H

/*!
  \brief The event dialog's responsibility is to define events in either loaded or newly created file with predefined format.\
 The configuration is used once the program is started. Additionally an user may keep the loaded file up-to-date once the configuration is changed.
  \author Alexander Syvak.
  \date July, 2013.
*/

// PROJECT
#include <QDialog>
#include <QString>

// LOCAL
#include "event_pages_updating_visitor.h" // FRIEND OF
#include "events_list_model.h" // AGGREGATES

class QHBoxLayout;
class QListView;
class QStackedWidget;
class QMenu;
class QToolBar;
class QMenuBar;
class QVBoxLayout;
class QModelIndex;
class QStatusTipEvent;
class QEvent;
class QStatusBar;

namespace ised
{

class simple_event_page_widget; // USES

//class events_list_model; // AGGREGATES

class pattern_qualifier; // AGGREGATES

class event_dlg : public QDialog
{
    Q_OBJECT
    
public:

    // TYPES

    typedef events_list_model::events_container events_container;
    typedef events_list_model::event_ptr event_ptr;

    // CTOR && DTOR

    explicit event_dlg(QWidget *parent = 0);
    ~event_dlg();

    // OPERATIONS

    /*!
     * \brief set_config_file should be called to set a configuration file name before the dialog is shown.
     * \param config_file_name_ The configuration file name.
     * \attention If no configuration file was loaded, the name must be an empty string.
     */
    inline void set_config_file(QString const & config_file_name_)
    {
        config_file_name = config_file_name_;
    }
    events_container get_events() const;
    inline QString get_config_path() const
    {
        return config_file_name;
    }
    /*!
     * \brief parse_config_file Parse the preset configuration file so that each event is ready to be identified in GUI.
     */
    void parse_config_file();
    void reset_stacked_event_pages();
    void update_view();
    bool validate_events();
    //void showEvent(QShowEvent *) override;

signals:
    void config_was_accepted();
public:
    // EVENTS

    bool event(QEvent * evt_raw_ptr);

    // SLOTS

private slots:
    void add_simple_event();
    void add_complex_event();
    void del_event();
    void save_configuration();
    void accept_configuration();
    void cancel_configuration();
    void load_trigger();
    void show_pattern_context_menu();
    void load_pattern();
    void create_pattern();
    void select_stacked_page(QModelIndex const & idx);
    void open_current_trigger();
    void open_current_pattern();
    void on_finish_pattern_qualifier(int result);
private:
    QRect primary_screen_geometry() const;

    void create_actions();

    void create_action(QAction** action_ptr, QString const & tip, QString const & icon_path, QString const & shortcut, QString const & slot, QString const & status_tip);

    void create_toolbar();

    void lay_out();

    void create_central_layout();

    void append_simple_event_page(simple_event const & sevt);

    void append_complex_event_page(complex_event const & cevt);

    QWidget* make_simple_event_page(simple_event const & sevt = simple_event());

    QWidget* make_complex_event_page(complex_event const & cevt = complex_event());

    void warn_usr(QString const & title, QString const & text, QString const & warning = "");
    void do_warn_usr(QString const & title, QString const & text, QString const & warning = "");

    void qualify_pattern(QString const & pattern_path = "");

    void receive_config_file_name();

    simple_event_page_widget* get_current_page() const;
    simple_event_page_widget* get_page_at(int const & idx) const;

    QVBoxLayout* main_layout_raw_ptr;
    QHBoxLayout* central_layout_raw_ptr;
    QListView* events_listview_raw_ptr;
    events_list_model* events_list_model_raw_ptr;
    QStackedWidget* stacked_event_pages_raw_ptr;
    QHBoxLayout* toolbars_layout_raw_ptr;
    QToolBar *edit_config_toolbar_raw_ptr,
             *manage_config_toolbar_raw_ptr;
    QAction* add_simple_event_action_raw_ptr,
             *add_complex_event_action_raw_ptr,
             *del_event_action_raw_ptr,
             *save_configuration_action_raw_ptr,
             *accept_configuration_action_raw_ptr,
             *cancel_configuration_action_raw_ptr,
             *load_trigger_action_raw_ptr,
             *load_pattern_action_raw_ptr,
             *load_or_create_pattern_action_raw_ptr,
             *create_pattern_action_raw_ptr,
             *open_current_trigger_action_raw_ptr,
             *open_current_pattern_action_raw_ptr;
    static QString const add_simple_event_icon_path,
                         add_complex_event_icon_path,
                         del_event_icon_path,
                         save_configuration_icon_path,
                         accept_configuration_icon_path,
                         cancel_configuration_icon_path,
                         event_dlg_icon_path,
                         load_trigger_icon_path,
                         load_or_create_trigger_icon_path,
                         load_pattern_icon_path,
                         create_pattern_icon_path,
                         open_current_pattern_icon,
                         open_current_trigger_icon;
    int const minimum_warn_msg_hspacer_width;
    QString config_file_name;
    pattern_qualifier* ptrn_qualifier_wdg;
    QStatusBar* status_bar_raw_ptr;
    // FRIENDS

    friend class event_pages_updating_visitor;

}; // event_dlg

} // namespace ised

#endif // EVENT_DLG_H
