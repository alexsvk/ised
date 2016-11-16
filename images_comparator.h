#ifndef IMAGES_COMPARATOR_H
#define IMAGES_COMPARATOR_H
#include "imageviewer.h"
#include <QRegion> // USES
#include <QImage> // USES

namespace ised
{

    class images_comparator
    {
    public:
        static bool cmp(QImage const && lhs_img, QImage const && rhs_img, QRegion const && region)
        {
            //lhs_img.save("l.png"), rhs_img.save("r.png");
            qDebug() << "Region rectangles count => " << region.rectCount();
            if ( region.rectCount() == 0 ) return false;
            for ( auto const & rect : region.rects() )
            {
                qDebug() << "Compared rectangles => " <<
                            "x : " << rect.x() <<
                            "y : " << rect.y() <<\
                            "bottom right x : " << rect.bottomRight().x() <<
                            "bottom right y : " << rect.bottomRight().y();
                if ( lhs_img.copy(rect) != rhs_img.copy(rect) ) return false;
                qDebug() << "Matched rectangles => " << rect.width() << rect.height();
            }
            return true;
        }
    };

} // namespace ised

#endif // IMAGES_COMPARATOR_H
