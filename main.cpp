#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include "header.h"

using namespace std;
typedef unordered_set<pint, hash_pair> us;
typedef vector<us> vus;

inline void Board:: print(const Pieces id /*= Pieces::noth*/) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                cout << map[i][j] << " ";
            }
            cout << "\n";
        }
    }

inline us Board:: getPositions(const boardPieces id) {
        us ret;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (map[i][j] == id) {
                    ret.insert({ i, j });
                }
            }
        }
        return ret;
    }


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

pint Board::whiteKingPos = { 7, 4 };
pint Board::blackKingPos = { 0, 4 };

class Piece : public Board {
public:
    vector<unordered_set<pint, hash_pair>> figures;
    vector<vector<bool>> attackByEnemies;
    bool isCheck;
    bool isCheckMate;
    bool isStaleMate;

    Piece() {
        figures.resize(6);
        attackByEnemies.resize(8, vector<bool>(8));
    }
    ~Piece() = default;
    void updateAttackByEnemies(const vector<vector<bool>>& newMap) {
        attackByEnemies = newMap;
    }
protected:
    inline void print(const Pieces id) override final {
        for (auto& it : figures[id]) {
            cout << it.first << " " << it.second << "\n";
        }
    }
    inline unordered_set<pint, hash_pair> getPositions(const boardPieces id) final {
        return Board::getPositions(id);
    }
    vpint pawnMoves(int i, int j, Colors col) {
        vpint ret;
        if (col == white) {
            // первый ход на 2 клетки вверх
            if (i == 6 && Board::map[5][j] == boardPieces::empt && Board::map[4][j] == boardPieces::empt) {
                ret.push_back(pint(4, j));
            }
            // ход на 1 клетку вверх
            if (i - 1 > 0 && Board::map[i - 1][j] == boardPieces::empt) {
                ret.push_back(pint(i - 1, j));
            }
            // кушание влево вверх
            if (checkRange(i - 1, j - 1) && checkCell(i - 1, j - 1, Colors::white)) {
                ret.push_back(pint(i - 1, j - 1));
            }
            // кушание вправо вверх
            if (checkRange(i - 1, j + 1) && checkCell(i - 1, j + 1, Colors::white)) {
                ret.push_back(pint(i - 1, j + 1));
            }
        }
        else { // black
            // первый ход на 2 клетки вниз
            if (i == 1 && Board::map[2][j] == boardPieces::empt && Board::map[3][j] == boardPieces::empt) {
                ret.push_back(pint(3, j));
            }
            // ход на 1 клетку вниз
            if (i + 1 <= 7 && Board::map[i + 1][j] == boardPieces::empt) {
                ret.push_back(pint(i + 1, j));
            }
            // кушание влево вниз
            if (checkRange(i + 1, j - 1) && checkCell(i + 1, j - 1, Colors::black)) {
                ret.push_back(pint(i + 1, j - 1));
            }
            // кушание вправо вниз
            if (checkRange(i + 1, j + 1) && checkCell(i + 1, j + 1, Colors::black)) {
                ret.push_back(pint(i + 1, j + 1));
            }
        }
        return ret;
    }
    vpint knightMoves(int i, int j, Colors col) {
        vpint ret;
        // пони ходят буковкой Г
        if (checkRange(i + 2, j - 1) && (Board::map[i + 2][j - 1] == boardPieces::empt || checkCell(i + 2, j - 1, col))) {
            ret.push_back(pint(i + 2, j - 1));
        }
        if (checkRange(i + 2, j + 1) && (Board::map[i + 2][j + 1] == boardPieces::empt || checkCell(i + 2, j + 1, col))) {
            ret.push_back(pint(i + 2, j + 1));
        }
        if (checkRange(i + 1, j - 2) && (Board::map[i + 1][j - 2] == boardPieces::empt || checkCell(i + 1, j - 2, col))) {
            ret.push_back(pint(i + 1, j - 2));
        }
        if (checkRange(i + 1, j + 2) && (Board::map[i + 1][j + 2] == boardPieces::empt || checkCell(i + 1, j + 2, col))) {
            ret.push_back(pint(i + 1, j + 2));
        }
        if (checkRange(i - 1, j - 2) && (Board::map[i - 1][j - 2] == boardPieces::empt || checkCell(i - 1, j - 2, col))) {
            ret.push_back(pint(i - 1, j - 2));
        }
        if (checkRange(i - 1, j + 2) && (Board::map[i - 1][j + 2] == boardPieces::empt || checkCell(i - 1, j + 2, col))) {
            ret.push_back(pint(i - 1, j + 2));
        }
        if (checkRange(i - 2, j - 1) && (Board::map[i - 2][j - 1] == boardPieces::empt || checkCell(i - 2, j - 1, col))) {
            ret.push_back(pint(i - 2, j - 1));
        }
        if (checkRange(i - 2, j + 1) && (Board::map[i - 2][j + 1] == boardPieces::empt || checkCell(i - 2, j + 1, col))) {
            ret.push_back(pint(i - 2, j + 1));
        }
        return ret;
    }
    vpint rookMoves(int i, int j, Colors col) {
        vpint ret;
        // смотрим клетки сверху
        for (int k = i - 1; k >= 0; k--) {
            if (Board::map[k][j] == boardPieces::empt) {
                ret.push_back(pint(k, j));
            }
            // если встретили вражескую фигуру, то можем ее съесть
            else if (checkCell(k, j, col)) {
                ret.push_back(pint(k, j));
                break;
            }
            else if (checkCell(k, j, changeColor(col)) || checkKing(k, j, col)) {
                break;
            }
            else {
                break;
            }
        }
        // смотрим клетки снизу
        for (int k = i + 1; k <= 7; k++) {
            if (Board::map[k][j] == boardPieces::empt) {
                ret.push_back(pint(k, j));
            }
            // если встретили вражескую фигуру, то можем ее съесть
            else if (checkCell(k, j, col)) {
                ret.push_back(pint(k, j));
                break;
            }
            else if (checkCell(k, j, changeColor(col)) || checkKing(k, j, col)) {
                break;
            }
            else {
                break;
            }
        }
        // смотрим клетки слева
        for (int k = i - 1; k >= 0; k--) {
            if (Board::map[i][k] == boardPieces::empt) {
                ret.push_back(pint(i, k));
            }
            // если встретили вражескую фигуру, то можем ее съесть
            else if (checkCell(i, k, col)) {
                ret.push_back(pint(i, k));
                break;
            }
            else if (checkCell(i, k, changeColor(col)) || checkKing(i, k, col)) {
                break;
            }
            else {
                break;
            }
        }
        // смотрим клетки справа
        for (int k = i + 1; k <= 7; k++) {
            if (Board::map[i][k] == boardPieces::empt) {
                ret.push_back(pint(i, k));
            }
            // если встретили вражескую фигуру, то можем ее съесть
            else if (checkCell(i, k, col)) {
                ret.push_back(pint(i, k));
                break;
            }
            else if (checkCell(i, k, changeColor(col)) || checkKing(i, k, col)) {
                break;
            }
            else {
                break;
            }
        }
        return ret;
    }
    vpint bishopMoves(int i, int j, Colors col) {
        vpint ret;

        // все диагонали диагональ
        for (auto ii : { -1, 1 }) {
            for (auto jj : { -1, 1 }) {
                int dx = ii, dy = jj;
                while (checkRange(i + ii, j + jj)) {
                    if (Board::map[i + ii][j + jj] == empt) {
                        ret.push_back(pint(i + ii, j + jj));
                    }
                    else if (checkCell(i + ii, j + jj, col)) {
                        ret.push_back(pint(i + ii, j + jj));
                        break;
                    }
                    else if (checkCell(i + ii, j + jj, changeColor(col)) || checkKing(i + ii, j + jj, col)) {
                        break;
                    }
                    else break;
                    if (dx == -1) ii--;
                    else if (dx == 1) ii++;
                    if (dy == -1) jj--;
                    else if (dy == 1) jj++;
                }
                ii = dx;
            }
        }
        return ret;
    }
    // спасибо ферзю что ходит как ладья + слон\
    -30 строк кода
    vpint queenMoves(int i, int j, Colors col) {
        vpint ret = rookMoves(i, j, col);
        vpint temp = bishopMoves(i, j, col);
        ret.insert(ret.end(), temp.begin(), temp.end());
        return ret;
    }
    vpint kingMoves(int i, int j, Colors col) {
        vpint ret;
        for (auto ii : { -1, 0, 1 }) {
            for (auto jj : { -1, 0, 1 }) {
                if (ii == 0 && jj == 0) {
                    continue;
                }
                if (checkRange(i + ii, j + jj) &&
                    (Board::map[i + ii][j + jj] == boardPieces::empt || checkCell(i + ii, j + jj, col)
                        && attackByEnemies[i + ii][j + jj] == false)) {
                    ret.push_back({ i + ii, j + jj });
                }
            }
        }
        return ret;
    }


