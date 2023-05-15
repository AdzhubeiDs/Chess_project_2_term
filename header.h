#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <iostream>
#include <vector>
#include <unordered_set>
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

    inline virtual void print(const Pieces id = Pieces::noth);
    inline virtual unordered_set<pst, hash_pair> getPositions(const boardPieces id);
    static vector<vector<size_t>> map;
    static pst whiteKingPos;
    static pst blackKingPos;
};
#endif // HEADER_H_INCLUDED
