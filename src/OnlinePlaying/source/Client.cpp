#include "src/OnlinePlaying/headers/Client.h"

ClientSocket::ClientSocket(QTcpSocket *parent) : QTcpSocket(parent), UserName()
{
    UserName = "ABUDABIEEEELADAS";
    isConnected = false;
    this->gameIsStarted = false;
}

void ClientSocket::setUserName(const QString &UserName_) noexcept
{
    this->UserName = UserName_;
}

void ClientSocket::setChessGUI(ChessGUI *gui_) noexcept
{
    this->gui = gui_;
}

void ClientSocket::makeConnection(QHostAddress address, qint16 port) noexcept
{
    this->connectToHost(address, port);

    QObject::connect(this, &ClientSocket::connected, this, &ClientSocket::onConnected);
    QObject::connect(this, &ClientSocket::connected, this, &ClientSocket::sendUserName);
    QObject::connect(this, &ClientSocket::readyRead, this, &ClientSocket::getDataFromServer);
}

void ClientSocket::ParseMessage(const QString &message)
{
    /* Сценарии для парсировки сообщения от сервера:
     * 1 - получение информации об игре(%VersusWho%YourSide)
     * 2 - получение хода от противника (%MOVE%)
     * 3 - Получение статуса противника(%ONLINEORNOT%)
    */

    QChar command = message[1];
    if(command == '1'){
        if(this->gameIsStarted){
            qDebug() << "Error: game is already started!";
            return;
        }
        //Получаем информацию о противнике
        QString EnemyName = message.mid(3, message.size() - 10);
        QString Side = message.mid(message.size() - 9, 5);

        side = (Side == "WHITE"? WHITE : BLACK);

        this->MakeGame(EnemyName, side);
        gameIsStarted = true;
        return;
    }
    if(command == '2'){
        QString moveStr = message.mid(3, 4);
        uint8_t from = (moveStr[0].toLatin1() - 'a') * 8 + '8' - moveStr[1].toLatin1();
        uint8_t to = (moveStr[2].toLatin1() - 'a')* 8 + '8' - moveStr[3].toLatin1();

        if(gameIsStarted){
            this->gui->MoveOnBoard(from, to);
        }else{
            qDebug() << "Error : No mathching game!";
        }
        return;
    }
    if(command == '3'){
        QString status = message.mid(3, 4);
        if(status == "LEFT"){
            if(this->side == WHITE){
                this->gui->endGame(BLACK_LEFT_GAME);
            }else
                this->gui->endGame(WHITE_LEFT_GAME);
        }else{
            return;
        }
    }
}

bool ClientSocket::sendMessage(const QString &message) noexcept
{
    if(this->state() == QTcpSocket::UnconnectedState)
        return false;
    else{
        QByteArray data = message.toUtf8();
        this->write(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        return true;
    }
}

QString ClientSocket::parseMove(const Move &move) noexcept
{


    uint8_t fromX = move.getFrom() % 8;
    uint8_t fromY = move.getFrom() / 8;

    uint8_t toX = move.getTo() % 8;
    uint8_t toY = move.getTo() / 8;

    QString ParsedMove = QString("%1%2%3%4")
                             .arg(QChar('a' + fromX))
                             .arg(QString::number(8 - fromY))
                             .arg(QChar('a' + toX))
                             .arg(QString::number(8 - toY));

    qDebug() << ParsedMove;

    return ParsedMove;
}

bool ClientSocket::SendMoveToServer(const Move &move) noexcept
{
    QString message =  QString("%") + "2" + QString("%")  + this->UserName + "%" + this->parseMove(move) + "%";
    this->sendMessage(message);
}

void ClientSocket::MakeGame(const QString &enemyName, uint8_t yourSide) noexcept
{

}


void ClientSocket::onConnected()
{
    isConnected = true;
    emit connectionEstablished();
}

void ClientSocket::Moved(Move move)
{
    this->SendMoveToServer(move);
}

void ClientSocket::sendUserName()
{
    QString UserNamePlusPassword = "%1%" + this->UserName + "%";
    this->sendMessage(UserNamePlusPassword);
}

void ClientSocket::getDataFromServer()
{
    while(this->bytesAvailable()){
        QString temp = this->readAll();
        if(!temp.isEmpty())
            this->ParseMessage(temp);
    }
}