    bool checkKing(int i, int j, Colors col) {
        if (col == Colors::white) {
            return Board::map[i][j] == boardPieces::whiteKing;
        }
        else {
            return Board::map[i][j] == boardPieces::blackKing;
        }
    }

    //protected:
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
    inline bool checkRange(int i, int j) {
        return i >= 0 && i <= 7 && j >= 0 && j <= 7;
    }
    // проверяем, встретили ли мы вражескую фигуру (кроме короля, с ним какая-то пиздень будет)
    inline bool checkCell(size_t i, size_t j, Colors col) {
        // проверяем, если кодировка вражеской фигуры, кроме короля
        if (col == white) {
            return Board::map[i][j] >= 7 && Board::map[i][j] <= 11;
        }
        else { // black
            return Board::map[i][j] >= 1 && Board::map[i][j] <= 5;
        }
    }
    Colors changeColor(Colors col) {
        return (col == Colors::white ? Colors::black : Colors::white);
    }

    vector<vector<bool>> getAllAttacks(Colors col) {
        vector<vector<bool>> nm(8, vector<bool>(8));
        for (size_t i = 0; i <= 5; i++) {
            Pieces temp = static_cast<Pieces>(i);
            us tempUs = Board::getPositions(returnFigure(temp, col));
            // белые пешки
            if (temp == Pieces::pawn && col == Colors::white) {
                for (auto it : tempUs) {
                    for (auto it2 : { -1, 1 }) {
                        if (checkRange(it.first - 1, it.second + it2)) {
                            nm[it.first - 1][it.second + it2] = true;
                        }
                    }
                }
            }
            // черные пешки
            else if (temp == Pieces::pawn && col == Colors::black) {
                for (auto it : tempUs) {
                    for (auto it2 : { -1, 1 }) {
                        if (checkRange(it.first + 1, it.second + it2)) {
                            nm[it.first + 1][it.second + it2] = true;
                        }
                    }
                }
            }
            // ладья
            else if (temp == Pieces::rook) {
                for (auto it : tempUs) {
                    vector<pint> v = rookMoves(it.first, it.second, col);
                    for (auto it2 : v) {
                        nm[it2.first][it2.second] = true;
                    }
                }
            }
            // слон
            else if (temp == Pieces::bishop) {
                for (auto it : tempUs) {
                    vector<pint> v = bishopMoves(it.first, it.second, col);
                    for (auto it2 : v) {
                        nm[it2.first][it2.second] = true;
                    }
                }
            }
            // конь
            else if (temp == Pieces::knight) {
                for (auto it : tempUs) {
                    vector<pint> v = knightMoves(it.first, it.second, col);
                    for (auto it2 : v) {
                        nm[it2.first][it2.second] = true;
                    }
                }
            }
            // ферзь
            else if (temp == Pieces::queen) {
                for (auto it : tempUs) {
                    vector<pint> v = queenMoves(it.first, it.second, col);
                    for (auto it2 : v) {
                        nm[it2.first][it2.second] = true;
                    }
                }
            }
            // король
            else if (temp == Pieces::king) {
                auto q = *tempUs.begin();
                for (auto it : { -1, 0, 1 }) {
                    for (auto it2 : { -1, 0, 1 }) {
                        if (it == 0 && it2 == 0) continue;
                        if (checkRange(q.first + it, q.second + it2)) {
                            nm[q.first + it][q.second + it2] = true;
                        }
                    }
                }
            }
        }
        return nm;
    }
    void printAttacks() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                cout << attackByEnemies[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
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
        figures[Pieces::bishop] = Piece::getPositions(boardPieces::whiteBishop);
        figures[Pieces::king] = Piece::getPositions(boardPieces::whiteKing);
        figures[Pieces::rook] = Piece::getPositions(boardPieces::whiteRook);
        figures[Pieces::queen] = Piece::getPositions(boardPieces::whiteQueen);
    }
    void get(const Pieces id) {
        this->print(id);
    }
    void getAttacks() {
        this->printAttacks();
    }
};

