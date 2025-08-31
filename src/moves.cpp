#include "../inc/moves.hpp"
static std::array<std::array<uint64_t, 512>, 64> bishopAttacks;
static std::array<std::array<uint64_t, 4096>, 64> rookAttacks;
static std::array<uint64_t, 64> bishopBlockers;
static std::array<uint64_t, 64> rookBlockers;
static std::array<std::array<uint64_t, 64>, 2> pawnAttacks;
static std::array<uint64_t, 64> kingAttacks;
static std::array<uint64_t, 64> knightAttacks;

Move::Move(){}
Move::Move(uint8_t pieceType, int startSquare, int endSquare, uint8_t promotionPiece, 
            bool doublePushMove, bool captureFlag, bool castleFlag, bool enpassantFlag)
{
    piece = pieceType;
    start = startSquare;
    end = endSquare;
    promotion = promotionPiece;
    doublePush = doublePushMove;
    castle = castleFlag;
    capture = captureFlag;
    enpassant = enpassantFlag;
}

void Move::printDebug(void)
{
    std::cout << "Piece type: " << pieceString[piece] << std::endl;
    std::cout << "Start square: " << coordinates[start] << std::endl;
    std::cout << "End square: " << coordinates[end] << std::endl;
    std::cout << "Promotion piece: " << promotion << std::endl;
    std::cout << "double push: " << doublePush << std::endl;
    std::cout << "castle: " << castle << std::endl;
    std::cout << "capture: " << capture << std::endl;
    std::cout << "enpassant: " << enpassant << std::endl;
}



const std::array<uint64_t, 64> GenerateKingAttacksTable(void)
{
    uint64_t moves;
    uint64_t startSquareBB = 1ULL;
    static std::array<uint64_t, 64> kingAttacksTable;
    for (int sq = a1; sq <= h8; sq++)
    {
        moves = startSquareBB;
        moves |= (moves & notAFile) >> 1;
        moves |= (moves & notHFile) << 1;
        moves |= (moves << 8);
        moves |= (moves >> 8);
        moves ^= startSquareBB;
        startSquareBB <<= 1;

        kingAttacksTable[sq] = moves;
    }
    return kingAttacksTable;
}

void initKingAttacks(void)
{
    kingAttacks = GenerateKingAttacksTable();
}

const std::array<std::array<uint64_t, 64>, 2> GeneratePawnAttacksTable(void)
{
    uint64_t whiteAttacks, blackAttacks;
    uint64_t startSquareBB;
    static std::array<std::array<uint64_t, 64>, 2> pawnAttacksTable;
    for (int sq = a1; sq <= h8; sq++)
    {
        startSquareBB = 1ULL << sq;
        whiteAttacks = startSquareBB;
        whiteAttacks |= (startSquareBB & notAFile) << 7;
        whiteAttacks |= (startSquareBB & notHFile) << 9;
        whiteAttacks ^= startSquareBB;

        blackAttacks = startSquareBB;
        blackAttacks |= (startSquareBB & notHFile) >> 7;
        blackAttacks |= (startSquareBB & notAFile) >> 9;
        blackAttacks ^= startSquareBB;

        pawnAttacksTable[White][sq] = whiteAttacks;
        pawnAttacksTable[Black][sq] = blackAttacks;

    }
    return pawnAttacksTable;
}

void initPawnAttacks(void)
{
    pawnAttacks = GeneratePawnAttacksTable();
}

const std::array<uint64_t, 64> GenerateKnightAttacksTable(void)
{
    uint64_t startSquareBB, moves;
    static std::array<uint64_t, 64> knightAttacksTable;
    for (int sq = a1; sq <= h8; sq++)
    {
        moves = 0;
        startSquareBB = 1ULL << sq;
        //north
        moves |= (startSquareBB & notABFile) << 6;
        moves |= (startSquareBB & notAFile) << 15;
        moves |= (startSquareBB & notHFile) << 17;
        moves |= (startSquareBB & notHGFile) << 10;

        //south
        moves |= (startSquareBB & notABFile) >> 10;
        moves |= (startSquareBB & notAFile) >> 17;
        moves |= (startSquareBB & notHFile) >> 15;
        moves |= (startSquareBB & notHGFile) >> 6;

        knightAttacksTable[sq] = moves;
    }
    return knightAttacksTable;
}
void initKnightAttacks(void)
{
    knightAttacks = GenerateKnightAttacksTable();
}

uint64_t rookBlockersMask(uint8_t square)
{
 uint64_t result = 0;
 uint8_t startRank = square / 8;
 uint8_t startFile = square % 8;
 for (int rank = startRank+1; rank < 7; rank++) 
 {
  result |= (1ULL << (8*rank + startFile));
 }
 for (int rank = startRank-1; rank > 0; rank--) 
 {
  result |= (1ULL << (8*rank + startFile));
 }
 for (int file = startFile+1; file < 7; file++) 
 {
  result |= (1ULL << (8*startRank + file));
 }
 for (int file = startFile-1; file > 0; file--) 
 {
  result |= (1ULL << (8*startRank + file));
 }
 return result;
}

