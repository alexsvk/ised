#ifndef XML_PARSER_ADAPTER_H
#define XML_PARSER_ADAPTER_H

/*!
  \brief The xml_parser_adapter defines the events configuration adapter for the events list model to be independent of an XML parser API.\
  The adapter is using QXmlStreamReader internally because of its speed and simplicity (compared to the DOM parser).
  \author Alexander Syvak.
  \date July, 2013.
*/

#include <QFile> // reset_src_file()
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace ised
{

class xml_parser_adapter
{
public:
    // CTOR

    xml_parser_adapter();

    // OPERATIONS

    /*!
     * \brief reset_reader_src_file Sets the current XML reader device.
     * \param target_file_ The new events configuration source file.
     */
    void reset_reader_src_file(QFile & target_file_);
    /*!
     * \brief reset_reader_src_file Sets the current XML writer device.
     * \param target_file_ The new events configuration source file.
     */
    void reset_writer_src_file(QFile & target_file_);
    /*!
     * \brief read_token Reads the next token.
     */
    void read_token();
    /*!
     * \brief read_element_text Reads a "castrated" current element text. The current element is the element matching the most recently parsed start element.
     * \sa skip_current_element().
     * \return The current element text.
     */
    QString read_element_text();
    /*!
     * \brief skip_current_element Skips the current element until the next token after the current element end element is reached.
     * \sa read_token().
     */
    void skip_current_element();
    /*!
     * \brief store_element_beginning Stores an element start element into the events configuration file.
     * \param name // An element start element name.
     */
    void store_element_beginning(QString const & name);
    /*!
     * \brief store_element_text Stores an element with the start and end correspondent elements inserting text between them.
     * \param element_name The element name.
     * \param character_seq The text to inserted.
     */
    void store_element_text(QString const & element_name, QString const & character_seq);
    /*!
     * \brief store_last_element_ending Stores the end element matching the most recently stored start element ( stack-like functionality ).
     */
    void store_last_element_ending();
    /*!
     * \brief store_doc_start Writes a document start with the XML version number version.
     */
    void store_doc_start();
    /*!
     * \brief store_doc_end Closes all remaining open start elements and writes a newline.
     */
    void store_doc_end();

    // SETTERS

    /*!
     * \brief set_xml_auto_formatting Sets the auto-formatting property. This property controls whether or not the stream writer automatically formats \
     * the generated XML data. If enabled, the writer automatically adds line-breaks and indentation to empty sections \
     * between elements (ignorable whitespace). The main purpose of auto-formatting is to split the data into several lines,\
     * and to increase readability for a human reader. The indentation depth can be controlled through the auto_formatting_indent property.
     * \param enabled The auto-formatting flag.
     * \sa auto_formatting_indent(bool enabled).
     */
    void xml_auto_formatting(bool enabled = true);
    /*!
     * \brief auto_formatting_indent Sets the auto-formatting indent property.\
     * This property holds the number of spaces or tabs used for indentation when auto-formatting is enabled.\
     * Positive numbers indicate spaces, negative numbers tabs.
     * \param spaces_or_tabs The number of spaces or tabs.
     * \sa xml_auto_formatting(bool enabled).
     */
    void xml_auto_formatting_indent(int const & spaces_or_tabs);

    // GETTERS

    /*!
     * \brief is_xml_auto_formatting_set Points out whether the XML auto-formatting is enabled.
     * \return true if the auto-formatting is turned on; false otherwise.
     */
    bool is_xml_auto_formatting_set() const;
    /*!
     * \brief auto_formatting_indent Points out whether the XML auto-formatting indent is set.
     * \return true if the auto-formatting indent is set; false otherwise.
     */
    bool is_auto_formatting_indent_set() const;

    /*!
     * \brief err_str Points out the internal reader error represented as a string.
     * \return The error converted to a string.
     */
    QString err_str() const;

    /*!
     * \brief line_number Points out the current line number the reader stopped at.
     * \return The XML reader current line number.
     */
    qint64 line_number() const;

    // QUERIES

    /*!
     * \brief at_end Points out whether the document end was reached.
     * \return true if the end of the document was reached; false otherwise.
     */
    bool at_end() const;
    /*!
     * \brief start_element Points out whether an element start was reached.
     * \sa end_element().
     * \return true if an element start was reached; false otherwise.
     */
    bool start_element() const;
    /*!
     * \brief equal_to Compares the current element name with the passed parameter 'name'.
     * \param name The name to be compared.
     * \attention The comparison is case-insensitive.
     * \return true if the current element name matches the expected one; false otherwise.
     */
    bool equal_to(QString const & name) const;
    /*!
     * \brief has_error Points out whether an internal XML reader error was met.
     * \return true if an error occurred; false otherwise.
     */
    bool has_error() const;
    /*!
     * \brief end_element Points out whether an element end was reached.
     * \sa start_element().
     * \return true if an element end was reached; false otherwise.
     */
    bool end_element() const;

    QString name() const;

private:

    // FIELDS

    QString target_file;
    QXmlStreamReader xml_reader;
    QXmlStreamWriter xml_writer;

}; // class xml_parser_adapter

} // namespace ised

#endif // XML_PARSER_ADAPTER_H
