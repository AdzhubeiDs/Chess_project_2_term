#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>
using namespace std;

size_t minR = 1;
size_t maxR = 10000;

random_device rd;
mt19937 mt(rd());
uniform_int_distribution<size_t> dist(minR, maxR);
// size_t rand_num = dist(mt);

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        uintmax_t hash = std::hash<T1>{}(p.first);
        hash <<= sizeof(uintmax_t) * 4;
        hash ^= std::hash<T2>{}(p.second);
        return std::hash<uintmax_t>{}(hash);
    }
};

typedef vector<int> vint;
typedef vector<vint> vint2;
typedef pair<int, int> pint;
typedef vector<pint> vpint;
typedef vector<vpint> vpst2;
typedef unordered_set<pint, hash_pair> us;
typedef vector<us> vus;

enum Colors : size_t {
    white = 0,
    black
};
enum Pieces : size_t {
    pawn = 0,
    knight,
    bishop,
    rook,
    queen,
    king,
    noth
};

enum boardPieces : size_t {
    empt = 0,
    whitePawn,  // 1
    whiteKnight,// 2
    whiteBishop,// 3
    whiteRook,  // 4
    whiteQueen, // 5
    whiteKing,  // 6

    blackPawn,  // 7
    blackKnight,// 8
    blackBishop,// 9
    blackRook,  // 10
    blackQueen,  // 11
    blackKing  // 12
};


class Board {
public:
    Board() = default;
    ~Board() = default;

    inline virtual void print(const Pieces id = Pieces::noth);
    inline virtual us getPositions(const boardPieces id);
    static vector<vector<size_t>> map;
    static pint whiteKingPos;
    static pint blackKingPos;
};
#endif // HEADER_H_INCLUDED
