#ifndef SEARCHINTERRUPTER_H
#define SEARCHINTERRUPTER_H

class SearchInterrupter{
public:

    static SearchInterrupter *getPtr();

    SearchInterrupter(const SearchInterrupter& other) = delete;

    void interrupt();
    void resume();
    bool interrupted() const;



private:
    SearchInterrupter();
    static SearchInterrupter* interrupter;
    bool halt;

};

#endif // SEARCHINTERRUPTER_H
