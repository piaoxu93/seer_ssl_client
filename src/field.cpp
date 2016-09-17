#include "field.h"
#include <QColor>
#include <QFont>
#include <QtMath>
#include <QtDebug>
#include <QPixmap>
#include <QElapsedTimer>
#include "singleparams.h"
#include "./follow/GlobalData.h"
#include "visionmodule.h"
Field *Field::single = nullptr;
namespace{
    const static QColor COLOR_BLUE(19,49,137);
    const static QColor COLOR_TRANSBLUE(19,49,137,25);
    const static QColor COLOR_YELLOW(241,231,36);
    const static QColor COLOR_TRANSYELLOW(241,231,36,25);
    const static QColor COLOR_PINK(255,63,149);
    const static QColor COLOR_GREEN(105,255,0);
    const static QColor COLOR_ORANGE(255,170,85);
    const static QColor COLOR_TRANSORANGE(255,170,85,25);
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
    , cameraMode(FourCamera)
    , pixmap(nullptr)
    , counter(0)
    , pen(Qt::white,1){
    if(nullptr == single){
        single = this;
    }else{
        qDebug() << "Error declaration of not-only-one Field";
    }
    connect(VisionModule::instance(),SIGNAL(needDraw(bool,bool,bool)),this,SLOT(draw(bool,bool,bool)));
    changeMode(cameraMode);
}
void Field::initField(){
    if (pixmap == nullptr){
        pixmap = new QPixmap(QSize(totalWidth,totalHeight));
        imagePainter.begin(pixmap);
    }
    setImplicitWidth(totalWidth);
    setImplicitHeight(totalHeight);
    imagePainter.resetTransform();
    imagePainter.setWindow(-totalWidth/2,totalHeight/2,totalWidth,-totalHeight);

    imagePainter.scale(invert,invert);
    //image
    painterPath = QPainterPath();
    initPainterPath();
}
void Field::paint(QPainter* painter){
    painter->drawPixmap(area,*pixmap);
}
void Field::changeMode(bool ifBig){
    std::string prefix = ifBig ? "bigField" : "smallField";
    pen.setWidth(ifBig ? 3 : 2);
    totalHeight           = SingleParams::instance()->_(prefix+".canvas.height");
    totalWidth            = SingleParams::instance()->_(prefix+".canvas.width");
    width                 = SingleParams::instance()->_(prefix+".field.width");
    height                = SingleParams::instance()->_(prefix+".field.height");
    goalWidth             = SingleParams::instance()->_(prefix+".field.goalWidth");
    goalDepth             = SingleParams::instance()->_(prefix+".field.goalDepth");
    penaltyRadius         = SingleParams::instance()->_(prefix+".field.penaltyRadius");
    penaltyCenterLength   = SingleParams::instance()->_(prefix+".field.penaltyCenterLength");
    centerCircleRadius    = SingleParams::instance()->_(prefix+".field.centerCircleRadius");
    invert                = SingleParams::instance()->_("invert") ? -1 : 1;
    initField();
    area = QRect(0,0,this->property("width").toReal(),this->property("height").toReal());
    pixmap->fill(COLOR_DARKGREEN);
    imagePainter.strokePath(painterPath, pen);
    this->update(area);
}
void Field::draw(bool robot,bool ball,bool style){
    counter++;
    static QRect area(0,0,this->property("width").toReal(),this->property("height").toReal());
    pixmap->fill(COLOR_DARKGREEN);
    imagePainter.strokePath(painterPath, pen);

    //for (int i=-1;i>-40;i-=1)
    //    style ? drawOneFrame(i,ball,false) : drawPoint(i,ball,true);
    drawOneFrame(0,true,true,true);
    this->update(area);
}
void Field::drawPoint(int index,bool ball,bool solid){
    static qreal posRatio = 0.1;
    static qreal angRatio = 1;//180/3.14159;
    auto& vision = GlobalData::instance()->msg[index];
    for(int i=0;i<SendCarNum;i++){
        if(vision.RobotFound[BLUE][i]){
            paintBall(solid ? COLOR_BLUE : COLOR_TRANSBLUE
                     ,vision.RobotPosX[BLUE][i]*posRatio,vision.RobotPosY[BLUE][i]*posRatio);
        }
    }
    for(int i=0;i<SendCarNum;i++){
        if(vision.RobotFound[YELLOW][i]){
            paintBall(solid ? COLOR_YELLOW : COLOR_TRANSYELLOW
                     ,vision.RobotPosX[YELLOW][i]*posRatio,vision.RobotPosY[YELLOW][i]*posRatio);
        }
    }
    if (vision.BallFound && ball){
        paintBall(solid ? COLOR_ORANGE : COLOR_TRANSORANGE, vision.Ballx*posRatio,vision.Bally*posRatio);
    }
}
void Field::drawOneFrame(int index,bool ball,bool solid,bool ifDrawNum) {
    static qreal posRatio = 0.1;
    static qreal angRatio = 1;//180/3.14159;
    auto& vision = GlobalData::instance()->msg[index];
    for(int i=0;i<SendCarNum;i++){
        if(vision.RobotFound[BLUE][i]){
            paintCar(solid ? COLOR_BLUE : COLOR_TRANSBLUE
                     ,vision.RobotINDEX[BLUE][i]
                     ,vision.RobotPosX[BLUE][i]*posRatio
                     ,vision.RobotPosY[BLUE][i]*posRatio
                     ,vision.RobotRotation[BLUE][i]*angRatio
                     ,ifDrawNum,Qt::white);
        }
    }
    for (int i=0;i<SendCarNum;i++) {
        if (vision.RobotFound[YELLOW][i]) {
            paintCar(solid ? COLOR_YELLOW : COLOR_TRANSYELLOW
                     ,vision.RobotINDEX[YELLOW][i]
                     ,vision.RobotPosX[YELLOW][i]*posRatio
                     ,vision.RobotPosY[YELLOW][i]*posRatio
                     ,vision.RobotRotation[YELLOW][i]*angRatio
                     ,ifDrawNum,Qt::black);
        }
    }
    if (vision.BallFound && ball){
        paintBall(solid ? COLOR_ORANGE : COLOR_TRANSORANGE, vision.Ballx*posRatio,vision.Bally*posRatio);
    }
}
void Field::paintCar(const QColor& color,quint8 num,qreal x,qreal y,qreal radian,bool ifDrawNum,const QColor& textColor){
    static float diameter = SingleParams::instance()->_("car.diameter");
    static float faceWidth = SingleParams::instance()->_("car.faceWidth");
    qreal radius = diameter/2;
    qreal chordAngel = qRadiansToDegrees(qAcos(faceWidth/diameter));
    imagePainter.setBrush(QBrush(color));
    imagePainter.setPen(Qt::NoPen);
    imagePainter.drawChord(x-radius,y-radius,diameter,diameter,(90.0-chordAngel - radian)*16,(180.0+2*chordAngel)*16);
    if (ifDrawNum) {
        imagePainter.setBrush(Qt::NoBrush);
        imagePainter.setPen(QPen(textColor));
        imagePainter.drawText(x-5,y+5,QString::number(num));
    }
}
void Field::paintBall(const QColor& color,qreal x, qreal y){
    static float radius = float(SingleParams::instance()->_("ball.diameter"))/2;
    imagePainter.setBrush(QBrush(color));
    imagePainter.setPen(Qt::NoPen);
    imagePainter.drawEllipse(x-radius,y-radius,2*radius,2*radius);
}
void Field::initPainterPath(){
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
quint16 Field::getFPS(){
    static QElapsedTimer timer;
    static bool ifStart = false;
    static quint64 lastCount;
    static quint16 result;
    if (!ifStart) {
        ifStart = true;
        timer.start();
        lastCount = counter;
        return 0;
    }
    result = (counter - lastCount)*1000.0/timer.restart();
    lastCount = counter;
    return result;
}
