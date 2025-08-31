#include "position.hpp"
class PerftData
{
    public:
        uint64_t nodes, captures, promotions, enpassantMoves, castlingMoves;
        PerftData operator+(PerftData);
        void print(void);
    
};

PerftData Perft(Position position, int depth);