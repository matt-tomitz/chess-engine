#include "../inc/helper.hpp"

void printBitboard(uint64_t bitboard) {
    const int BOARD_SIZE = 8;

    for (int rank = BOARD_SIZE - 1; rank >= 0; --rank) {
        // Top border of the row
        std::cout << "  +";
        for (int file = 0; file < BOARD_SIZE; ++file) {
            std::cout << "---+";
        }
        std::cout << "\n";

        // Row contents with rank label
        std::cout << rank + 1 << " |";
        for (int file = 0; file < BOARD_SIZE; ++file) {
            int square = rank * BOARD_SIZE + file;
            uint64_t mask = 1ULL << square;
            std::cout << " " << ((bitboard & mask) ? '1' : '0') << " |";
        }
        std::cout << "\n";
    }

    // Bottom border
    std::cout << "  +";
    for (int file = 0; file < BOARD_SIZE; ++file) {
        std::cout << "---+";
    }
    std::cout << "\n";

    // File labels
    std::cout << "    ";
    for (char file = 'a'; file <= 'h'; ++file) {
        std::cout << " " << file << "  ";
    }
    std::cout << "\n";
}


int countBits(uint64_t num)
{
    int count = 0;
    while (num)
    {
        num &= (num-1);
        count++;
    }
    return count;
}

uint64_t setOccupancy(int index, uint8_t numBits, uint64_t blockerMask)
{
    uint64_t occupancy = 0;
    for (int count = 0; count < numBits; count++)
    {
        int square = getPositionFromLSB(blockerMask);
        clearBit(blockerMask, square);

        if (index & (1ULL << count))
            occupancy |= (1ULL << square);
        
    }
    return occupancy;
}

