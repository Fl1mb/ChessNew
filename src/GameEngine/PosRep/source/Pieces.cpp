#include "src/GameEngine/PosRep/headers/Pieces.h"


Pieces::Pieces() = default;

bool operator==(const Pieces& left, const Pieces& right) {
    for (uint8_t side = 0; side < 2; ++side) {
        for (uint8_t piece = 0; piece < 6; ++piece) {
            if (left.pieceBitBoards[side][piece] != right.pieceBitBoards[side][piece]) {
                return false;
            }
        }
    }
    return true;
}
Pieces::Pieces(const std::string &shortFen, bool Flag)
{
    uint8_t x = 0;
    uint8_t y = 7;

    uint8_t side;

    if(!Flag){
        for (auto buff : shortFen) {
            if (buff == '/') {
                x = 0;
                y = y - 1;
            }
            else if (std::isdigit(buff)) {
                x = x + buff - '0';
            }
            else {
                if (std::isupper(buff)) {
                    buff = (char)std::tolower(buff);
                    side = SIDE::WHITE;
                }
                else {
                    side = SIDE::BLACK;
                }

                switch (buff) {
                    case 'p':
                        this->pieceBitBoards[side][PIECE::PAWN] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::PAWN], y * 8 + x);
                        break;
                    case 'n':
                        this->pieceBitBoards[side][PIECE::KNIGHT] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::KNIGHT], y * 8 + x);
                        break;
                    case 'b':
                        this->pieceBitBoards[side][PIECE::BISHOP] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::BISHOP], y * 8 + x);
                        break;
                    case 'r':
                        this->pieceBitBoards[side][PIECE::ROOK] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::ROOK], y * 8 + x);
                        break;
                    case 'q':
                        this->pieceBitBoards[side][PIECE::QUEEN] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::QUEEN], y * 8 + x);
                        break;
                    case 'k':
                        this->pieceBitBoards[side][PIECE::KING] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::KING], y * 8 + x);
                        break;
                }

                x = x + 1;
            }
        }
    }else{
        for (auto buff : shortFen) {
            if (buff == '/') {
                x = 0;
                y = y - 1;
            }
            else if (std::isdigit(buff)) {
                x = x + buff - '0';
            }
            else {
                if (std::isupper(buff)) {
                    buff = (char)std::tolower(buff);
                    side = SIDE::BLACK;
                }
                else {
                    side = SIDE::WHITE;
                }

                switch (buff) {
                    case 'p':
                        this->pieceBitBoards[side][PIECE::PAWN] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::PAWN], y * 8 + x);
                        break;
                    case 'n':
                        this->pieceBitBoards[side][PIECE::KNIGHT] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::KNIGHT], y * 8 + x);
                        break;
                    case 'b':
                        this->pieceBitBoards[side][PIECE::BISHOP] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::BISHOP], y * 8 + x);
                        break;
                    case 'r':
                        this->pieceBitBoards[side][PIECE::ROOK] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::ROOK], y * 8 + x);
                        break;
                    case 'q':
                        this->pieceBitBoards[side][PIECE::QUEEN] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::QUEEN], y * 8 + x);
                        break;
                    case 'k':
                        this->pieceBitBoards[side][PIECE::KING] = BitBoardOperations::set1(this->pieceBitBoards[side][PIECE::KING], y * 8 + x);
                        break;
                }

                x = x + 1;
            }
        }
    }


    this->updateBitBoards();
}
std::ostream &operator<<(std::ostream &ostream, Pieces pieces) {
    for (int8_t y = 7; y >= 0; y = y - 1) {
        for (uint8_t x = 0; x < 8; x = x + 1) {
            ostream << "|  ";

            uint8_t index = y * 8 + x;

            if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::WHITE][PIECE::PAWN], index)) {
                ostream << "♙";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::WHITE][PIECE::KNIGHT], index)) {
                ostream << "♘";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::WHITE][PIECE::BISHOP], index)) {
                ostream << "♗";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::WHITE][PIECE::ROOK], index)) {
                ostream << "♖";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::WHITE][PIECE::QUEEN], index)) {
                ostream << "♕";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::WHITE][PIECE::KING], index)) {
                ostream << "♔";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::BLACK][PIECE::PAWN], index)) {
                ostream << "♟";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::BLACK][PIECE::KNIGHT], index)) {
                ostream << "♞";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::BLACK][PIECE::BISHOP], index)) {
                ostream << "♝";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::BLACK][PIECE::ROOK], index)) {
                ostream << "♜";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::BLACK][PIECE::QUEEN], index)) {
                ostream << "♛";
            }
            else if (BitBoardOperations::getBit(pieces.pieceBitBoards[SIDE::BLACK][PIECE::KING], index)) {
                ostream << "♚";
            }
            else {
                ostream << " ";
            }

            ostream << "  ";
        }
        ostream << "|";
        if (y != 0) {
            ostream << "\n";
        }
    }

    return ostream;
}
void Pieces::updateBitBoards() {
    this->sideBitBoards[SIDE::WHITE] = this->pieceBitBoards[SIDE::WHITE][PIECE::PAWN] |
                                         this->pieceBitBoards[SIDE::WHITE][PIECE::KNIGHT] |
                                         this->pieceBitBoards[SIDE::WHITE][PIECE::BISHOP] |
                                         this->pieceBitBoards[SIDE::WHITE][PIECE::ROOK] |
                                         this->pieceBitBoards[SIDE::WHITE][PIECE::QUEEN] |
                                         this->pieceBitBoards[SIDE::WHITE][PIECE::KING];

    this->sideBitBoards[SIDE::BLACK] = this->pieceBitBoards[SIDE::BLACK][PIECE::PAWN] |
                                         this->pieceBitBoards[SIDE::BLACK][PIECE::KNIGHT] |
                                         this->pieceBitBoards[SIDE::BLACK][PIECE::BISHOP] |
                                         this->pieceBitBoards[SIDE::BLACK][PIECE::ROOK] |
                                         this->pieceBitBoards[SIDE::BLACK][PIECE::QUEEN] |
                                         this->pieceBitBoards[SIDE::BLACK][PIECE::KING];

    this->invSideBitBoards[SIDE::WHITE] = ~this->sideBitBoards[SIDE::WHITE];
    this->invSideBitBoards[SIDE::BLACK] = ~this->sideBitBoards[SIDE::BLACK];

    this->all = this->sideBitBoards[SIDE::WHITE] | this->sideBitBoards[SIDE::BLACK];
    this->empty = ~this->all;
}
void Pieces::setPieceBitBoard(uint8_t side, uint8_t piece, BitBoard bb) {
    this->pieceBitBoards[side][piece] = bb;
}
std::array<std::array<BitBoard, 6>, 2> Pieces::getPieceBitBoards() const {
    return this->pieceBitBoards;
}
BitBoard Pieces::getPieceBitBoard(uint8_t side, uint8_t piece) const {
    return this->pieceBitBoards[side][piece];
}
BitBoard Pieces::getSideBitBoard(uint8_t side) const {
    return this->sideBitBoards[side];
}
BitBoard Pieces::getInvSideBitBoard(uint8_t side) const {
    return this->invSideBitBoards[side];
}
BitBoard Pieces::getAllBitBoard() const {
    return this->all;
}
BitBoard Pieces::getEmptyBitBoard() const {
    return this->empty;
}
uint8_t Pieces::inverse(uint8_t side) {
    return !side;
}
