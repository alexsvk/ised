#ifndef ISED_STREAM_PARSER_H
#define ISED_STREAM_PARSER_H

/*!
  \class ised_stream_parser defines input and output of patterns in predefined format.

    The format defines the monitor resolution the pattern was defined with and the sequence of rectangles encapsulated into region.

  \author Alexander Syvak.
  \date July, 2013.
*/

#include <QDataStream> // INHERITS
#include <QRegion> // COMPOSITION
#include <QString> // COMPOSITION

namespace ised
{

class ised_stream_parser : public ::QDataStream
{
public:

    // TYPES

    /*!
     * \brief The ised_pattern_container struct defines a pattern with the monitor resolution width and height the pattern was made with and the correspondent region of the pattern.
     */
    struct ised_pattern_container
    {
        quint16 width;
        quint16 height;
        QRegion region;

        inline operator QRect() const
        {
            return QRect(QPoint(0, 0), QSize(width, height));
        }
    };

    // CTORS

    ised_stream_parser()
    {
    }
    ised_stream_parser(QString const & src_file_str_): src_file_str(src_file_str_)
    {

    }

    // OPERATORS

    /*!
     * \brief operator << Stores the 'ised_pattern' into predefined source file.
     * \param ised_pattern The pattern to be stored.
     * \return The parser stream.
     * \sa set_src_file().
     * \attention setVersion() should be called before using the operator.
     */
    ised_stream_parser & operator<<(ised_pattern_container const & ised_pattern);
    /*!
     * \brief operator >> Read a pattern out of the source file and stores it into the 'ised_pattern'.
     * \param ised_pattern The pattern container.
     * \return The parser stream.
     * \sa set_src_file().
     * \attention setVersion() should be called before using the operator.
     */
    ised_stream_parser const & operator>>(ised_pattern_container & ised_pattern);

    // SETTERS

    /*!
     * \brief set_src_file Defines the source file the stream parser should work with.
     * \param src_file_ The source file.
     * \sa operator <<(), operator >>().
     */
    inline void set_src_file(QString const & src_file_str_)
    {
        src_file_str = src_file_str_;
    }

    // GETTERS

    /*!
     * \brief get_src_file Points out the current source file the parser stream is working with.
     * \return The current source file.
     * \sa set_src_file().
     */
    inline QString get_src_file() const
    {
        return src_file_str;
    }

private:
    QString src_file_str;

}; // class ised_stream_parser

} // namespace ised



#endif // ISED_STREAM_PARSER_H
