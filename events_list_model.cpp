// SYSTEM
#include <string>
// PROJECT
#include <QFile> // config_parser
#include <QFileInfo> // QFileInfo::exists
#include <QStringList> // QRegExp::capturedTexts
#include <QFont> // USES
#include <boost/lexical_cast.hpp>
// LOCAL
#include "events_list_model.h" // IMPLEMENTS
#include "event.h" // USES, AGGREGATES
#include "simple_event.h" // USES, AGGREGATES
#include "complex_event.h" // USES, AGGREGATES
#include "xml_parser_adapter.h" // AGGREGATES
#include "event_serializing_visitor.hpp" // USES
#include "null_deleter.h" // USES
#include <QDebug>
namespace ised
{

//////////////////////////////////////////////////////////////////////////////////////////////////
/// events_list_model::config_parser
///

events_list_model::config_parser::config_parser() : pxml_parser_adapter(new(std::nothrow)xml_parser_adapter)
{
}

QString const events_list_model::config_parser::events_start_element = "events";
QString const events_list_model::config_parser::simple_event_start_element = "simple_event";
QString const events_list_model::config_parser::simple_event_start_element_shortcut = "se";
QString const events_list_model::config_parser::complex_event_start_element = "complex_event";
QString const events_list_model::config_parser::complex_event_start_element_shortcut = "ce";
QString const events_list_model::config_parser::interval_start_element = "interval";
QString const events_list_model::config_parser::trigger_start_element = "trigger";
QString const events_list_model::config_parser::pattern_start_element = "pattern";
QRegExp const events_list_model::config_parser::interval_regexp = QRegExp(QString("\\s*(\\d+)\\s*,\\s*(\\d+)\\s*"));
//QString const events_list_model::config_parser::simple_event_beginning = "simple_event";
//QString const events_list_model::config_parser::complex_event_beginning = "<complex_event";

events_list_model::events_container events_list_model::config_parser::parse(QString const & events_src_path)
{
    if ( pxml_parser_adapter )
    {
        QFile events_src_file(events_src_path);
        if (!events_src_file.open(QFile::ReadOnly | QFile::Text))
            throw std::runtime_error((QString("Failed to parse the events configuration file : ") + events_src_file.errorString()).toStdString());
        events.clear();
        pxml_parser_adapter->reset_reader_src_file(events_src_file);
        pxml_parser_adapter->read_token();
        while ( !pxml_parser_adapter->at_end() )
        {
            if ( pxml_parser_adapter->start_element() )
                if ( pxml_parser_adapter->equal_to(events_start_element) ) parse_events();
                else throw std::runtime_error("Failed to parse the events configuration file : Not an events file");
            else pxml_parser_adapter->read_token();
        }
        events_src_file.close();
        if  ( pxml_parser_adapter->has_error() )
            throw std::runtime_error((QString("Failed to parse the events source file : ") + pxml_parser_adapter->err_str()).toStdString());
        else if ( events_src_file.error() != QFile::NoError )
            throw std::runtime_error((QString("Failed to parse the events source file : ") + events_src_file.errorString()).toStdString());
    }
    else throw std::runtime_error("Failed to parse the events source file : Failed to allocate memory for the parser adapter");
    return events;
}

void events_list_model::config_parser::parse_events()
{
    pxml_parser_adapter->read_token();
    while ( !pxml_parser_adapter->at_end() )\
    {
        if ( pxml_parser_adapter->end_element() )
        {
            pxml_parser_adapter->read_token();
            break; // tolerant of several <events> elements
        }
        if ( pxml_parser_adapter->start_element() ) // either <simple_event> or <complex_event>, otherwise unknown element
        {
            if ( pxml_parser_adapter->equal_to(simple_event_start_element) ) events.append( parse_simple_event() ); // tolerant of non-full simple event specification
            else if ( pxml_parser_adapter->equal_to(complex_event_start_element) ) events.append( parse_complex_event() ); // tolerant of non-full complex event specification
            else pxml_parser_adapter->skip_current_element(); // tolerant of new elements
        }
        else pxml_parser_adapter->read_token(); // tolerant of non-element "rubbish"
    }
}

events_list_model::event_ptr events_list_model::config_parser::parse_simple_event()
{
    //if ( !simple_event_ptr ) throw std::runtime_error("Failed to parse a simple event : Failed to allocate dynamic memory for a simple event object");
    simple_event sevent;
    pxml_parser_adapter->read_token();
    while ( !pxml_parser_adapter->at_end() )
    {
        if ( pxml_parser_adapter->end_element() )
        {
            pxml_parser_adapter->read_token();
            break;
        }
        if ( pxml_parser_adapter->start_element() ) // <interval> or <trigger>, or <pattern> otherwise unknown element
        {

            if ( pxml_parser_adapter->equal_to(interval_start_element) ) sevent.interval() = parse_interval();
            else if ( pxml_parser_adapter->equal_to(trigger_start_element) )
            {
               // qDebug() << pxml_parser_adapter->start_element() << pxml_parser_adapter->name(); qDebug() << pxml_parser_adapter->read_element_text();
                sevent.trigger_path() = parse_trigger_path();
            }
            else if ( pxml_parser_adapter->equal_to(pattern_start_element) ) sevent.pattern_path() = parse_pattern_path();
            else pxml_parser_adapter->skip_current_element(); // tolerant of new elements
        }
        else pxml_parser_adapter->read_token(); // tolerant of non-element "rubbish"
    }
    //*simple_event_ptr = std::move(sevent);
    auto sep = event_ptr( new(std::nothrow)simple_event(sevent) );
    if ( !sep ) throw std::runtime_error("Failed to parse a simple event : Failed to allocate dynamic memory for a simple event object");
    return sep;
}

ised::event::event_interval events_list_model::config_parser::parse_interval()
{
//    auto const txt = pxml_parser_adapter->read_element_text();
//    qDebug() << interval_regexp << ' ' << txt;
//    auto r = interval_regexp.exactMatch(txt);
//    qDebug() << r;
    if ( !interval_regexp.exactMatch(pxml_parser_adapter->read_element_text()) )
        throw std::runtime_error(
                                    QString("Failed to parse interval at the line #%1 : Not correct value")
                                    .arg(pxml_parser_adapter->line_number())
                                    .toStdString()
                                );
    if ( pxml_parser_adapter->end_element() ) pxml_parser_adapter->read_token();
    return qMakePair<ised::event::event_interval_atom,ised::event::event_interval_atom>
           (
                (interval_regexp.capturedTexts().constBegin()+1)->toUInt(),
                (interval_regexp.capturedTexts().constBegin()+2)->toUInt()
           );
}

QString events_list_model::config_parser::parse_path(QString const & concrete_element)
{
    //qDebug() << pxml_parser_adapter->start_element() << pxml_parser_adapter->equal_to("trigger");
    //qDebug() << pxml_parser_adapter->read_element_text();

    //QString const p = pxml_parser_adapter->read_element_text();
    QFileInfo path = pxml_parser_adapter->read_element_text();
    //qDebug() << path.canonicalFilePath() << path.exists();
    //throw std::runtime_error("");
    if ( path.size() && !path.exists() )
        throw std::runtime_error(
                                     QString("Failed to parse the %1 path at the line #%2 : The path does not exist")
                                     .arg(concrete_element)
                                     .arg(pxml_parser_adapter->line_number())
                                     .toStdString()
                                );
    if ( pxml_parser_adapter->end_element() ) pxml_parser_adapter->read_token();
    return path.canonicalFilePath (); // N.B. !!! Returns the canonical path including the file name,
                                      // i.e. an absolute path without symbolic links or redundant "." or ".." elements.
                                      // If the file does not exist, canonicalFilePath() returns an empty string.
}

QString events_list_model::config_parser::parse_trigger_path()
{
    return parse_path("trigger");
}

QString events_list_model::config_parser::parse_pattern_path()
{
    return parse_path("pattern");
}

events_list_model::event_ptr events_list_model::config_parser::parse_complex_event()
{
    event_ptr complex_event_ptr(new(std::nothrow)(complex_event));
    if ( !complex_event_ptr ) throw std::runtime_error("Failed to parse a complex event : Failed to allocate dynamic memory for a complex event object");
    complex_event cevent;
    pxml_parser_adapter->read_token();
    while ( !pxml_parser_adapter->at_end() )
    {
        if ( pxml_parser_adapter->end_element() )
        {
            pxml_parser_adapter->read_token();
            break;
        }
        if ( pxml_parser_adapter->start_element() ) // <interval> or <trigger>, or <pattern> otherwise unknown element
            if ( pxml_parser_adapter->equal_to(interval_start_element) ) cevent.interval() = parse_interval();
            else if ( pxml_parser_adapter->equal_to(simple_event_start_element) )
                cevent.events().append  (
                                           *static_cast<simple_event const*>
                                            (
                                                static_cast<void const*>
                                                (
                                                    parse_simple_event().data()
                                                )
                                            )
                                        );
            else pxml_parser_adapter->skip_current_element(); // tolerant of new elements
        else pxml_parser_adapter->read_token(); // tolerant of non-element "rubbish"
    }
    *complex_event_ptr = std::move(cevent);
    return complex_event_ptr;
}

void events_list_model::config_parser::set_xml_writer_settings(bool enable_auto_formatting, int number_of_spaces_or_tab)
{
    pxml_parser_adapter->xml_auto_formatting(enable_auto_formatting);
    pxml_parser_adapter->xml_auto_formatting_indent(number_of_spaces_or_tab);
}

void events_list_model::config_parser::store(events_container const & events, QString const & events_dest_path)
{
    // Open the target file.
    QFile events_dest_file(events_dest_path);
    if (!events_dest_file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
        throw std::runtime_error(
                                    QString("Failed to store the events into the %1 configuration file : %2")
                                        .arg(events_dest_path)
                                        .arg(events_dest_file.errorString())
                                        .toStdString()
                                );
    // Set the new events configuration target file.
    pxml_parser_adapter->reset_writer_src_file(events_dest_file);

    // Declare the XML version.
    pxml_parser_adapter->store_doc_start();
    // Declare the events configuration header start element <events>.
    pxml_parser_adapter->store_element_beginning(events_start_element);

    // Store the events configuration.
    typedef event_serializing_visitor<config_parser> config_parser_based_visitor;
    if ( pxml_parser_adapter ) for( auto const & pevent : events ) pevent->apply_visitor( config_parser_based_visitor(config_parser_based_visitor::parser_ptr(this, ised::null_deleter())) );
    else throw std::runtime_error("Failed to store the events into the events source file : Failed to allocate memory for the parser adapter");

    // Store the </events> end element and put the new line character.
    pxml_parser_adapter->store_doc_end();

    // Close the target file.
    events_dest_file.close(); // flush()
}

void events_list_model::config_parser::store_element_beginning(QString const & element_beginning, QString const & element)
{
    if ( pxml_parser_adapter ) pxml_parser_adapter->store_element_beginning(element_beginning);
    else throw std::runtime_error(
                                    QString("Failed to store the %1 beggining into the events source file : Failed to allocate memory for the parser adapter")
                                     .arg(element)
                                     .toStdString()
                                 );
}

void events_list_model::config_parser::store_simple_event_beginning()
{
    store_element_beginning(simple_event_start_element, "simple event");
}

void events_list_model::config_parser::store_interval(ised::event::event_interval const & interval)
{
    if ( pxml_parser_adapter )
        pxml_parser_adapter->store_element_text(
                                                    interval_start_element,
                                                    QString("%1,%2")
                                                        .arg(interval.first)
                                                        .arg(interval.second)
                                                );
    else throw std::runtime_error("Failed to store the interval into the events source file : Failed to allocate memory for the parser adapter");
}

void events_list_model::config_parser::store_trigger_path(QString const & trigger_path)
{
    if ( pxml_parser_adapter ) pxml_parser_adapter->store_element_text( trigger_start_element, trigger_path );
    else throw std::runtime_error("Failed to store the trigger path into the events source file : Failed to allocate memory for the parser adapter");
}

void events_list_model::config_parser::store_pattern_path(QString const & pattern_path)
{
    if ( pxml_parser_adapter ) pxml_parser_adapter->store_element_text( pattern_start_element, pattern_path );
    else throw std::runtime_error("Failed to store the pattern path into the events source file : Failed to allocate memory for the parser adapter");
}

void events_list_model::config_parser::store_element_ending(QString const & element)
{
    if ( pxml_parser_adapter ) pxml_parser_adapter->store_last_element_ending();
    else throw std::runtime_error(
                                    QString("Failed to store the %1 ending into the events source file : Failed to allocate memory for the parser adapter")
                                        .arg(element)
                                        .toStdString()
                                 );
}

void events_list_model::config_parser::store_simple_event_ending()
{
    store_element_ending("simple event");
}

void events_list_model::config_parser::store_complex_event_beginning()
{
    store_element_beginning(complex_event_start_element, "complex event");
}

void events_list_model::config_parser::store_complex_event_ending()
{
    store_element_ending("complex event");
}





//////////////////////////////////////////////////////////////////////////////////////////////////
/// events_list_model
///

QString const events_list_model::header = "Events";

events_list_model::events_list_model(QObject *parent) :
    QAbstractListModel(parent)
{
}

int events_list_model::rowCount( QModelIndex const & parent ) const
{
    if ( parent.isValid() ) return 0; // a valid index has non-negative row and column and belongs to the model
    return events.count();
}

QVariant events_list_model::data( QModelIndex const & index, int role ) const
{
    if (
            index.row() >= 0 &&
            index.row() < events.size()
       )
        switch ( role )
        {
        case Qt::ToolTipRole: return (*(events.begin() + index.row()))->print_name();
        case Qt::FontRole: return QFont("Helvetica", -1, 600);
        case Qt::TextAlignmentRole: return static_cast<int>(Qt::AlignCenter | Qt::AlignVCenter);
        case Qt::DisplayRole: case Qt::EditRole: /*return "The first event";//*/return QString("%1").arg((*(events.begin() + index.row()))->get_id());
        }
    return QVariant();
}

Qt::ItemFlags events_list_model::flags( QModelIndex const & index ) const
{
    if ( !index.isValid() ) return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
    return QAbstractItemModel::flags(index); //|  Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

bool events_list_model::setData(QModelIndex const & index, QVariant const & value, int role)
{
    if (
            index.row() >= 0 &&
            index.row() < events.size() &&
            role == Qt::DisplayRole
       )
    {
        *(events.begin() + index.row()) = *static_cast<event_ptr const*>(value.constData());//value.value<event_ptr>() could also be used.
        emit dataChanged( index, index );
        return true;
    }
    return false;
}

qint16 events_list_model::get_last_id_of_event_type() const
{
    qint16 idx = -1;
    for ( auto const & evt : events ) if ( evt->get_id().contains(inserting_event_type == simple ? "S" : "C") ) idx = ::boost::lexical_cast<quint16>(evt->get_id().right(evt->get_id().size()-1).toStdString());
    return idx;
}

bool events_list_model::insertRows( int row, int count, QModelIndex const & parent )
{
    if ( count < 1 || row < 0 || row > rowCount(parent) ) return false;
    auto last_id_of_evt_type = get_last_id_of_event_type() + 1;
    beginInsertRows( QModelIndex(), row, row + count - 1 );
    event_ptr event_p;
    for (int r = 0; r < count; ++r)
    {
        auto const id_str = (inserting_event_type == simple ? QString("S") : QString("C")) + QString(::boost::lexical_cast<std::string>(last_id_of_evt_type++).c_str());
        if ( inserting_event_type == event_type::simple ) event_p = event_ptr( new(std::nothrow)simple_event( id_str ) );
        else if ( inserting_event_type == event_type::complex ) event_p = event_ptr( new(std::nothrow)complex_event( id_str ) );
        if ( !event_p )
        {
            endInsertRows();
            throw std::runtime_error("Failed to insert rows : Failed to allocate memory for the event pointer");
        }
        events.insert( events.begin() + row + r, event_p );
    }
    endInsertRows();
    return true;
}

void events_list_model::insert_rows(int row, int count, event_type const & event_type_)
{
    inserting_event_type = event_type_;
    insertRows(row, count);
}

bool ised::events_list_model::removeRows( int row, int count, QModelIndex const & parent )
{
    if ( count <= 0 || row < 0 || row + count > rowCount(parent) ) return false;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    auto start_it = events.begin() + row;
    auto last_it = events.end();
    if ( std::distance(start_it, events.end()) > std::distance(start_it, start_it + count) ) last_it = start_it + count;
    events.erase( start_it, last_it );
    endRemoveRows();
    return true;
}

void events_list_model::set_events_src_file(QString const & events_src_file_)
{
    events_src_file = events_src_file_;
}

void events_list_model::parse(QString const & events_src_file_)
{
    set_events_src_file(events_src_file_);
    events = psr.parse(events_src_file);
    psr.empty_buffer();
}

void events_list_model::reset_events()//QString const & events_src_file_)
{
    emit beginResetModel();
    //set_events_src_file(events_src_file_);
//    try
//    {
//        //quint8 idx = 0U;
//        //for ( auto & a : events ) a->id() = a->type() + QString::number(idx);//a->print();
//    }
//    catch ( std::runtime_error & )
//    {
//        emit endResetModel();
//        throw;
//    }
    emit endResetModel();
}

QVariant events_list_model::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0 ) return tr(header.toUtf8().data());
    return QVariant();
}

} // namespace ised
