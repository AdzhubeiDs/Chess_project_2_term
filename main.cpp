#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include "header.h"

using namespace std;

typedef pair<size_t, size_t> pst;
typedef vector<vector<size_t>> vst2;
#define x first
#define y second

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        uintmax_t hash = std::hash<T1>{}(p.first);
        hash <<= sizeof(uintmax_t) * 4;
        hash ^= std::hash<T2>{}(p.second);
        return std::hash<uintmax_t>{}(hash);
    }
};
enum Colors : size_t {
    white = 0,
    black
};
enum Pieces : size_t {
    pawn = 0,
    king,
    knight,
    rook,
    bishop,
    queen,
    noth
};

enum boardPieces : size_t {
    empt = 0,
    whitePawn,  // 1
    whiteKing,  // 2
    whiteKnight,// 3
    whiteRook,  // 4
    whiteBishop,// 5
    whiteQueen, // 6

    blackPawn,  // 7
    blackKing,  // 8
    blackKnight,// 9
    blackRook,  // 10
    blackBishop,// 11
    blackQueen  // 12
};
class Board {
public:
    Board()=default;
    ~Board()=default;

    inline virtual void print(const Pieces id = Pieces::noth) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                cout << map[i][j] << " ";
            }
            cout << "\n";
        }
    }
    inline virtual unordered_set<pst, hash_pair> getPositions(const boardPieces id) {
        unordered_set<pst, hash_pair> ret;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (map[i][j] == id) {
                    ret.insert(pst(i, j));
                }
            }
        }
        return ret;
    }
    static vector<vector<size_t>> map;
    static pst whiteKingPos;
    static pst blackKingPos;
};
vector<vector<size_t>> Board::map = {
    {boardPieces::blackRook, boardPieces::blackKnight, boardPieces::blackBishop, boardPieces::blackQueen, boardPieces::blackKing, boardPieces::blackBishop, boardPieces::blackKnight, boardPieces::blackRook}, // RANK_8
    {boardPieces::blackPawn, boardPieces::blackPawn, boardPieces::blackPawn, boardPieces::blackPawn, boardPieces::blackPawn, boardPieces::blackPawn, boardPieces::blackPawn, boardPieces::blackPawn},
    {boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt},
    {boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt},
    {boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt},
    {boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt, boardPieces::empt},
    {boardPieces::whitePawn, boardPieces::whitePawn, boardPieces::whitePawn, boardPieces::whitePawn, boardPieces::whitePawn, boardPieces::whitePawn, boardPieces::whitePawn, boardPieces::whitePawn},
    {boardPieces::whiteRook, boardPieces::whiteKnight, boardPieces::whiteBishop, boardPieces::whiteQueen, boardPieces::whiteKing, boardPieces::whiteBishop, boardPieces::whiteKnight, boardPieces::whiteRook} // RANK_1
};
pst Board::whiteKingPos = pst(7, 4);
pst Board::blackKingPos = pst(0, 4);

class Piece : public Board {
public:
    vector<unordered_set<pst, hash_pair>> figures;
    Piece() {
        figures.resize(6);
    }
    ~Piece() = default;
protected:
    inline void print(const Pieces id) override final {
        for (auto& it : figures[id]) {
            cout << it.x << " " << it.y << "\n";
        }
    }
    inline unordered_set<pst, hash_pair> getPositions(const boardPieces id) final {
        return Board::getPositions(id);
    }


