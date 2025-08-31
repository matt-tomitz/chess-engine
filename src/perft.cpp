#include "perft.hpp"
PerftData PerftData::operator+(PerftData pdata)
{
    PerftData result = {};
    result.nodes = (this->nodes + pdata.nodes);
    result.promotions = (this->promotions + pdata.promotions);
    result.captures = (this->captures + pdata.captures);
    result.castlingMoves = (this->castlingMoves + pdata.castlingMoves);
    result.enpassantMoves = (this->enpassantMoves + pdata.enpassantMoves);

    return result;
}

void PerftData::print(void)
{
    std::cout << "nodes: " << nodes << std::endl;
    std::cout << "captures: " << captures << std::endl;
    std::cout << "promotions: " << promotions << std::endl;
    std::cout << "castlingMoves: " << castlingMoves << std::endl;
    std::cout << "enpassantMoves: " << enpassantMoves << std::endl;
}

PerftData Perft(Position position, int depth)
{
    PerftData pdata = {};
    if (depth == 0)
    {
        return pdata;
    }

    Position backup;
    std::vector<Move> moves = position.generateAllMoves();
    for (auto it = moves.begin(); it != moves.end(); it++)
    {
        position.makeMove(*it);
        backup = position.undoMove();
        if (position.isLegal())
        {
            if (depth == 1)
            {
                pdata.nodes += 1;
                pdata.castlingMoves += it->castle;
                pdata.enpassantMoves += it->enpassant;
                pdata.captures += it->capture;
                pdata.promotions += (it->promotion) ? 1 : 0;
            }
            if (depth > 1)
         
            pdata = pdata + Perft(position, depth-1);
        }
        position = backup;

    }
    return pdata;
}