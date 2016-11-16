#ifndef EVENTS_LIST_MODEL_H
#define EVENTS_LIST_MODEL_H

/*!
  \brief The events_list_model class defines the model for the read-only list view of events.\
  It includes an XML implementation independent parser to handle its data source.
  \author Alexander Syvak.
  \date July, 2013.
*/

// PROJECT
#include <QAbstractListModel>
#include <QVariant>
#include <QLinkedList> // events_container
#include <QSharedPointer>
#include <QRegExp> // events configuration format validation
// LOCAL
#include "xml_parser_adapter.h" // AGGREGATES
#include "event.h" // AGGREGATES, USES

namespace ised
{

class pxml_parser_adapter;

class events_list_model : public QAbstractListModel
{
    Q_OBJECT
public:

    // TYPES
    enum event_type {simple, complex};
    typedef QSharedPointer<ised::event> event_ptr;
    typedef QLinkedList<event_ptr> events_container;

    // CTOR

    explicit events_list_model(QObject *parent = 0);
    /*!
     * \brief set_events_src_file Sets the name of the events source file.
     * \param events_src_file_
     */

    // OPERATIONS

    void set_events_src_file(QString const & events_src_file_);
    /*!
     * \brief reset_events Updates the correspondent view.
     */
    void reset_events();
    /*!
     * \brief parse Reads the provided file data and if found valid populates the events list.
     * \attention The parser buffer is automatically
     * \param events_src_file_ A file with specified events.
     */
    void parse(QString const & events_src_file_);
    /*!
     * \brief store_events Serializes the events.
     * \param dest_file_str The destination file.
     */
    inline void store_events(QString const & dest_file_str)
    {
        try
        {
            psr.store(events, dest_file_str);
        }
        catch( std::runtime_error & )
        {
            throw;
        }
    }
    /*!
     * \brief events Points out the current events.
     * \return Events.
     */
    inline events_container & get_events()
    {
        return events;
    }

    /*!
     * \brief rowCount One of the requirements for the read-only access to the QListView.
     * \param parent Not used in the implementation.
     * \return The number of events.
     */
    int rowCount ( QModelIndex const & parent = QModelIndex() ) const override;
    /*!
     * \brief data One of the requirements for the read-only access to the QListView.
     * \param index The model index encapsulating row id, column id and a pointer to the model.
     * \param role One of the model data types (e.g. Qt::Editable, Qt::Display).
     * \return The data stored under the given role for the item referred to by the index.
     */
    QVariant data ( QModelIndex const & index, int role = Qt::DisplayRole ) const override;
    /*!
     * \brief flags Defines all settings for each event specified by the model.
     * \param index An event ID.
     * \return The item flags for the given index.
     */
    Qt::ItemFlags flags ( QModelIndex const & index ) const override;
    /*!
     * \brief setData Sets the 'role' data for the item at 'index' to value.
     * \param index
     * \param value
     * \param role
     * \return true if successful; otherwise returns false.
     */
    bool setData(QModelIndex const & index, QVariant const & value, int role) override;
    /*!
     * \brief insertRows Inserts 'count' new rows into the model before the given 'row'.
     * \param row The row ID starting insertion point.
     * \param count The number of rows to be inserted.
     * \param parent Not used in the model.
     * \attention If row is 0, the rows are prepended to any existing rows in the parent.\
     *If row is rowCount(), the rows are appended to any existing rows in the parent.
     * \return true if the rows were successfully inserted; otherwise returns false.
     */
    bool insertRows ( int row, int count, QModelIndex const & parent = QModelIndex() ) override;
    /*!
     * \brief removeRows Removes 'count' rows starting with the given 'row'.
     * \param row The row ID starting removal point.
     * \param count The number of rows to be removed.
     * \param parent Not used in the model.
     * \return true if the rows were successfully removed; otherwise returns false.
     */
    bool removeRows ( int row, int count, QModelIndex const & parent = QModelIndex() ) override;
    /*!
     * \brief headerData Returns the data for the given 'role' and 'section' in the 'header' with the specified orientation.
     * \param section
     * \param orientation
     * \param role
     * \return -//-
     */
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;

    /*!
     * \brief insert_rows Inserts 'count' new rows with an event of type 'event_type_' into the model before the given 'row'.
     * \param row
     * \param count
     * \param event_type_
     * \attention Use only the method when inserting rows !!!
     */
    void insert_rows(int row, int count, event_type const & event_type_);
    qint16 get_last_id_of_event_type() const;
signals:
    
public slots:

private:

    // TYPES

    /*!
     * \brief The config_parser class is a recursive-descent parser of events configuration XML-like formatted file.
     */
    class config_parser
    {
    public:
        config_parser();
        events_container parse(QString const & events_src_file);
        void store(events_container const & events, QString const & events_dest_file);
        void store_simple_event_beginning();
        void store_interval(ised::event::event_interval const & interval);
        void store_trigger_path(QString const & trigger_path);
        void store_pattern_path(QString const & pattern_path);
        void store_simple_event_ending();
        void store_complex_event_beginning();
        void store_complex_event_ending();
        void set_xml_writer_settings(bool enable_auto_formatting = true, int number_of_spaces_or_tab = -1); // one '\t' character
        inline void empty_buffer()
        {
            events.clear();
        }

    private:
        void store_element_ending(QString const & element);
        void store_element_beginning(QString const & element_beginning, QString const & element);
        event_ptr parse_simple_event();
        void parse_events();
        ised::event::event_interval parse_interval();
        QString parse_trigger_path();
        QString parse_pattern_path();
        QString parse_path(QString const & concrete_element);
        event_ptr parse_complex_event();
        typedef QSharedPointer<xml_parser_adapter> xml_parser_adapter_ptr;
        xml_parser_adapter_ptr pxml_parser_adapter;
        static QString const    events_start_element,
                                simple_event_start_element,
                                simple_event_start_element_shortcut,
                                complex_event_start_element,
                                complex_event_start_element_shortcut,
                                interval_start_element,
                                trigger_start_element,
                                pattern_start_element;
        static QRegExp const interval_regexp;
        events_container events;
    };

    // FIELDS

    events_container events;
    config_parser psr;
    QString events_src_file;
    static QString const header;
    event_type inserting_event_type;
};

} // namespace ised

Q_DECLARE_METATYPE(ised::events_list_model::event_ptr)

#endif // EVENTS_LIST_MODEL_H
