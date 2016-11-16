//// PROJECT
//#include <QApplication>
//#include <QMessageBox>
//#include <QDir>
//#include <QDebug>
//// LOCAL
//#include "event_dlg.h"
//#include "event.h"

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    ised::event_dlg w;
//    try
//    {
//        qDebug() << QDir::currentPath();
//        w.set_config_file("events.xml");
//        w.parse_config_file();
//        w.validate_events(); // N.B. !!! Each event trigger and pattern paths may be deleted.
//        w.reset_stacked_event_pages();
//        w.update_view();
//    }
//    catch ( std::runtime_error & e )
//    {
//        QMessageBox msgBox;
//        msgBox.setText(e.what());
//        msgBox.exec();
//        a.quit();
//    }
//    w.show();
//    return a.exec();
//}
