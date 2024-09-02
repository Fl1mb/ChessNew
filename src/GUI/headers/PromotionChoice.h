#ifndef PROMOTIONCHOICE_H
#define PROMOTIONCHOICE_H
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "src/GameEngine/PosRep/headers/Position.h"


class PromotionChoice : public QWidget{
    Q_OBJECT
public:
    explicit PromotionChoice(uint8_t Side,QWidget* parent = nullptr);

    void setButtons() noexcept;

    bool IsMade() noexcept;

    [[nodiscard]]uint8_t getChoice() noexcept;

public slots:
    void BishopPromoting();
    void RookPromoting();
    void QueenPromoting();
    void KnightPromoting();

signals:
    void ChoiceMade();

private:
    QGridLayout* layout;
    QPushButton* KnightChoice;
    QPushButton* BishopChoice;
    QPushButton* QueenChoice;
    QPushButton* RookChoice;

    bool isPromotionChoiceMade;

    uint8_t side;
    uint8_t Choice;
};



#endif // PROMOTIONCHOICE_H
