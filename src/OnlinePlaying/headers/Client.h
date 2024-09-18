#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include "src/GUI/headers/ChessGUI.h"
#include <thread>

class ClientSocket : public QTcpSocket{
    Q_OBJECT
public:
    explicit ClientSocket(QTcpSocket* parent = nullptr);

    void setUserName(const QString& UserName_) noexcept;
    void setChessGUI(ChessGUI* gui_) noexcept;

    void makeConnection(QHostAddress address, qint16 port) noexcept;
    void ParseMessage(const QString& message);

    bool sendMessage(const QString& message) noexcept;

    QString parseMove(const Move& move) noexcept;
    bool SendMoveToServer(const Move& move) noexcept;

    void MakeGame(const QString& enemyName, uint8_t yourSide) noexcept;



public slots:
    void onConnected();
    void Moved(Move move);
    void sendUserName();
    void getDataFromServer();

signals:
    void connectionEstablished();



private:
    QString UserName;
    QString Password;

    ChessGUI* gui;

    uint8_t side;
    bool isConnected;
    bool gameIsStarted;

    friend class MainMenu;

};


#endif // CLIENT_H
