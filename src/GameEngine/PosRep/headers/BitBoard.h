#include <iostream>
#include <cstdint>
#include <bit>
#include <array>
#include <bitset>

#pragma once

typedef uint64_t BitBoard;

namespace BitBoardOperations {
    static constexpr std::array<uint8_t, 64> BIT_SCAN_TABLE = {
        0, 47,  1, 56, 48, 27,  2, 60,
        57, 49, 41, 37, 28, 16,  3, 61,
        54, 58, 35, 52, 50, 42, 21, 44,
        38, 32, 29, 23, 17, 11,  4, 62,
        46, 55, 26, 59, 40, 36, 15, 53,
        34, 51, 20, 43, 31, 22, 10, 45,
        25, 39, 14, 33, 19, 30,  9, 24,
        13, 18,  8, 12,  7,  6,  5, 63
    };

    static constexpr BitBoard set1(BitBoard bb, uint8_t square){
        bb = bb | (1ull << square);
        return bb;
    }

    static constexpr BitBoard set0(BitBoard bb, uint8_t square){
        bb = bb & (~(1ull << square));
        return bb;
    }

    static constexpr bool getBit(BitBoard bb, uint8_t square){
        return (bb & (1ull << square));
    }

    static constexpr uint8_t count_1(BitBoard bb){
        return std::__popcount(bb);
    }

    static constexpr uint8_t bsf(BitBoard bb) {
        return BIT_SCAN_TABLE[((bb ^ (bb - 1)) * 0x03f79d71b4cb0a89) >> 58];
    }

    static constexpr uint8_t bsr(BitBoard bb) {
        bb = bb | (bb >> 1);
        bb = bb | (bb >> 2);
        bb = bb | (bb >> 4);
        bb = bb | (bb >> 8);
        bb = bb | (bb >> 16);
        bb = bb | (bb >> 32);
        return BIT_SCAN_TABLE[(bb * 0x03f79d71b4cb0a89) >> 58];
    }
}

namespace BRows {
    static constexpr std::array<BitBoard, 8> calcRows() {
        std::array<BitBoard, 8> rows{};
        for (uint8_t y = 0; y < 8; y = y + 1) {
            for (uint8_t x = 0; x < 8; x = x + 1) {
                rows[y] = BitBoardOperations::set1(rows[y], y * 8 + x);
            }
        }
        return rows;
    }
    static constexpr std::array<BitBoard, 8> ROWS = calcRows();
    static constexpr std::array<BitBoard, 8> calcInvRows() {
        std::array<BitBoard, 8> invRows{};
        for (uint8_t i = 0; i < 8; i = i + 1) {
            invRows[i] = ~ROWS[i];
        }
        return invRows;
    }
    static constexpr std::array<BitBoard, 8> INV_ROWS = BRows::calcInvRows();
}

namespace BColumns {
    static constexpr std::array<BitBoard, 8> calcColumns() {
        std::array<BitBoard, 8> columns{};
        for (uint8_t x = 0; x < 8; x = x + 1) {
            for (uint8_t y = 0; y < 8; y = y + 1) {
                columns[x] = BitBoardOperations::set1(columns[x], y * 8 + x);
            }
        }
        return columns;
    }
    static constexpr std::array<BitBoard, 8> COLUMNS = BColumns::calcColumns();
    static constexpr std::array<BitBoard, 8> calcInvColumns() {
        std::array<BitBoard, 8> invColumns{};
        for (uint8_t i = 0; i < 8; i = i + 1) {
            invColumns[i] = ~COLUMNS[i];
        }
        return invColumns;
    }
    static constexpr std::array<BitBoard, 8> INV_COLUMNS = BColumns::calcInvColumns();
}