    vector<pst> pawnMoves(size_t i, size_t j, Colors col) {
        vector<pst> ret;
        if (col == white) {
            // первый ход на 2 клетки вверх
            if (i == 6 && Board::map[5][j] == boardPieces::empt && Board::map[4][j] == boardPieces::empt) {
                ret.push_back(pst(4, j));
            }
            // ход на 1 клетку вверх
            if (i - 1 > 0 && Board::map[i - 1][j] == boardPieces::empt) {
                ret.push_back(pst(i - 1, j));
            }
            // кушание влево вверх
            if (checkRange(i - 1, j - 1) && checkCell(i - 1, j - 1, Colors::white)) {
                ret.push_back(pst(i - 1, j - 1));
            }
            // кушание вправо вверх
            if (checkRange(i - 1, j + 1) && checkCell(i - 1, j + 1, Colors::white)) {
                ret.push_back(pst(i - 1, j + 1));
            }
        }
        else { // black
            // первый ход на 2 клетки вниз
            if (i == 1 && Board::map[2][j] == boardPieces::empt && Board::map[3][j] == boardPieces::empt) {
                ret.push_back(pst(3, j));
            }
            // ход на 1 клетку вниз
            if (i + 1 <= 7 && Board::map[i + 1][j] == boardPieces::empt) {
                ret.push_back(pst(i + 1, j));
            }
            // кушание влево вниз
            if (checkRange(i + 1, j - 1) && checkCell(i + 1, j - 1, Colors::black)) {
                ret.push_back(pst(i + 1, j - 1));
            }
            // кушание вправо вниз
            if (checkRange(i + 1, j + 1) && checkCell(i + 1, j + 1, Colors::black)) {
                ret.push_back(pst(i + 1, j + 1));
            }
        }
        return ret;
    }
    vector<pst> knightMoves(size_t i, size_t j, Colors col) {
        vector<pst> ret;
        // пони ходят буковкой Г
        if (checkRange(i + 2, j - 1) && (Board::map[i + 2][j - 1] == boardPieces::empt || checkCell(i + 2, j - 1, col))) {
            ret.push_back(pst(i + 2, j - 1));
        }
        if (checkRange(i + 2, j + 1) && (Board::map[i + 2][j + 1] == boardPieces::empt || checkCell(i + 2, j + 1, col))) {
            ret.push_back(pst(i + 2, j + 1));
        }
        if (checkRange(i + 1, j - 2) && (Board::map[i + 1][j - 2] == boardPieces::empt || checkCell(i + 1, j - 2, col))) {
            ret.push_back(pst(i + 1, j - 2));
        }
        if (checkRange(i + 1, j + 2) && (Board::map[i + 1][j + 2] == boardPieces::empt || checkCell(i + 1, j + 2, col))) {
            ret.push_back(pst(i + 1, j + 2));
        }
        if (checkRange(i - 1, j - 2) && (Board::map[i - 1][j - 2] == boardPieces::empt || checkCell(i - 1, j - 2, col))) {
            ret.push_back(pst(i - 1, j - 2));
        }
        if (checkRange(i - 1, j + 2) && (Board::map[i - 1][j + 2] == boardPieces::empt || checkCell(i - 1, j + 2, col))) {
            ret.push_back(pst(i - 1, j + 2));
        }
        if (checkRange(i - 2, j - 1) && (Board::map[i - 2][j - 1] == boardPieces::empt || checkCell(i - 2, j - 1, col))) {
            ret.push_back(pst(i - 2, j - 1));
        }
        if (checkRange(i - 2, j + 1) && (Board::map[i - 2][j + 1] == boardPieces::empt || checkCell(i - 2, j + 1, col))) {
            ret.push_back(pst(i - 2, j + 1));
        }
        return ret;
    }
    vector<pst> rookMoves(size_t i, size_t j, Colors col) {
        vector<pst> ret;
        // смотрим клетки сверху
        for (int k = i - 1; k >= 0; k--) {
            if (Board::map[k][j] == boardPieces::empt) {
                ret.push_back(pst(k, j));
            }
            // если встретили вражескую фигуру, то можем ее съесть
            else if (checkCell(k, j, col)) {
                ret.push_back(pst(k, j));
                break;
            }
            else {
                break;
            }
        }
        // смотрим клетки снизу
        for (int k = i + 1; k <= 7; k++) {
            if (Board::map[k][j] == boardPieces::empt) {
                ret.push_back(pst(k, j));
            }
            // если встретили вражескую фигуру, то можем ее съесть
            else if (checkCell(k, j, col)) {
                ret.push_back(pst(k, j));
                break;
            }
            else {
                break;
            }
        }
        // смотрим клетки слева
        for (int k = i - 1; k >= 0; k--) {
            if (Board::map[i][k] == boardPieces::empt) {
                ret.push_back(pst(i, k));
            }
            // если встретили вражескую фигуру, то можем ее съесть
            else if (checkCell(i, k, col)) {
                ret.push_back(pst(i, k));
                break;
            }
            else {
                break;
            }
        }
        // смотрим клетки справа
        for (int k = i + 1; k <= 7; k++) {
            if (Board::map[i][k] == boardPieces::empt) {
                ret.push_back(pst(i, k));
            }
            // если встретили вражескую фигуру, то можем ее съесть
            else if (checkCell(i, k, col)) {
                ret.push_back(pst(i, k));
                break;
            }
            else {
                break;
            }
        }
        return ret;
    }
    vector<pst> bishopMoves(size_t i, size_t j, Colors col) {
        vector<pst> ret;
        // все диагонали диагональ
        for (auto ii : { -1, 1 }) {
            for (auto jj : { -1, 1 }) {
                while (checkRange(i + ii, j + jj)) {
                    if (Board::map[i + ii][j + jj] == empt) {
                        ret.push_back(pst(i + ii, j + jj));
                    }
                    else if (checkCell(i + ii, j + jj, col)) {
                        ret.push_back(pst(i + ii, j + jj));
                        break;
                    }
                    else break;
                    if (ii < 0) ii--;
                    else if (ii > 0) ii++;
                    if (jj < 0) jj--;
                    else if (jj > 0) jj++;
                }
            }
        }
        return ret;
    }
    // спасибо ферзю что ходит как ладья + слон\
    -30 строк кода
    vector<pst> queenMoves(size_t i, size_t j, Colors col) {
        vector<pst> ret = rookMoves(i, j, col);
        vector<pst> temp = bishopMoves(i, j, col);
        ret.insert(ret.end(), temp.begin(), temp.end());
        return ret;
    }


