#ifndef HELPER_H
#define HELPER_H
#include "standards.hpp"
void printBitboard(uint64_t bitboard);
void printGameBoard(std::array<uint64_t, 8> bitboards);
int getPositionFromLSB(uint64_t bb);
int countBits(uint64_t num);
uint64_t setOccupancy(int index, uint8_t numBits, uint64_t blockerMask);


inline void setBit(uint64_t& bitboard, uint8_t square)
{
    bitboard |= (1ULL << square);
}
inline bool getBit(uint64_t& bitboard, uint8_t square)
{
    return (bitboard & (1ULL << square));
}
inline void clearBit(uint64_t& bitboard, uint8_t square)
{
    bitboard &= ~(1ULL << square);
}

inline int getPositionFromLSB(uint64_t bb)
{
    // return ( __builtin_ffsl(bb) - 1);
    if(bb)
        return (countBits((bb & -bb) - 1));
    else
        return -1;
}
#endif