class PieceBlack : public Piece {
public:
    PieceBlack() {
        isCheck = false;
        isCheckMate = false;
        isStaleMate = false;
        figures[Pieces::pawn] = Piece::getPositions(boardPieces::blackPawn);
        figures[Pieces::knight] = Piece::getPositions(boardPieces::blackKnight);
        figures[Pieces::bishop] = Piece::getPositions(boardPieces::blackBishop);
        figures[Pieces::king] = Piece::getPositions(boardPieces::blackKing);
        figures[Pieces::rook] = Piece::getPositions(boardPieces::blackRook);
        figures[Pieces::queen] = Piece::getPositions(boardPieces::blackQueen);
    }
    void get(const Pieces id) {
        this->print(id);
    }
    void getAttacks() {
        this->printAttacks();
    }
};

struct retNewMove {
    boardPieces bp;
    int from1;
    int from2;
    int to1;
    int to2;
    retNewMove(boardPieces newBp, int newFrom1, int newFrom2, int newTo1, int newTo2) :
        bp(newBp), from1(newFrom1), from2(newFrom2), to1(newTo1), to2(newTo2) {};
};

class ChessEngine : public Piece {
public:
    inline void update() {
        updateWhite();
        updateBlack();
    }
    inline void updateWhite() {
        pw.updateAttackByEnemies(Piece::getAllAttacks(Colors::black));
    }
    inline void updateBlack() {
        pb.updateAttackByEnemies(Piece::getAllAttacks(Colors::white));
    }
    ChessEngine() {
        update();
        pw.getAttacks();
        pb.getAttacks();
    }

