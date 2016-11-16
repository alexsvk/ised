// PROJECT
#include <QFile> // USES
// LOCAL
#include "ised_stream_parser.h" // IMPLEMENTS

namespace ised
{

    // OPERATORS

    ised_stream_parser & ised_stream_parser::operator<<(ised_pattern_container const & ised_pattern)
    {
        QFile src_file(src_file_str);
        if ( src_file.open(QFile::WriteOnly) == false ) return *this;
        setDevice(&src_file); // set the I/O device
        *this << ised_pattern.width;
        *this << ised_pattern.height;
        *this << ised_pattern.region;
        setDevice(0); // reset the I/O device
        return *this;
    }

    ised_stream_parser const & ised_stream_parser::operator>>(ised_pattern_container & ised_pattern)
    {
        QFile src_file(src_file_str);
        if ( src_file.open(QFile::ReadOnly) == false ) return *this;
        setDevice(&src_file); // set the I/O device
        QDataStream::operator>>(ised_pattern.width);
        QDataStream::operator>>(ised_pattern.height);
        *this >> ised_pattern.region;
        setDevice(0); // reset the I/O device
        return *this;
    }

} // namespace ised
