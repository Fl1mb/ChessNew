#ifndef SLIDERMASKS_H
#define SLIDERMASKS_H

#include "src/GameEngine/PosRep/headers/BitBoard.h"

namespace SlidersMasks{
    enum DIRECTION{
        NORTH = 0,
        SOUTH,
        WEST,
        EAST,
        NORTH_WEST,
        NORTH_EAST,
        SOUTH_WEST,
        SOUTH_EAST
    };

    static consteval BitBoard calc_mask(uint8_t p, uint8_t dir){
        BitBoard mask = 0;
        int8_t x = p % 8;
        int8_t y = p / 8;

        while (true) {
            switch (dir) {
                case SlidersMasks::DIRECTION::NORTH:
                    y = y + 1;
                    break;
                case SlidersMasks::DIRECTION::SOUTH:
                    y = y - 1;
                    break;
                case SlidersMasks::DIRECTION::WEST:
                    x = x - 1;
                    break;
                case SlidersMasks::DIRECTION::EAST:
                    x = x + 1;
                    break;
                case SlidersMasks::DIRECTION::NORTH_WEST:
                    y = y + 1;
                    x = x - 1;
                    break;
                case SlidersMasks::DIRECTION::NORTH_EAST:
                    y = y + 1;
                    x = x + 1;
                    break;
                case SlidersMasks::DIRECTION::SOUTH_WEST:
                    y = y - 1;
                    x = x - 1;
                    break;
                case SlidersMasks::DIRECTION::SOUTH_EAST:
                    y = y - 1;
                    x = x + 1;
                    break;
            }

            if (x < 0 || x > 7 || y < 0 || y > 7) {
                break;
            }

            mask = BitBoardOperations::set1(mask, y * 8 + x);
        }
        return mask;
    }

    static consteval std::array<std::array<BitBoard, 8>, 64> calc_masks(){
        std::array<std::array<BitBoard, 8>, 64> masks{};
        for(uint8_t i = 0; i < 64; i ++){
            for(uint8_t j = 0; j < 8; j ++ ){
                masks[i][j] = SlidersMasks::calc_mask(i, j);
            }
        }
        return masks;
    }

    static constexpr std::array<std::array<BitBoard, 8>, 64> Masks = calc_masks();
}

#endif // SLIDERMASKS_H
