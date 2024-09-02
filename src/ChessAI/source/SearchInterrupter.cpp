#include "src/ChessAI/headers/SearchInterrupter.h"


SearchInterrupter* SearchInterrupter::interrupter = nullptr;

SearchInterrupter *SearchInterrupter::getPtr()
{
    if(SearchInterrupter::interrupter == nullptr){
        SearchInterrupter::interrupter = new SearchInterrupter();
    }
    return SearchInterrupter::interrupter;
}

void SearchInterrupter::interrupt()
{
    this->halt = true;
}

void SearchInterrupter::resume()
{
    this->halt = false;
}

bool SearchInterrupter::interrupted() const
{
    return this->halt;
}

SearchInterrupter::SearchInterrupter()
{
    this->halt = false;
}