    inline bool makeSomething(pint pos, pint from, pint to, Pieces piece, Colors col) {
        if (col == Colors::white) {
            if (Board::map[to.first][to.second] == boardPieces::empt) {
                swap(Board::map[from.first][from.second], Board::map[to.first][to.second]);
                pw.figures[Pieces::pawn].erase(from);
                pw.figures[Pieces::pawn].insert(to);
                update();
                if (pw.attackByEnemies[pos.first][pos.second] == false) {
                    pw.isCheck = false;
                }
                swap(Board::map[from.first][from.second], Board::map[to.first][to.second]);
                pw.figures[Pieces::pawn].insert(from);
                pw.figures[Pieces::pawn].erase(to);
                update();
                return !pw.isCheck;
            }
            else {
                auto tempPos = Board::map[to.first][to.second] - 6 - 1;
                pw.figures[Pieces::pawn].erase(from);
                pw.figures[Pieces::pawn].insert(to);
                pb.figures[tempPos].erase(to);
                Board::map[to.first][to.second] = boardPieces::empt;
                swap(Board::map[from.first][from.second], Board::map[to.first][to.second]);
                update();
                if (pw.attackByEnemies[pos.first][pos.second] == false) {
                    pw.isCheck = false;
                }
                swap(Board::map[from.first][from.second], Board::map[to.first][to.second]);
                pb.figures[tempPos].insert(to);
                pw.figures[Pieces::pawn].insert(from);
                pw.figures[Pieces::pawn].erase(to);
                update();
                return !pw.isCheck;
            }
        }
        else { // если черные
            if (Board::map[to.first][to.second] == boardPieces::empt) {
                swap(Board::map[from.first][from.second], Board::map[to.first][to.second]);
                pb.figures[Pieces::pawn].erase(from);
                pb.figures[Pieces::pawn].insert(to);
                update();
                if (pb.attackByEnemies[pos.first][pos.second] == false) {
                    pb.isCheck = false;
                }
                swap(Board::map[from.first][from.second], Board::map[to.first][to.second]);
                pb.figures[Pieces::pawn].insert(from);
                pb.figures[Pieces::pawn].erase(to);
                update();
                return !pb.isCheck;
            }
            else {
                auto tempPos = Board::map[to.first][to.second] - 1;
                pb.figures[Pieces::pawn].erase(from);
                pb.figures[Pieces::pawn].insert(to);
                pw.figures[tempPos].erase(to);
                Board::map[to.first][to.second] = boardPieces::empt;
                swap(Board::map[from.first][from.second], Board::map[to.first][to.second]);
                update();
                if (pb.attackByEnemies[pos.first][pos.second] == false) {
                    pb.isCheck = false;
                }
                swap(Board::map[from.first][from.second], Board::map[to.first][to.second]);
                pw.figures[tempPos].insert(to);
                pb.figures[Pieces::pawn].insert(from);
                pb.figures[Pieces::pawn].erase(to);
                update();
                return !pb.isCheck;
            }
        }
    }

