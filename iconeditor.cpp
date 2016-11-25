#include <QtWidgets>

#include "iconeditor.h"

IconEditor::IconEditor(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    curColor = Qt::darkGray;
    zoom = 16;

    image = QImage(16, 16, QImage::Format_ARGB32);
    image.fill(qRgba(255, 255, 255, 255));
}

void IconEditor::setPenColor(const QColor &newColor)
{
    curColor = newColor;
}

void IconEditor::setZoomFactor(int newZoom)
{
    if (newZoom < 1)
        newZoom = 1;

    if (newZoom != zoom) {
        zoom = newZoom;
        update();
        updateGeometry();
    }
}

void IconEditor::setIconImage(const QImage &newImage)
{
    if (newImage != image) {
        image = newImage.convertToFormat(QImage::Format_ARGB32);
        update();
        updateGeometry();
    }
}

QSize IconEditor::sizeHint() const
{
    QSize size = zoom * image.size();
    if (zoom >= 3)
        size += QSize(1, 1);
    return size;
}

void IconEditor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setImagePixel(event->pos(), true);
    } else if (event->button() == Qt::RightButton) {
        if ((!onePixelPerColumn)&&(!onePixelPerRow)) setImagePixel(event->pos(), false);
    }
}

void IconEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        setImagePixel(event->pos(), true);
    } else if (event->buttons() & Qt::RightButton) {
        if ((!onePixelPerColumn)&&(!onePixelPerRow)) setImagePixel(event->pos(), false);
    }
}

void IconEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRect rect = pixelRect(i, j);
            if (!event->region().intersected(rect).isEmpty()) { // intersect(rect) not used in Qt5
                QColor color = QColor::fromRgba(image.pixel(i, j));
                if (color.alpha() < 255)
                    painter.fillRect(rect, Qt::white);
                painter.fillRect(rect, color);
            }
        }
    }

    if (zoom >= 3) {
        painter.setPen(palette().foreground().color());
        for (int i = 0; i <= image.width(); ++i) {
            painter.drawLine(zoom * i, 0,
                             zoom * i, zoom * image.height());
            if ((i%4)==0) painter.drawLine((zoom * i)+1, 0,
                             (zoom * i)+1, zoom * image.height());
        }
        for (int j = 0; j <= image.height(); ++j) {
            painter.drawLine(0, zoom * j,
                             zoom * image.width(), zoom * j);
            if ((j%4)==0) painter.drawLine(0, (zoom * j)+1,
                                       zoom * image.width(), (zoom * j)+1);
        }
    }

}

void IconEditor::setImagePixel(const QPoint &pos, bool opaque)
{
    int i = pos.x() / zoom;
    int j = pos.y() / zoom;

    if (image.rect().contains(i, j)) {

        if (opaque) {
            image.setPixel(i, j, penColor().rgba());
            if (onePixelPerColumn) {
                for (int x = 0; x < image.height(); x++) {
                    if (x!= j) image.setPixel(i, x, qRgba(255, 255, 255, 255));
                }
            }
            if (onePixelPerRow) {
                for (int x = 0; x < image.width(); x++) {
                    if (x!= i) image.setPixel(x, j, qRgba(255, 255, 255, 255));
                }
            }

        } else {
            image.setPixel(i, j, qRgba(255, 255, 255, 255));
        }
        if (onePixelPerColumn || onePixelPerRow) update();
        else update(pixelRect(i, j));
    }
    emit imageChanged();

}

QRect IconEditor::pixelRect(int i, int j) const
{
    if (zoom >= 3) {
        return QRect(zoom * i + 1, zoom * j + 1, zoom - 1, zoom - 1);
    } else {
        return QRect(zoom * i, zoom * j, zoom, zoom);
    }
}

void IconEditor::forceOnePixelPerRowColumn(bool _onePixelPerRow, bool _onePixelPerColumn)
{
    onePixelPerColumn = _onePixelPerColumn;
    onePixelPerRow = _onePixelPerRow;
    if (onePixelPerColumn) {
        for (int i = 0; i < image.width(); ++i) {
            for (int j = 0; j < image.height(); ++j) {
                    if ((i%image.height())==(j%image.height()))
                        image.setPixel(i, j, qRgba(127, 127, 127, 255));
                    else
                        image.setPixel(i, j, qRgba(255, 255, 255, 255));
            }
        }
    }

    if (onePixelPerRow) {
        for (int i = 0; i < image.width(); ++i) {
            for (int j = 0; j < image.height(); ++j) {
                    if ((i%image.width())==(j%image.width()))
                        image.setPixel(i, j, qRgba(127, 127, 127, 255));
                    else
                        image.setPixel(i, j, qRgba(255, 255, 255, 255));
            }
        }
    }

    if ( (!onePixelPerColumn)&&(!_onePixelPerRow) ) {
        for (int i = 0; i < image.width(); ++i) {
            for (int j = 0; j < image.height(); ++j) {
                    if ( (i+j)%2 == 0)
                        image.setPixel(i, j, qRgba(127, 127, 127, 255));
                    else
                        image.setPixel(i, j, qRgba(255, 255, 255, 255));
            }
        }
    }
}

