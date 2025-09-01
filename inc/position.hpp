#ifndef POSITION_H
#define POSITION_H
#include "standards.hpp"
#include "helper.hpp"
#include "moves.hpp"

const std::string defaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w kqKQ - 0 1";
const std::string position2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
const std::string position3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
const std::string position4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
const std::string position5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";



class Position
{
private:
    std::array<uint64_t, NUM_PIECES> pieceBB;
    uint64_t occupancies;
    uint64_t castleFlag;
    bool side;
    int8_t enpassantSquare;
    int halfMoveCounter;
    int fullMoveCounter;
    Position *oldPosition;
    int kingSquare[2];
    public:
    Move lastMove;
    Position(std::string fen = defaultFEN);
    std::array<uint64_t, NUM_PIECES> GenerateBBFromFEN(std::string fen = defaultFEN);
    static uint64_t EvaluateMoves(Piece, int);
    // Position(Position& position);
    bool isSquareAttacked(int, bool);
    std::vector<Move> generateAllMoves();

    void makeMove(Move);
    Position undoMove(void);
    void makeNextMove(void);
    void printGameBoard(void);
    int getPieceOnSquare(int square);
    bool isLegal(void);

    char getOccupancy(int);


    std::array<uint64_t, NUM_PIECES> getBitboards(void);
//    uint64_t getPieceSet(PieceType pt) const {return pieceBB[pieceCode(pt)] & pieceBB[colorCode(pt)];}
//    uint64_t getWhitePawns() const {return pieceBB[Pawn] & pieceBB[nWhite];}
//    ...
//    uint64_t getPawns(ColorType ct) const {return pieceBB[Pawn] & pieceBB[ct];}
//    ...
};

void printAttackedBoard(Position position, bool side);



class PositionManager
{
    public:
        Position position, backup;
        bool tryMove(int start, int end);
        bool tryPromotion(int start, int end, int piece);
        int printGameBoard(void);
        std::vector<Move> generateAllLegalMoves(void);
        Move playRandomLegalMove(void);
        char getOccupancy(int);
        Move getLastMove(void);
        void undoMove(void);

};



#endif //POSITION_H