#include "xml_parser_adapter.h" // IMPLEMENTS

namespace ised
{

xml_parser_adapter::xml_parser_adapter()
{
}

// OPERATIONS

void xml_parser_adapter::reset_reader_src_file(QFile & target_file_)
{
    xml_reader.setDevice(&target_file_);
}

void xml_parser_adapter::reset_writer_src_file(QFile & target_file_)
{
    xml_writer.setDevice(&target_file_);
}

void xml_parser_adapter::read_token()
{
    xml_reader.readNext(); // Reads the next token and returns its type.
                           // N.B. !!! With one exception, once an error() is reported by readNext(),
                           // further reading of the XML stream is not possible. Then atEnd() returns true,
                           // hasError() returns true, and this function returns QXmlStreamReader::Invalid.
}

QString xml_parser_adapter::read_element_text()
{
    return xml_reader.readElementText(); // N.B. !!! Calling this function is equivalent to calling readElementText(ErrorOnUnexpectedElement).
}

void xml_parser_adapter::skip_current_element()
{
    xml_reader.skipCurrentElement(); // Reads until the end of the current element, skipping any child nodes.
                                     // N.B. !!! The current element is the element matching the most recently parsed start element of which a matching end element has not yet been reached.
                                     // When the parser has reached the end element, the current element becomes the parent element.
}

void xml_parser_adapter::store_element_beginning(QString const & name)
{
    xml_writer.writeStartElement(name); // Writes a start element with name, prefixed for the specified namespaceUri.
                                        // If the namespace has not been declared yet, QXmlStreamWriter will generate a namespace
                                        // declaration for it. Subsequent calls to writeAttribute() will add attributes to this element.
}

void xml_parser_adapter::store_element_text(QString const & element_name, QString const & character_seq)
{
    xml_writer.writeTextElement(element_name, character_seq); // Writes a text element with name, prefixed for the specified namespaceUri,
                                                              // and text. If the namespace has not been declared, QXmlStreamWriter will
                                                              // generate a namespace declaration for it. It is a convenience function.
}

void xml_parser_adapter::store_last_element_ending()
{
    xml_writer.writeEndElement(); // Closes the previous start element.
}

void xml_parser_adapter::store_doc_start()
{
    xml_writer.writeStartDocument();
}

void xml_parser_adapter::store_doc_end()
{
    xml_writer.writeEndDocument();
}

// SETTERS

void xml_parser_adapter::xml_auto_formatting(bool enabled)
{
    xml_writer.setAutoFormatting(enabled);
}

void xml_parser_adapter::xml_auto_formatting_indent(int const & spaces_or_tabs)
{
    xml_writer.setAutoFormattingIndent(spaces_or_tabs);
}

// GETTERS

bool xml_parser_adapter::is_xml_auto_formatting_set() const
{
    return xml_writer.autoFormatting();
}

bool xml_parser_adapter::is_auto_formatting_indent_set() const
{
    return xml_writer.autoFormattingIndent();
}

QString xml_parser_adapter::err_str() const
{
    return xml_reader.errorString();
}

qint64 xml_parser_adapter::line_number() const
{
    return xml_reader.lineNumber();
}

// QUERIES

bool xml_parser_adapter::at_end() const
{
    return xml_reader.atEnd(); // N.B. !!! Returns true if the reader has read until the end of the XML document,
                               // or if an error() has occurred and reading has been aborted. Otherwise, it returns false.
}

bool xml_parser_adapter::start_element() const
{
    return xml_reader.isStartElement(); // N.B. !!! Returns true if tokenType() equals StartElement; otherwise returns false.
}

bool xml_parser_adapter::equal_to(QString const & name) const
{
    // N.B. !!! Returns the local name of a StartElement, EndElement, or an EntityReference.
    return xml_reader.name().compare(name, Qt::CaseInsensitive) == 0;
}

bool xml_parser_adapter::has_error() const
{
    return xml_reader.error() != QXmlStreamReader::NoError; // Returns the type of the current error.
}

bool xml_parser_adapter::end_element() const
{
    return xml_reader.isEndElement(); // N.B. !!! Returns true if tokenType() equals EndElement; otherwise returns false.
}

QString xml_parser_adapter::name() const
{
    return xml_reader.name().toString();
}

} // namespace ised
