#ifndef MOVES_H
#define MOVES_H

#include "standards.hpp"
#include "helper.hpp"
#include "magic.hpp"
const int ROOK_OCCUPANCIES = (1 << 12);
const int BISHOP_OCCUPANCIES = (1 << 9);

const std::array<uint64_t, 64> GenerateKingAttacksTable(void);
const std::array<std::array<uint64_t, 64>, 2> GeneratePawnAttacksTable(void);
const std::array<uint64_t, 64> GenerateKnightAttacksTable(void);
void initBishopAttacks(void);
void initRookAttacks(void);
void initPawnAttacks(void);
void initAttacks(void);

uint64_t rookBlockersMask(uint8_t square);
uint64_t bishopBlockersMask(uint8_t square);
uint64_t rookAttacksOnTheFly(uint8_t square, uint64_t blockers);
uint64_t bishopAttacksOnTheFly(uint8_t square, uint64_t blockers);

uint64_t getBishopAttacks(int square, uint64_t occupancy);
uint64_t getRookAttacks(int square, uint64_t occupancy);
uint64_t getQueenAttacks(int square, uint64_t occupancy);
uint64_t getKingAttacks(int square);
uint64_t getKnightAttacks(int square);
uint64_t getPawnAttacks(int square, bool side);




const int rookRelevantBits[64] = {
    12,11,11,11,11,11,11,12,11,10,10,10,10,10,10,11,11,10,10,10,
    10,10,10,11,11,10,10,10,10,10,10,11,11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,11,10,10,10,10,10,10,11,12,11,11,11,
    11,11,11,12
};
const int bishopRelevantBits[64] = {
    6,5,5,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,7,7,7,7,5,5,5,5,7,9,9,7,5,5,5,
    5,7,9,9,7,5,5,5,5,7,7,7,7,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,5,5,6
};

class Move
{
    private:
    public:
        int start, end;
        uint8_t piece, promotion;
        bool doublePush, capture, castle, enpassant;
        Move();
        Move(uint8_t pieceType, int startSquare, int endSquare, uint8_t promotionPiece, bool doublePushMove, bool captureFlag, bool castleFlag, bool enpassantFlag);
        void printDebug(void);
};
#endif

void printAllMoves(std::vector<Move> movesList);