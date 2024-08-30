#include "src/GUI/headers/BoardElement.h"


BoardElement::BoardElement(ColorOfSquare color_, qreal x_, qreal y_, uint8_t vert, uint8_t goriz, char num, char let, QObject* parent)
    : QObject(parent),
      QGraphicsItem(nullptr),
      color(color_),
      x(x_), y(y_) ,
      PossibleMove(false),
      horizontal(goriz),
      vertical(vert),
      FigureDisable(false)
{
    this->side = WHITE; // default side
    this->piece = NONE;

    this->NumCRD = (num == 'n') ? "" : QString::fromLatin1(&num, 1);
    this->LetCRD = (let == 'n') ? "" : QString::fromLatin1(&let, 1);

}

BoardElement::~BoardElement()
{

}

void BoardElement::setFigure(uint8_t side_, uint8_t piece_)
{
    this->side = side_;
    this->piece = piece_;
    this->update();
}

void BoardElement::setPossible(bool isPossible)
{
    this->PossibleMove = isPossible;
    update();
}

void BoardElement::setFigureDisable()
{
    this->FigureDisable = true;
}

uint8_t BoardElement::getSide() const
{
    return this->side;
}

uint8_t BoardElement::getPiece() const
{
    return this->piece;
}

QRectF BoardElement::boundingRect() const
{
    return QRectF(x, y, width, height);
}

void BoardElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor brushForRect = QColor(color == black? QColor(125, 148, 93) : QColor(238, 238, 213));
    QColor brushForText(color == black? QColor(238, 238, 213) : QColor(125, 148, 93)); // white text color

    painter->setBrush(brushForRect);
    painter->drawRect(boundingRect());

    // Draw text with shadow effect
    painter->setPen(QPen(brushForText));
    painter->setFont(QFont("Anton", 10));
    painter->drawText(boundingRect().adjusted(5, 5, 0, 0), Qt::AlignLeft | Qt::AlignTop, NumCRD); // shadow
    painter->drawText(boundingRect().adjusted(30, 10, -5, -3), Qt::AlignRight | Qt::AlignBottom, LetCRD); // shadow


    if(piece != NONE){
        // рисование фигуры
        QString figure;
        QString Side;

        switch(piece){
        case PAWN:
            figure = "Pawn";
            break;
        case KNIGHT:
            figure = "Knight";
            break;
        case BISHOP:
            figure = "Bishop";
            break;
        case ROOK:
            figure = "Rook";
            break;
        case QUEEN:
            figure = "Queen";
            break;
        case KING:
            figure = "King";
            break;
        }

        switch(this->side){
        case BLACK:
            Side = "White";
            break;
        case WHITE:
            Side = "Black";
            break;
        }



        painter->setRenderHint(QPainter::SmoothPixmapTransform );
        QPixmap pixmap(this->path.arg(Side).arg(figure));
        painter->drawPixmap(boundingRect().center().x() - pixmap.width() / 2 ,  boundingRect().center().y() - pixmap.height() / 2, pixmap);
    }


    if (PossibleMove) {
        painter->setBrush(QColor(16, 184, 251));
        QRectF rect = this->boundingRect();
        QPointF center = rect.center();
        painter->setOpacity(0.8);
        painter->drawEllipse(center.x() - 7, center.y() - 6, 15,15);
    }
}

void BoardElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(PossibleMove){
        //Если этот квадратик - один из возможных ходов, то мы посылаем движку данную координату
        emit MakeMove(QPair<uint8_t, uint8_t>(this->horizontal, this->vertical));
        qDebug() << this->vertical << " " << this->horizontal;
        return;
    }
    if(this->piece == Position::NONE)
        return;
    else if(FigureDisable){
        return;
    }
    else{
        emit GetPrepared(QPair<uint8_t, uint8_t>(this->vertical, this->horizontal));
        return;
    }

    qDebug() << "Pressed";

}

int BoardElement::type() const
{
    return UserType + 1;
}

void BoardElement::advance(int phase)
{
    if(!phase)
        setPos(this->x +  1, y);
}

QPainterPath BoardElement::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

