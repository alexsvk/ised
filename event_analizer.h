#ifndef EVENT_ANALIZER_H
#define EVENT_ANALIZER_H

#include <QObject> // INHERITS
#include <QString> // COMPOSITION

namespace ised
{

class event_analizer : public QObject
{
    Q_OBJECT
public:
    typedef quint32 frequency;
    explicit event_analizer(frequency freq_, QString const & dest_path_): freq(freq_), dest_path(dest_path_)
    {
    }
    static QString const trigger_format;
protected:
    frequency freq;
    QString dest_path;

}; // class event_analizer

} // namespace event_analizer


#endif // EVENT_ANALIZER_H