    vector<pst> kingMoves(size_t i, size_t j, Colors col) {
        vector<pst> ret;
        //сделать проверку на невозможные ходы
        vector<vector<bool>> tempMap(8, vector<bool>(8, 0)); //временная карта попадаешь под удар или нет. 1 если попадаешь под удар, 0 если нет
        for (size_t i = 0; i <= 5; i++) {
            Pieces temp = static_cast<Pieces>(i);
            unordered_set<pst, hash_pair> us = Board::getPositions(returnFigure(temp, col));
            // белые пешки
            if (temp == Pieces::pawn && Colors::white) {
                for (auto it : us) {
                    for (auto it2 : { -1, 1 }) {
                        if (checkRange(it.x - 1, it.y + it2)) {
                            tempMap[it.x - 1][it.y + it2] = true;
                        }
                    }
                }
            }
            // черные пешки
            else if (temp == Pieces::pawn && Colors::black) {
                for (auto it : us) {
                    for (auto it2 : { -1, 1 }) {
                        if (checkRange(it.x + 1, it.y + it2)) {
                            tempMap[it.x + 1][it.y + it2] = true;
                        }
                    }
                }
            }
            // ладья
            else if (temp == Pieces::rook) {
                for (auto it : us) {
                    vector<pst> v = rookMoves(it.x, it.y, col);
                    for (auto it2 : v) {
                        tempMap[it2.x][it2.y] = true;
                    }
                }
            }
            // слон
            else if (temp == Pieces::bishop) {
                for (auto it : us) {
                    vector<pst> v = bishopMoves(it.x, it.y, col);
                    for (auto it2 : v) {
                        tempMap[it2.x][it2.y] = true;
                    }
                }
            }
            // конь
            else if (temp == Pieces::knight) {
                for (auto it : us) {
                    vector<pst> v = knightMoves(it.x, it.y, col);
                    for (auto it2 : v) {
                        tempMap[it2.x][it2.y] = true;
                    }
                }
            }
            // ферзь
            else if (temp == Pieces::queen) {
                for (auto it : us) {
                    vector<pst> v = queenMoves(it.x, it.y, col);
                    for (auto it2 : v) {
                        tempMap[it2.x][it2.y] = true;
                    }
                }
            }
            // король
            else if (temp == Pieces::king) {
                for (auto it : { -1, 0, 1 }) {
                    for (auto it2 : { -1, 0, 1 }) {
                        if (it == 0 && it2 == 0) continue;
                        if (checkRange(i + it, j + it2)) {
                            tempMap[i + it][j + it2] = true;
                        }
                    }
                }
            }
        }
        // если мы под шахом

        for (auto ii : { -1, 0, 1 }) {
            for (auto jj : { -1, 0, 1 }) {
                if (ii == 0 && jj == 0) continue;
                if (checkRange(i + ii, j + jj)) {
                    if ((Board::map[i + ii][j + jj] == boardPieces::empt || checkCell(i + ii, j + jj, col))
                        && tempMap[i + ii][j + jj] == false) {
                        ret.push_back(pst(i + ii, j + jj));
                    }
                }
            }
        }
        return ret;
    }


