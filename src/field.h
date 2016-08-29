#ifndef FIELD_H
#define FIELD_H

#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QPainterPath>
#include <QImage>

class Field : public QQuickPaintedItem
{
public:
    static Field* instance();
    void paint(QPainter* painter) override;
    Field(QQuickItem *parent = 0);
    void draw();
//    void update(const QRect &rect);
signals:

public slots:

private:
    static Field *single;
    void drawOneFrame(int,bool solid = true);
    void paintCar(const QColor& color,quint8 num,qreal x,qreal y,qreal radian);
    void paintBall(const QColor& color,qreal x,qreal y);
    QImage image;
    QPixmap pixmap;
    QPainter imagePainter;
    QPainterPath painterPath;
};

#endif // FIELD_H
