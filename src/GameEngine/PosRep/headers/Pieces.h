#ifndef PIECES_H
#define PIECES_H
#include  <cctype>
#include "BitBoard.h"

enum PIECE {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5
};

enum SIDE{
    WHITE = 0,
    BLACK = 1
};

class Pieces{
public:
    Pieces();
    Pieces(const Pieces& other) = default;
    Pieces(const std::string &shortFen, bool Flag = false);

    friend std::ostream &operator<<(std::ostream& ostream, Pieces pieces);
    friend bool operator==(const Pieces& left, const Pieces& right);

    void updateBitBoards();
    void setPieceBitBoard(uint8_t side, uint8_t piece, BitBoard bb);

    [[nodiscard]] std::array<std::array<BitBoard, 6>, 2> getPieceBitBoards() const;
    [[nodiscard]] BitBoard getPieceBitBoard(uint8_t side, uint8_t piece) const;
    [[nodiscard]] BitBoard getSideBitBoard(uint8_t side) const;
    [[nodiscard]] BitBoard getInvSideBitBoard(uint8_t side) const;
    [[nodiscard]] BitBoard getAllBitBoard() const;
    [[nodiscard]] BitBoard getEmptyBitBoard() const;

    static uint8_t inverse(uint8_t side);



private:
    std::array<std::array<BitBoard, 6>, 2> pieceBitBoards{};
    std::array<BitBoard, 2> sideBitBoards{};
    std::array<BitBoard,2> invSideBitBoards{};

    BitBoard all;
    BitBoard empty;
};


#endif // PIECES_H