    boardPieces returnFigure(Pieces piece, Colors col) {
        if (col == Colors::white) {
            size_t id = piece + 1;
            return static_cast<boardPieces>(id);
        }
        else {
            size_t id = piece + 1 + 6;
            return static_cast<boardPieces>(id);
        }
    }

    // проверка на границы доски
    inline bool checkRange(size_t i, size_t j) {
        return i >= 0 && i <= 7 && j >= 0 && j <= 7;
    }
    // проверяем, встретили ли мы вражескую фигуру (кроме короля, с ним какая-то пиздень будет)
    inline bool checkCell(size_t i, size_t j, Colors col) {
        // проверяем, если кодировка вражеской фигуры, кроме короля
        if (col == white) {
            return Board::map[i][j] >= 7 && Board::map[i][j] <= 12 && Board::map[i][j] != 8;
        }
        else { // black
            return Board::map[i][j] >= 1 && Board::map[i][j] <= 6 && Board::map[i][j] != 2;
        }
    }
    Colors changeColor(Colors col) {
        return (col == Colors::white ? Colors::black : Colors::white);
    }
};
class PieceWhite : public Piece {
public:
    PieceWhite() {
        isCheck = false;
        isCheckMate = false;
        isStaleMate = false;
        figures[Pieces::pawn] = Piece::getPositions(boardPieces::whitePawn);
        figures[Pieces::knight] = Piece::getPositions(boardPieces::whiteKnight);
        figures[Pieces::king] = Piece::getPositions(boardPieces::whiteKing);
        figures[Pieces::rook] = Piece::getPositions(boardPieces::whiteRook);
        figures[Pieces::bishop] = Piece::getPositions(boardPieces::whiteBishop);
        figures[Pieces::queen] = Piece::getPositions(boardPieces::whiteQueen);
    }
    void get(const Pieces id) {
        this->print(id);
    }
    bool isCheck;
    bool isCheckMate;
    bool isStaleMate;
};

class PieceBlack : public Piece {
public:
    PieceBlack() {
        isCheck = false;
        isCheckMate = false;
        isStaleMate = false;
        figures[Pieces::pawn] = Piece::getPositions(boardPieces::blackPawn);
        figures[Pieces::knight] = Piece::getPositions(boardPieces::blackKnight);
        figures[Pieces::king] = Piece::getPositions(boardPieces::blackKing);
        figures[Pieces::rook] = Piece::getPositions(boardPieces::blackRook);
        figures[Pieces::bishop] = Piece::getPositions(boardPieces::blackBishop);
        figures[Pieces::queen] = Piece::getPositions(boardPieces::blackQueen);
    }
    void get(const Pieces id) {
        this->print(id);
    }
    bool isCheck;
    bool isCheckMate;
    bool isStaleMate;
};

class ChessEngine : public Piece {
public:
    ChessEngine() = default;
    void getMoves(Pieces id, Colors col) {
        unordered_set<pst, hash_pair> us;
        if (col == white) {
            us = Piece::getPositions(boardPieces::whitePawn);
            for (auto it : us) {
                vector<pst> ans = Piece::pawnMoves(it.x, it.y, col);
                for (auto it2 : ans) {
                    cout << it2.x << " " << it2.y << "\n";
                }
                cout << "\n";
            }
        }
        else {
            us = Piece::getPositions(boardPieces::blackPawn);
            for (auto it : us) {
                vector<pst> ans = Piece::pawnMoves(it.x, it.y, col);
                for (auto it2 : ans) {
                    cout << it2.x << " " << it2.y << "\n";
                }
                cout << "\n";
            }
        }
    }
    void printBoard() {
        for (auto it1 : Board::map) {
            for (auto it2 : it1) {
                cout << it2 << " ";
            }
            cout << "\n";
        }
    }

};

int main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);

    ChessEngine engine;
    engine.printBoard();
    return 0;
}
