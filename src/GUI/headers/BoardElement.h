#ifndef BOARDELEMENT_H
#define BOARDELEMENT_H

#include <QGraphicsItem>
#include <QString>
#include <QGraphicsTextItem>
#include <QSize>
#include <QFont>
#include <QPainter>
#include <QObject>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "src/GameEngine/PosRep/headers/Position.h"

enum ColorOfSquare{
    white = 17,
    black
};



class BoardElement : public QObject, public QGraphicsItem{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    BoardElement(ColorOfSquare color_ ,qreal x_, qreal y_, uint8_t vert, uint8_t goriz,char num, char let, QObject* parent = nullptr);
    virtual ~BoardElement() override;

    void setFigure(uint8_t side_, uint8_t piece_);
    void setPossible(bool isPossible);
    void setFigureDisable();
    void setLetters(char Num, char let);

    uint8_t getSide()const;
    uint8_t getPiece() const;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    int type()const override;
    void advance(int phase) override;
    QPainterPath shape() const override;

    static constexpr uint8_t NONE = 255;
    static constexpr qreal width = 70;
    static constexpr qreal height = 70;

signals:
    void GetPrepared(QPair<uint8_t, uint8_t> from);
    void MakeMove(QPair<uint8_t, uint8_t> to);



private:

    uint8_t vertical;
    uint8_t horizontal;
    uint8_t side;
    uint8_t piece;
    ColorOfSquare color;
    bool PossibleMove;
    bool FigureDisable;


    QString NumCRD;
    QString LetCRD;

    qreal x;
    qreal y;

    QString path = QString(":/images/data/images/%1%2.png");
};



#endif // BOARDELEMENT_H
