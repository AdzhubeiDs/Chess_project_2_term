#include "chessEngine.h"
using namespace std;

int main()
{
    Board board;
    ChessEngine ce;
    cout << "\n\n";
    for (int i = 0; i < 1000; i++) {
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