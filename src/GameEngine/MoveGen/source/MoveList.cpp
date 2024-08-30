#include "src/GameEngine/MoveGen/headers/MoveList.h"



MoveList::MoveList()
{
    this->size_ = 0;
}

Move &MoveList::operator[](uint8_t index)
{
    return this->moves[index];
}

void MoveList::push_back(Move move)
{
    this->moves[this->size_] = move;
    this->size_++;
}

uint8_t MoveList::size() const
{
    return this->size_;
}

Move MoveList::operator[](uint8_t index) const
{
    return this->moves[index];
}