    retNewMove getNextMove(Colors col) {
        update();
        if (col == Colors::white) {
            // если король под шахом
            pint newMove;
            auto pos = *pw.figures[Pieces::king].begin();
            if (pw.attackByEnemies[pos.first][pos.second] == true) {
                pw.isCheck = true;
                // 2 выхода: уйти от шаха\
                или закрыться от шаха
                /*
                * если мы можем уйти, то супер
                * далее, если не можем уйти, то проверяем, можем ли мы закрыться любой из наших фигур
                * если нет, то нам въебашили жесткий мат !
                */ //\
                                                пытаемся уйти королем
                pint newPos;
                vpint temp = Piece::kingMoves(pos.first, pos.second, col); // получили ходы
                for (auto it : temp) {
                    // если нашли куда отойти
                    if (pw.attackByEnemies[it.first][it.second] == false) {
                        newPos = it;
                        pw.isCheck = false;
                        return retNewMove(boardPieces::whiteKing, pos.first, pos.second, it.first, it.second);
                    }
                }
                // иначе пытаемся закрыться
                // получаем все возможные ходы
                vpst2 temp2(5);
                // пешка
                for (auto it : pw.figures[Pieces::pawn]) {
                    temp = Piece::pawnMoves(it.first, it.second, col);
                    for (auto it2 : temp) {
                        if (makeSomething(pos, it, it2, Pieces::pawn, col)) {
                            // нашли ход
                            pw.isCheck = false;
                            return retNewMove((boardPieces)Board::map[it.first][it.second],
                                it.first, it.second, it2.first, it2.second);
                        }
                    }
                }
                // конь
                for (auto it : pw.figures[Pieces::knight]) {
                    temp = Piece::knightMoves(it.first, it.second, col);
                    for (auto it2 : temp) {
                        if (makeSomething(pos, it, it2, Pieces::knight, col)) {
                            // нашли ход
                            pw.isCheck = false;
                            return retNewMove((boardPieces)Board::map[it.first][it.second],
                                it.first, it.second, it2.first, it2.second);
                        }
                    }
                }
                // слон
                for (auto it : pw.figures[Pieces::bishop]) {
                    temp = Piece::bishopMoves(it.first, it.second, col);
                    for (auto it2 : temp) {
                        if (makeSomething(pos, it, it2, Pieces::bishop, col)) {
                            pw.isCheck = false;
                            return retNewMove((boardPieces)Board::map[it.first][it.second],
                                it.first, it.second, it2.first, it2.second);
                        }
                    }
                }
                // ладья
                for (auto it : pw.figures[Pieces::rook]) {
                    temp = Piece::rookMoves(it.first, it.second, col);
                    for (auto it2 : temp) {
                        if (makeSomething(pos, it, it2, Pieces::rook, col)) {
                            pw.isCheck = false;
                            return retNewMove((boardPieces)Board::map[it.first][it.second],
                                it.first, it.second, it2.first, it2.second);
                        }
                    }
                }
                // ферзь
                for (auto it : pw.figures[Pieces::queen]) {
                    temp = Piece::queenMoves(it.first, it.second, col);
                    for (auto it2 : temp) {
                        if (makeSomething(pos, it, it2, Pieces::queen, col)) {
                            pw.isCheck = false;
                            return retNewMove((boardPieces)Board::map[it.first][it.second],
                                it.first, it.second, it2.first, it2.second);
                        }
                    }
                }
                pw.isCheckMate = true;
                return retNewMove(boardPieces::empt, -1, -1, -1, -1);
            }
            else {
                // СДЕЛАТЬ РАНДОМ

                // getAllMoves()\
                rand()
                vpst2 allMoves = getAllMoves(col);
                size_t cntMoves = 0;
                for (auto it : allMoves) {
                    cntMoves += it.size();
                }
                if (cntMoves == 0) {
                    pw.isStaleMate = true;
                    // Аче дальше делать - то?\
                    наверн просто ретурн + проверка в другом файле на сталемате
                    return retNewMove(boardPieces::empt, -1, -1, -1, -1);
                }
                // ищем рандомный ход
                size_t randMove = dist(mt) % cntMoves;
                size_t tempCnt = 0;
                vpint temp;
                for (size_t i = 0; i <= 5; i++) {
                    for (auto it : pw.figures[i]) {
                        if ((Pieces)i == Pieces::pawn) {
                            temp = Piece::pawnMoves(it.first, it.second, col);
                        }
                        else if ((Pieces)i == Pieces::knight) {
                            temp = Piece::knightMoves(it.first, it.second, col);
                        }
                        else if ((Pieces)i == Pieces::bishop) {
                            temp = Piece::bishopMoves(it.first, it.second, col);
                        }
                        else if ((Pieces)i == Pieces::rook) {
                            temp = Piece::rookMoves(it.first, it.second, col);
                        }
                        else if ((Pieces)i == Pieces::queen) {
                            temp = Piece::queenMoves(it.first, it.second, col);
                        }
                        else if ((Pieces)i == Pieces::king) {
                            temp = Piece::kingMoves(it.first, it.second, col);
                        }
                        for (auto it2 : temp) {
                            if (tempCnt == randMove) {
                                return retNewMove((boardPieces)(i + 1),
                                    it.first, it.second,
                                    it2.first, it2.second);
                            }
                            tempCnt++;
                        }
                    }
                }
            }
        }
        else { // ЕСЛИ ХОДЯТ ЧЕРНЫЕ
            // если король под шахом
            pint newMove;
            auto pos = *pb.figures[Pieces::king].begin();
            if (pb.attackByEnemies[pos.first][pos.second] == true) {
                pb.isCheck = true;
                // 2 выхода: уйти от шаха\
                    или закрыться от шаха
                    /*
                    * если мы можем уйти, то супер
                    * далее, если не можем уйти, то проверяем, можем ли мы закрыться любой из наших фигур
                    * если нет, то нам жесткий мат !
                    */ //\
                                                                        пытаемся уйти королем
                pint newPos;
                vpint temp = Piece::kingMoves(pos.first, pos.second, col); // получили ходы
                for (auto it : temp) {
                    // если нашли куда отойти
                    if (pb.attackByEnemies[it.first][it.second] == false) {
                        pb.isCheck = false;
                        return retNewMove((boardPieces)Board::map[it.first][it.second],
                            pos.first, pos.second, it.first, it.second);
                    }
                }
                // иначе пытаемся закрыться
                // получаем все возможные ходы
                    // пешка
                for (auto it : pb.figures[Pieces::pawn]) {
                    temp = Piece::pawnMoves(it.first, it.second, col);
                    for (auto it2 : temp) {
                        if (makeSomething(pos, it, it2, Pieces::pawn, col)) {
                            pb.isCheck = false;
                            return retNewMove((boardPieces)Board::map[it.first][it.second],
                                it.first, it.second, it2.first, it2.second);
                        }
                    }
                }
                // конь
                for (auto it : pb.figures[Pieces::knight]) {
                    temp = Piece::knightMoves(it.first, it.second, col);
                    for (auto it2 : temp) {
                        if (makeSomething(pos, it, it2, Pieces::knight, col)) {
                            pb.isCheck = false;
                            return retNewMove((boardPieces)Board::map[it.first][it.second],
                                it.first, it.second, it2.first, it2.second);
                        }
                    }
                }
                // слон
                for (auto it : pb.figures[Pieces::bishop]) {
                    temp = Piece::bishopMoves(it.first, it.second, col);
                    for (auto it2 : temp) {
                        if (makeSomething(pos, it, it2, Pieces::bishop, col)) {
                            pb.isCheck = false;
                            return retNewMove((boardPieces)Board::map[it.first][it.second],
                                it.first, it.second, it2.first, it2.second);
                        }
                    }
                }
                // ладья
                for (auto it : pb.figures[Pieces::rook]) {
                    temp = Piece::rookMoves(it.first, it.second, col);
                    for (auto it2 : temp) {
                        if (makeSomething(pos, it, it2, Pieces::rook, col)) {
                            pb.isCheck = false;
                            return retNewMove((boardPieces)Board::map[it.first][it.second],
                                it.first, it.second, it2.first, it2.second);
                        }
                    }
                }
                // ферзь
                for (auto it : pb.figures[Pieces::queen]) {
                    temp = Piece::queenMoves(it.first, it.second, col);
                    for (auto it2 : temp) {
                        if (makeSomething(pos, it, it2, Pieces::queen, col)) {
                            pb.isCheck = false;
                            return retNewMove((boardPieces)Board::map[it.first][it.second],
                                it.first, it.second, it2.first, it2.second);
                        }
                    }
                }
                pb.isCheckMate = true;
                return retNewMove(boardPieces::empt, -1, -1, -1, -1);
            }
            else {
            // СДЕЛАТЬ РАНДОМ

            // getAllMoves()\
                rand()
                vpst2 allMoves = getAllMoves(col);
                size_t cntMoves = 0;
                for (auto it : allMoves) {
                    cntMoves += it.size();
                }
                if (cntMoves == 0) {
                    pb.isStaleMate = true;
                    // Аче дальше делать - то?\
                    наверн просто ретурн + проверка в другом файле на сталемате
                    return retNewMove(boardPieces::empt, -1, -1, -1, -1);
                }
                // ищем рандомный ход
                size_t randMove = dist(mt) % cntMoves;
                size_t tempCnt = 0;
                vpint temp;
                for (size_t i = 0; i <= 5; i++) {
                    for (auto it : pb.figures[i]) {
                        if ((Pieces)i == Pieces::pawn) {
                            temp = Piece::pawnMoves(it.first, it.second, col);
                        }
                        else if ((Pieces)i == Pieces::knight) {
                            temp = Piece::knightMoves(it.first, it.second, col);
                        }
                        else if ((Pieces)i == Pieces::bishop) {
                            temp = Piece::bishopMoves(it.first, it.second, col);
                        }
                        else if ((Pieces)i == Pieces::rook) {
                            temp = Piece::rookMoves(it.first, it.second, col);
                        }
                        else if ((Pieces)i == Pieces::queen) {
                            temp = Piece::queenMoves(it.first, it.second, col);
                        }
                        else if ((Pieces)i == Pieces::king) {
                            temp = Piece::kingMoves(it.first, it.second, col);
                        }
                        for (auto it2 : temp) {
                            if (tempCnt == randMove) {
                                return retNewMove((boardPieces)(i + 6 + 1),
                                    it.first, it.second,
                                    it2.first, it2.second);
                            }
                            tempCnt++;
                        }
                    }
                }
            }
        }
    }
    int makeNextMove(Colors col) {
        retNewMove rnm = getNextMove(col);
        if (col == Colors::white) {
            if (pw.isCheckMate == true) {
                return 1;
            }
            if (pw.isStaleMate == true) {
                return 2;
            }
            // если кого-то едим, то должны удалить чужую фигуру
            if (Board::map[rnm.to1][rnm.to2] != boardPieces::empt) {
                pb.figures[Board::map[rnm.to1][rnm.to2] - 6 - 1].erase({ rnm.to1, rnm.to2 });
                Board::map[rnm.to1][rnm.to2] = boardPieces::empt;
            }
            pw.figures[rnm.bp - 1].erase({ rnm.from1, rnm.from2 });
            pw.figures[rnm.bp - 1].insert({ rnm.to1, rnm.to2 });
            swap(Board::map[rnm.from1][rnm.from2], Board::map[rnm.to1][rnm.to2]);
            update();
            return 0;
        }
        else {
            if (col == Colors::black && pb.isCheckMate == true) {
                return 1;
            }
            if (col == Colors::black && pb.isStaleMate == true) {
                return 2;
            }
            // если кого-то едим, то должны удалить чужую фигуру
            if (Board::map[rnm.to1][rnm.to2] != boardPieces::empt) {
                pw.figures[Board::map[rnm.to1][rnm.to2] - 1].erase({ rnm.to1, rnm.to2 });
                Board::map[rnm.to1][rnm.to2] = boardPieces::empt;
            }
            pb.figures[rnm.bp - 6 - 1].erase({ rnm.from1, rnm.from2 });
            pb.figures[rnm.bp - 6 - 1].insert({ rnm.to1, rnm.to2 });
            swap(Board::map[rnm.from1][rnm.from2], Board::map[rnm.to1][rnm.to2]);
            update();
            return 0;
        }
    }
private:
    vpst2 getAllMoves(Colors col) {
        vpst2 tempMoves(6);
        vpint temp;
        for (size_t i = 0; i <= 5; i++) {
            us tempUs;
            if (col == Colors::white) {
                tempUs = pw.figures[i];
            }
            else {
                tempUs = pb.figures[i];
            }
            for (auto it : tempUs) {
                if ((Pieces)i == Pieces::pawn) {
                    temp = Piece::pawnMoves(it.first, it.second, col);
                }
                else if ((Pieces)i == Pieces::knight) {
                    temp = Piece::knightMoves(it.first, it.second, col);
                }
                else if ((Pieces)i == Pieces::bishop) {
                    temp = Piece::bishopMoves(it.first, it.second, col);
                }
                else if ((Pieces)i == Pieces::rook) {
                    temp = Piece::rookMoves(it.first, it.second, col);
                }
                else if ((Pieces)i == Pieces::queen) {
                    temp = Piece::queenMoves(it.first, it.second, col);
                }
                else if ((Pieces)i == Pieces::king) {
                    temp = Piece::kingMoves(it.first, it.second, col);
                }
                tempMoves[i].insert(tempMoves[i].end(), temp.begin(), temp.end());
            }
        }
        return tempMoves;
    }

    PieceWhite pw;
    PieceBlack pb;
};

int main()
{
    Board board;
    ChessEngine ce;
    retNewMove temp = ce.getNextMove(Colors::white);
    cout << "\n\n";
    for (int i = 0; i < 300; i++) {
        int q = ce.makeNextMove((Colors)(i % 2));
        cout << i << ":\n";
        board.print();
        if (q == 1) {
            cout << "\tCHECKMATE BY " << (i % 2 == 0 ? "WHITE" : "BLACK") << "\n";
            break;
        }
        else if (q == 2) {
            cout << "\nSTALEMATE\n";
            break;
        }
        cout << "\n";
    }
    return 0;
}
