#include "magic.hpp"
#include "moves.hpp"
uint32_t seed = 1804289383;

uint32_t generateRandomU32()
{
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

uint64_t generateRandomU64()
{
    uint64_t n1, n2, n3, n4;
    n1 = generateRandomU32() & 0xFFFF;
    n2 = generateRandomU32() & 0xFFFF;
    n3 = generateRandomU32() & 0xFFFF;
    n4 = generateRandomU32() & 0xFFFF;

    return (n1 | n2 << 16 | n3 << 32 | n4 << 48);
}

uint64_t generateMagicNumberCandidate()
{
    return (generateRandomU64() & generateRandomU64() & generateRandomU64());
}

uint64_t findMagicNumber(int square, int relevantBits, bool isBishop)
{
    uint64_t occupancies[4096], attacks[4096], usedAttacks[4096];
    uint64_t blockers = isBishop ? bishopBlockersMask(square) : rookBlockersMask(square);
    int occupancy_indicies = (1 << relevantBits);

    for (int index = 0; index < occupancy_indicies; index++)
    {
        occupancies[index] = setOccupancy(index, relevantBits, blockers);
        attacks[index] = isBishop ? bishopAttacksOnTheFly(square, occupancies[index]) 
                                  : rookAttacksOnTheFly(square, occupancies[index]);
    }

    for (int count = 0; count < 10000000; count++)
    {
        uint64_t magicNumber = generateMagicNumberCandidate();
        if (countBits((blockers * magicNumber) & 0xFF00000000000000) < 6) continue;

        memset(usedAttacks, 0, sizeof(usedAttacks));
        bool fail;
        int index;
        for (index = 0, fail = false; !fail && index < occupancy_indicies; index++)
        {
            int magicIndex = (occupancies[index] * magicNumber) >> (64 - relevantBits);
            if (usedAttacks[magicIndex] == 0 || usedAttacks[magicIndex] == attacks[index])
            {
                usedAttacks[magicIndex] = attacks[index];
            }
            else
            {
                fail = true;
            }
        }
        if (!fail)
        {
            return magicNumber;
        }
        

    }
    return 0;


}