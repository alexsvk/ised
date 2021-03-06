#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QPrinter>
#include <QImage>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

class ImageViewer : public QMainWindow
{
 Q_OBJECT

public:
 ImageViewer();
 void open(QString const & image_path);
 void open(QImage const & image);
private slots:
 void open();
 void print();
 void zoomIn();
 void zoomOut();
 void normalSize();
 void fitToWindow();
 void about();

private:
 void createActions();
 void createMenus();
 void updateActions();
 void scaleImage(double factor);
 void adjustScrollBar(QScrollBar *scrollBar, double factor);

 QLabel *imageLabel;
 QScrollArea *scrollArea;
 double scaleFactor;

#ifndef QT_NO_PRINTER
 QPrinter printer;
#endif

 QAction *openAct;
 QAction *printAct;
 QAction *exitAct;
 QAction *zoomInAct;
 QAction *zoomOutAct;
 QAction *normalSizeAct;
 QAction *fitToWindowAct;
 QAction *aboutAct;
 QAction *aboutQtAct;

 QMenu *fileMenu;
 QMenu *viewMenu;
 QMenu *helpMenu;
};

#endif