uint64_t bishopBlockersMask(uint8_t square)
{
 uint64_t result = 0;
 uint8_t startRank = square / 8;
 uint8_t startFile = square % 8;
 for (int rank = startRank+1, file = startFile + 1; rank < 7 && file < 7; rank++, file++) 
 {
  result |= (1ULL << (8*rank + file));
 }
 for (int rank = startRank-1, file = startFile - 1; rank > 0 && file > 0; rank--, file--) 
 {
  result |= (1ULL << (8*rank + file));
 }
 for (int rank = startRank+1, file = startFile - 1; rank < 7 && file > 0; rank++, file--) 
 {
  result |= (1ULL << (8*rank + file));
 }
 for (int rank = startRank-1, file = startFile + 1; rank > 0 && file < 7; rank--, file++) 
 {
  result |= (1ULL << (8*rank + file));
 }
 return result;
}



uint64_t rookAttacksOnTheFly(uint8_t square, uint64_t blockers)
{
 uint64_t result = 0, squareBB;
 uint8_t startRank = square / 8;
 uint8_t startFile = square % 8;
 for (int rank = startRank+1; rank < 8; rank++) 
 {
  squareBB =  1ULL << (8*rank + startFile);
  result |= squareBB;
  if (blockers & squareBB ) break;
 }
 for (int rank = startRank-1; rank >= 0; rank--) 
 {
  squareBB =  1ULL << (8*rank + startFile);
  result |= squareBB;
  if (blockers & squareBB ) break;
 }
 for (int file = startFile+1; file < 8; file++) 
 {
  squareBB =  1ULL << (8*startRank + file);
  result |= squareBB;
  if (blockers & squareBB ) break;
 }
 for (int file = startFile-1; file >= 0; file--) 
 {
  squareBB =  1ULL << (8*startRank + file);
  result |= squareBB;
  if (blockers & squareBB ) break;
 }
 return result;
}

uint64_t bishopAttacksOnTheFly(uint8_t square, uint64_t blockers)
{
 uint64_t result = 0, squareBB;
 uint8_t startRank = square / 8;
 uint8_t startFile = square % 8;
 for (int rank = startRank+1, file = startFile + 1; rank < 8 && file < 8; rank++, file++) 
 {
  squareBB = 1ULL << (8*rank + file);
  result |= squareBB;
  if (blockers & squareBB) break;
 }
 for (int rank = startRank-1, file = startFile - 1; rank >= 0 && file >= 0; rank--, file--) 
 {
  squareBB = 1ULL << (8*rank + file);
  result |= squareBB;
  if (blockers & squareBB) break;
 }
 for (int rank = startRank+1, file = startFile - 1; rank < 8 && file >= 0; rank++, file--) 
 {
  squareBB = 1ULL << (8*rank + file);
  result |= squareBB;
  if (blockers & squareBB) break;
 }
 for (int rank = startRank-1, file = startFile + 1; rank >= 0 && file < 8; rank--, file++) 
 {
  squareBB = 1ULL << (8*rank + file);
  result |= squareBB;
  if (blockers & squareBB) break;
 }
 return result;
}

void initBishopAttacks()
{
    
    for (int square = a1; square <= h8; square++)
    {
        bishopBlockers[square] = bishopBlockersMask(square);
        for (int index = 0; index < BISHOP_OCCUPANCIES; index++)
        {
            uint64_t occupancy = setOccupancy(index, bishopRelevantBits[square], bishopBlockersMask(square));
            int magicIndex = (occupancy * bishopMagic[square]) >> (64 - bishopRelevantBits[square]);
            bishopAttacks[square][magicIndex] = bishopAttacksOnTheFly(square, occupancy);
        }
    }
}
void initRookAttacks()
{
    for (int square = a1; square <= h8; square++)
    {
        rookBlockers[square] = rookBlockersMask(square);
        for (int index = 0; index < ROOK_OCCUPANCIES; index++)
        {
            uint64_t occupancy = setOccupancy(index, rookRelevantBits[square], rookBlockersMask(square));
            int magicIndex = (occupancy * rookMagic[square]) >> (64 - rookRelevantBits[square]);
            rookAttacks[square][magicIndex] = rookAttacksOnTheFly(square, occupancy);
        }
    }
}

uint64_t getBishopAttacks(int square, uint64_t occupancy)
{
    int magicIndex = ((occupancy & bishopBlockers[square]) * bishopMagic[square]) >> (64 - bishopRelevantBits[square]);
    return bishopAttacks[square][magicIndex];
}

uint64_t getRookAttacks(int square, uint64_t occupancy)
{
    int magicIndex = ((occupancy & rookBlockers[square]) * rookMagic[square]) >> (64 - rookRelevantBits[square]);
    return rookAttacks[square][magicIndex];
}

uint64_t getQueenAttacks(int square, uint64_t occupancy)
{
    return (getBishopAttacks(square, occupancy) | getRookAttacks(square, occupancy));
}

uint64_t getKingAttacks(int square)
{
    return kingAttacks[square];
}
uint64_t getKnightAttacks(int square)
{
    return knightAttacks[square];
}
uint64_t getPawnAttacks(int square, bool side)
{
    return pawnAttacks[side][square];
}
void initAttacks(void)
{
    initBishopAttacks();
    initRookAttacks();
    initPawnAttacks();
    initKnightAttacks();
    initKingAttacks();

}
void printAllMoves(std::vector<Move> movesList)
{
    for (auto it = movesList.begin(); it != movesList.end(); it++)
    {
        std::cout << pieceString[it->piece] << " ";
        std::cout << coordinates[it->start];
        std::cout << coordinates[it->end] << std::endl;
    }
}