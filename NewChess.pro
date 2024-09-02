QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/ChessAI/source/AI.cpp \
    src/ChessAI/source/Entry.cpp \
    src/ChessAI/source/MoveSorter.cpp \
    src/ChessAI/source/OpeningBook.cpp \
    src/ChessAI/source/SearchInterrupter.cpp \
    src/ChessAI/source/StaticEvaluator.cpp \
    src/ChessAI/source/TranspositionTable.cpp \
    src/GUI/source/BoardElement.cpp \
    src/GUI/source/ChessBoard.cpp \
    src/GUI/source/ChessGUI.cpp \
    src/GUI/source/PromotionChoice.cpp \
    src/GameEngine/MoveGen/headers/tester.cpp \
    src/GameEngine/MoveGen/source/LegalMoveGen.cpp \
    src/GameEngine/MoveGen/source/MoveList.cpp \
    src/GameEngine/MoveGen/source/PsLegalMoveGen.cpp \
    src/GameEngine/PosRep/source/Move.cpp \
    src/GameEngine/PosRep/source/Pieces.cpp \
    src/GameEngine/PosRep/source/Position.cpp \
    src/GameEngine/PosRep/source/RepetitionHistory.cpp \
    src/GameEngine/PosRep/source/ZobristHash.cpp \
    src/main.cpp

HEADERS += \
    src/ChessAI/headers/AI.h \
    src/ChessAI/headers/Entry.h \
    src/ChessAI/headers/MoveSorter.h \
    src/ChessAI/headers/OpeningBook.h \
    src/ChessAI/headers/PassedPawnMasks.h \
    src/ChessAI/headers/SearchInterrupter.h \
    src/ChessAI/headers/StaticEvaluator.h \
    src/ChessAI/headers/TranspositionTable.h \
    src/GUI/headers/BoardElement.h \
    src/GUI/headers/ChessBoard.h \
    src/GUI/headers/ChessGUI.h \
    src/GUI/headers/PromotionChoice.h \
    src/GameEngine/MoveGen/headers/KingMasks.h \
    src/GameEngine/MoveGen/headers/KnightMask.h \
    src/GameEngine/MoveGen/headers/LegalMoveGen.h \
    src/GameEngine/MoveGen/headers/MoveList.h \
    src/GameEngine/MoveGen/headers/PsLegalMoveGen.h \
    src/GameEngine/MoveGen/headers/SliderMasks.h \
    src/GameEngine/MoveGen/headers/tester.h \
    src/GameEngine/PosRep/headers/BitBoard.h \
    src/GameEngine/PosRep/headers/Move.h \
    src/GameEngine/PosRep/headers/Pieces.h \
    src/GameEngine/PosRep/headers/Position.h \
    src/GameEngine/PosRep/headers/RepetitionHistory.h \
    src/GameEngine/PosRep/headers/ZobristHash.h \
    src/GameEngine/PosRep/headers/ZobristHashConstants.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
