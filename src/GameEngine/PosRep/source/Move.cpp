#include "src/GameEngine/PosRep/headers/Move.h"



Move::Move() = default;

bool operator==(Move right, Move left){
    return (right.from == left.from &&
            right.to == left.to &&
            right.attackerSide == left.attackerSide &&
            right.attackerType == left.attackerType &&
            right.defenderSide == left.defenderSide &&
            right.defenderType == left.defenderType &&
            right.flag == left.flag);
}


Move::Move(uint8_t from, uint8_t to, uint8_t attacterType, uint8_t attackerSide, uint8_t defenderType, uint8_t defenderSide, uint8_t flag)
{
    this->from = from;
    this->to = to;
    this->attackerType = attacterType;
    this->attackerSide = attackerSide;
    this->defenderSide = defenderSide;
    this->defenderType = defenderType;
    this->flag = flag;
}

void Move::setFrom(uint8_t newFrom)
{
    this->from = newFrom;
}

void Move::setTo(uint8_t newTo)
{
    this->to = newTo;
}

void Move::setAttackerType(uint8_t newAttackerType)
{
    this->attackerType = newAttackerType;
}

void Move::setAttackerSide(uint8_t newAttackerSide)
{
    this->attackerSide = newAttackerSide;
}

void Move::setDefenderType(uint8_t newDefenderType)
{
    this->defenderType = newDefenderType;
}

void Move::setDefenderSide(uint8_t newDefenderSide)
{
    this->defenderSide = newDefenderSide;
}

void Move::setFlag(uint8_t newFlag)
{
    this->flag = newFlag;
}

uint8_t Move::getFrom() const
{
    return this->from;
}

uint8_t Move::getTo() const
{
    return this->to;
}

uint8_t Move::getAttackerType() const
{
    return this->attackerType;
}

uint8_t Move::getAttackerSide() const
{
    return this->attackerSide;
}

uint8_t Move::getDefenderType() const
{
    return this->defenderType;
}

uint8_t Move::getDefenderSide() const
{
    return this->defenderSide;
}

uint8_t Move::getFlag() const
{
    return this->flag;
}
