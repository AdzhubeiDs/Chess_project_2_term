#include "blackAndWhitePieces.h"

class ChessEngine : public Piece {
public:
    inline void update();
    inline void updateWhite();
    inline void updateBlack();

    ChessEngine();
    ~ChessEngine();

    inline bool makeSomething(pair<int, int> pos, pair<int, int> from, pair<int, int> to, Pieces piece, Colors col);

    retNewMove getNextMove(Colors col);

    int makeNextMove(Colors col);
private:
    vector<vector<pair<int, int>>> getAllMoves(Colors col);

    PieceWhite pw;
    PieceBlack pb;

    rnd dist;
};