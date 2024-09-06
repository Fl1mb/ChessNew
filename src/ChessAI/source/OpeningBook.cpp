#include "src/ChessAI/headers/OpeningBook.h"


OpeningBook::OpeningBook() = default;
OpeningBook::OpeningBook(const std::string &path)
{
    std::ifstream file(path);
    if(file.is_open()){
        std::cout << "couldnt find file" << std::endl;
        std::exit(255);
    }

    std::string game;
    std::stringstream game_thread;
    std::string string_move;

    uint8_t from;
    uint8_t to;

    MoveList possibleMoves;
    bool move_found;

    Position buff;

    while(std::getline(file, game)){
        game_thread = std::stringstream(game);
        this->moves_.resize(this->moves_.size() + 1);

        buff = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};

        while(game_thread >>  string_move){
            if(!game_thread.good())
                break;

            from = (string_move[1] - '1') * 8 + string_move[0] - 'a';
            to = (string_move[3] - '1') * 8 + string_move[2] - 'a';

            possibleMoves = LegalMoveGen::generate(buff, buff.getMoveCtr() - std::floor(buff.getMoveCtr()) > 1e-7);

            for (uint8_t i =0 ; i < possibleMoves.size(); i++) {
                if (possibleMoves[i].getFrom() == from && possibleMoves[i].getTo() == to) {
                    this->moves_.back().push_back(possibleMoves[i]);
                    buff.move(possibleMoves[i]);
                    move_found = true;
                    break;
                }

            }
            if(!move_found){
                std::cout << "Error in opening book" << std::endl;
                std::exit(255);
            }
        }
    }
    file.close();
}

std::tuple<int32_t, bool, Move> OpeningBook::TryToFindMove(const Position &position)
{
    Position buff;
    std::vector<Move> possible_moves;
    bool move_exist;

    for(int32_t game = 0; game < this->moves_.size(); game ++){
        buff = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};

        if(buff.getPieces() == position.getPieces()){
            move_exist = false;
            for(auto added_move : possible_moves){
                if(added_move == this->moves_[game][0]){
                    move_exist = true;
                    break;
                }
            }

            if(!move_exist)possible_moves.push_back(this->moves_[game][0]);
            continue;
        }

        for(int32_t move = 0; move < this->moves_[game].size() - 1; move++){
            buff.move(this->moves_[game][move]);

            if(buff.getPieces() == position.getPieces()){
                move_exist = false;
                for(auto added_move : possible_moves){
                    if(added_move == this->moves_[game][move + 1]){
                        move_exist = true;
                        break;
                    }
                }

                if(!move_exist)possible_moves.push_back(this->moves_[game][move + 1]);
            }
        }
    }

    if(possible_moves.empty()){
        return std::make_tuple(0, false, Move());
    }

    return std::make_tuple(possible_moves.size(), true, possible_moves[time(nullptr) % possible_moves.size()] );
}
