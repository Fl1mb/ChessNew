#include "src/GUI/headers/PromotionChoice.h"

PromotionChoice::PromotionChoice(uint8_t Side, QWidget *parent) : QWidget(parent), side(Side), isPromotionChoiceMade(false)
{
    layout = new QGridLayout;
    KnightChoice = new QPushButton;
    BishopChoice = new QPushButton;
    QueenChoice = new QPushButton;
    RookChoice = new QPushButton;

    this->setButtons();
}

void PromotionChoice::setButtons() noexcept
{
    QString Side;
    QString path{":/images/data/images/%1%2.png"};
    if(side == WHITE){
        Side = "White";
    }else{
        Side = "Black";
    }

    BishopChoice->setIcon(QIcon(path.arg(Side).arg("Bishop")));
    KnightChoice->setIcon(QIcon(path.arg(Side).arg("Knight")));
    QueenChoice->setIcon(QIcon(path.arg(Side).arg("Queen")));
    RookChoice->setIcon(QIcon(path.arg(Side).arg("Rook")));

    BishopChoice->setIconSize(QSize(40, 40));
    KnightChoice->setIconSize(QSize(40, 40));
    QueenChoice->setIconSize(QSize(40, 40));
    RookChoice->setIconSize(QSize(40, 40));

    QObject::connect(BishopChoice, &QPushButton::clicked, this, &PromotionChoice::BishopPromoting);
    QObject::connect(RookChoice, &QPushButton::clicked, this, &PromotionChoice::RookPromoting);
    QObject::connect(QueenChoice, &QPushButton::clicked, this, &PromotionChoice::QueenPromoting);
    QObject::connect(KnightChoice, &QPushButton::clicked, this, &PromotionChoice::KnightPromoting);

    layout->addWidget(QueenChoice, 0, 0);
    layout->addWidget(BishopChoice, 0, 1);
    layout->addWidget(RookChoice, 1, 0);
    layout->addWidget(KnightChoice, 1, 1);

    // Add some spacing
    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);

    // Set a reasonable fixed size
    this->setLayout(layout);
    this->setFixedSize(QSize(200, 150));
}

bool PromotionChoice::IsMade() noexcept
{
    return this->isPromotionChoiceMade;
}

uint8_t PromotionChoice::getChoice() noexcept
{
    return this->Choice;
}

void PromotionChoice::BishopPromoting()
{
    this->Choice = Move::FLAGS::PROMOTE_TO_BISHOP;
    this->isPromotionChoiceMade = true;
    emit ChoiceMade();
}

void PromotionChoice::RookPromoting()
{
    this->Choice = Move::FLAGS::PROMOTE_TO_ROOK;
    this->isPromotionChoiceMade = true;
    emit ChoiceMade();
}

void PromotionChoice::QueenPromoting()
{
    this->Choice = Move::FLAGS::PROMOTE_TO_QUEEN;
    this->isPromotionChoiceMade = true;
    emit ChoiceMade();
}

void PromotionChoice::KnightPromoting()
{
    this->Choice = Move::FLAGS::PROMOTE_TO_KNIGHT;
    this->isPromotionChoiceMade = true;
    emit ChoiceMade();
}

