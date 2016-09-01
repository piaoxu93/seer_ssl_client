#include "field.h"
#include <QColor>
#include <QFont>
#include <QtMath>
#include <QtDebug>
#include <QPixmap>
#include <QElapsedTimer>
#include "singleparams.h"
#include "./follow/GlobalData.h"
Field *Field::single = nullptr;
namespace{
    const static QColor COLOR_BLUE(19,49,137);
    const static QColor COLOR_TRANSBLUE(19,49,137,30);
    const static QColor COLOR_YELLOW(241,231,36);
    const static QColor COLOR_TRANSYELLOW(241,231,36,30);
    const static QColor COLOR_PINK(255,63,149);
    const static QColor COLOR_GREEN(105,255,0);
    const static QColor COLOR_ORANGE(255,170,85);
    const static QColor COLOR_TRANSORANGE(255,170,85,30);
    const static QColor COLOR_DARKGREEN(Qt::darkGreen);
    const static QColor COLOR_TRANSPARENT(Qt::transparent);
}

Field* Field::instance(){
    if(nullptr == single){
        single = new Field();
    }
    return single;
}

static void initPainterPath(QPainterPath& painterPath);
static void addQuarterCirclePath(QPainterPath& painterPath,qreal x,qreal y,qreal radius,qreal angel);
Field::Field(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    ,image(QImage(QSize(
                      SingleParams::instance()->_("canvas.width"),
                      SingleParams::instance()->_("canvas.height"))
                  , QImage::Format_ARGB32))
    ,pixmap(QPixmap(QSize(
                        SingleParams::instance()->_("canvas.width"),
                        SingleParams::instance()->_("canvas.height")))){
    if(nullptr == single){
        single = this;
    }else{
        qDebug() << "Error declaration of not-only-one Field";
    }
    float totalHeight = SingleParams::instance()->_("canvas.height");
    float totalWidth = SingleParams::instance()->_("canvas.width");
    int invert = SingleParams::instance()->_("field.invert") ? -1 : 1;
    setImplicitWidth(totalWidth);
    setImplicitHeight(totalHeight);
    //imagePainter.begin(&image);
    imagePainter.begin(&pixmap);
    imagePainter.setWindow(-totalWidth/2,totalHeight/2,totalWidth,-totalHeight);
    imagePainter.scale(invert,invert);
    //image
    initPainterPath(painterPath);
    pixmap.fill(COLOR_DARKGREEN);
    imagePainter.strokePath(painterPath, QPen(Qt::white, 1));
}
void Field::paint(QPainter* painter){
    static QRect area(0,0,this->property("width").toReal(),this->property("height").toReal());
    //paintCar(false,0,0,0,90);
    painter->drawPixmap(area,pixmap);

}
void Field::draw(){
    static QRect area(0,0,this->property("width").toReal(),this->property("height").toReal());
    pixmap.fill(COLOR_DARKGREEN);
    imagePainter.strokePath(painterPath, QPen(Qt::white, 1));
    drawOneFrame(0);
//    for (int i=-1;i>-60;i-=2)
//        drawOneFrame(i,false);
    this->update(area);
}
void Field::drawOneFrame(int index,bool solid){
    static qreal posRatio = 0.1;
    static qreal angRatio = 1;//180/3.14159;
    auto& vision = GlobalData::Instance()->msg[index];
    for(int i=0;i<SendCarNum;i++){
        if(vision.RobotFound[BLUE][i]){
            paintCar(solid ? COLOR_BLUE : COLOR_TRANSBLUE
                             ,vision.RobotINDEX[BLUE][i]
                     ,vision.RobotPosX[BLUE][i]*posRatio,vision.RobotPosY[BLUE][i]*posRatio,vision.RobotRotation[BLUE][i]*angRatio);
        }
    }
    for(int i=0;i<SendCarNum;i++){
        if(vision.RobotFound[YELLOW][i]){
            paintCar(solid ? COLOR_YELLOW : COLOR_TRANSYELLOW
                             ,vision.RobotINDEX[YELLOW][i]
                     ,vision.RobotPosX[YELLOW][i]*posRatio,vision.RobotPosY[YELLOW][i]*posRatio,vision.RobotRotation[YELLOW][i]*angRatio);
        }
    }
    if (vision.BallFound){
        paintBall(solid ? COLOR_ORANGE : COLOR_TRANSORANGE, vision.Ballx*posRatio,vision.Bally*posRatio);
    }
}
void Field::paintCar(const QColor& color,quint8 num,qreal x,qreal y,qreal radian){
    static float diameter = SingleParams::instance()->_("car.diameter");
    static float faceWidth = SingleParams::instance()->_("car.faceWidth");
    qreal radius = diameter/2;
    qreal chordAngel = qRadiansToDegrees(qAcos(faceWidth/diameter));
    imagePainter.setBrush(QBrush(color));
    imagePainter.setPen(Qt::NoPen);
    imagePainter.drawChord(x-radius,y-radius,diameter,diameter,(90.0-chordAngel - radian)*16,(180.0+2*chordAngel)*16);
}
void Field::paintBall(const QColor& color,qreal x, qreal y){
    static float radius = float(SingleParams::instance()->_("ball.diameter"))/2;
    imagePainter.setBrush(QBrush(color));
    imagePainter.setPen(Qt::NoPen);
    imagePainter.drawEllipse(x-radius,y-radius,2*radius,2*radius);
}
static void initPainterPath(QPainterPath& painterPath){
    static int width                 = SingleParams::instance()->_("field.width");
    static int height                = SingleParams::instance()->_("field.height");
    static int goalWidth             = SingleParams::instance()->_("field.goalWidth");
    static int goalDepth             = SingleParams::instance()->_("field.goalDepth");
    static int penaltyRadius         = SingleParams::instance()->_("field.penaltyRadius");
    static int penaltyCenterLength   = SingleParams::instance()->_("field.penaltyCenterLength");
    static int centerCircleRadius    = SingleParams::instance()->_("field.centerCircleRadius");
    painterPath.addRect(-width/2,-height/2,width,height);
    painterPath.addRect(-width/2,goalWidth/2,-goalDepth,-goalWidth);
    painterPath.addRect(width/2,goalWidth/2,goalDepth,-goalWidth);
    painterPath.moveTo(-width/2,0);
    painterPath.lineTo(width/2,0);
    painterPath.moveTo(0,-height/2);
    painterPath.lineTo(0,height/2);
    painterPath.addEllipse(-centerCircleRadius,-centerCircleRadius,2*centerCircleRadius,2*centerCircleRadius);
    addQuarterCirclePath(painterPath,width/2,-penaltyCenterLength/2,penaltyRadius,90);
    addQuarterCirclePath(painterPath,width/2,penaltyCenterLength/2,penaltyRadius,180);
    addQuarterCirclePath(painterPath,-width/2,penaltyCenterLength/2,penaltyRadius,-90);
    addQuarterCirclePath(painterPath,-width/2,-penaltyCenterLength/2,penaltyRadius,0);
    painterPath.moveTo(width/2-penaltyRadius,penaltyCenterLength/2);
    painterPath.lineTo(width/2-penaltyRadius,-penaltyCenterLength/2);
    painterPath.moveTo(-(width/2-penaltyRadius),penaltyCenterLength/2);
    painterPath.lineTo(-(width/2-penaltyRadius),-penaltyCenterLength/2);
}
static void addQuarterCirclePath(QPainterPath& painterPath,qreal x,qreal y,qreal radius,qreal angel){
    painterPath.moveTo(x,y);
    painterPath.arcMoveTo(x-radius,y-radius,2*radius,2*radius,angel);
    painterPath.arcTo(x-radius,y-radius,2*radius,2*radius,angel,90);
}
