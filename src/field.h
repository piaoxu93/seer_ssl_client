#ifndef FIELD_H
#define FIELD_H

#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QPainterPath>
#include <QImage>
#include "./follow/message.h"

class Field : public QQuickPaintedItem
{
public:
    static Field* instance();
    void paint(QPainter* painter) override;
    Field(QQuickItem *parent = 0);
    void draw(bool robot = true,bool ball = false,bool style = true);
    void changeMode(bool);
signals:

public slots:

private:
    void initField();
    CAMERAMODE cameraMode;
    static Field *single;
    void drawOneFrame(int,bool ball,bool solid = true);
    void drawPoint(int index,bool ball,bool solid);
    void paintCar(const QColor& color,quint8 num,qreal x,qreal y,qreal radian);
    void paintBall(const QColor& color,qreal x,qreal y);
    void initPainterPath();
    //QImage *image;
    QPixmap *pixmap;
    QPainter imagePainter;
    QPainterPath painterPath;
    int width;
    int height;
    int goalWidth;
    int goalDepth;
    int penaltyRadius;
    int penaltyCenterLength;
    int centerCircleRadius;
    float totalHeight;
    float totalWidth;
    int invert;
    QRect area;
};

#endif // FIELD_H
