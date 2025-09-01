#include "position.hpp"
#include "moves.hpp"
#include "perft.hpp"
int main()
{

    // std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/R3K2R b KQkq - 0 1";

    PositionManager pm;
    pm.printGameBoard();

    
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}