#ifndef LOGGER_H
#define LOGGER_H

#include <QSemaphore>
#include <QMutex>
#include <QFile>
#include <QDataStream>

namespace ised
{

    class ReadWriteMutex
    {
    public:
        ReadWriteMutex(int maxReaders = 32): maxReaders_(maxReaders), semaphore(maxReaders)
        {
        }
        inline void lockRead()
        {
            semaphore.acquire();
        }
        inline void unlockRead()
        {
            semaphore.release();
        }
        void lockWrite()
        {
            QMutexLocker locker(&mutex);
            for (int i = 0; i < maxReaders(); ++i) semaphore.acquire();
        }
        inline void unlockWrite()
        {
            semaphore.release(maxReaders_);
        }
        inline int maxReaders() const
        {
            return maxReaders_;
        }
    private:
        QSemaphore semaphore;
        QMutex mutex;
        int maxReaders_;
    };

    class logger
    {
    public:
        static void log(QString const & data, QString const & log_file_str)
        {
            ReadWriteMutex mutex;
            mutex.lockWrite();
            QFile log_file(log_file_str);
            if ( !log_file.open(QFile::Append | QFile::Text) )
                throw std::runtime_error( QString("Failed to log : %1").arg(log_file.errorString()).toStdString() );
            log_file.write( data.toStdString().c_str() );
            mutex.unlockWrite();
            log_file.close();
        }
    };
}

#endif // LOGGER_H
