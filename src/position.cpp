#include "../inc/position.hpp"
std::array<uint64_t, NUM_PIECES> Position::GenerateBBFromFEN(std::string fen)
{
    std::string splitFEN[6];
    int index = 0;
    std::string del = " ";
    auto pos = fen.find(del);
    while (pos != std::string::npos) {
		splitFEN[index] = fen.substr(0, pos);
		index++;	
        fen.erase(0, pos + del.length());

        pos = fen.find(del);
    }
    splitFEN[5] = fen;
    
    
    std::array<uint64_t, NUM_PIECES> bitmaps = {0};
    uint8_t position = a8;
    for (std::string::iterator it = splitFEN[0].begin(); it != splitFEN[0].end(); it++)
    {
        switch(*it)
        {
            case 'r':
            case 'R':
                setBit(bitmaps[Rook], position);
                break;
            case 'n':
            case 'N':
                setBit(bitmaps[Knight], position);
                break;
            case 'b':
            case 'B':
                setBit(bitmaps[Bishop], position);
                break;
            case 'q':
            case 'Q':
                setBit(bitmaps[Queen], position);
                break;
            case 'k':
                kingSquare[Black] = position;
                setBit(bitmaps[King], position);
                break;
            case 'K':
                kingSquare[White] = position;
                setBit(bitmaps[King], position);
                break;
            case 'p':
            case 'P':
                setBit(bitmaps[Pawn], position);
                break;
            case '/':
                position -= 16;
        }

        if (*it >= 'b' && *it <= 'r')
        {
            bitmaps[Black] |= (1ULL << position);
            position++;
        }
        else if (*it >= 'B' && *it <= 'R')
        {
            bitmaps[White] |= (1ULL << position);
            position++;
        }
        else if (*it >= '1' && *it <= '8')
        {
            position += (*it - '0');
        }
    }
    occupancies = bitmaps[White] | bitmaps[Black];


    side = (splitFEN[1] == "w") ? White : Black;
    
    castleFlag = 0;
    for (std::string::iterator it = splitFEN[2].begin(); it != splitFEN[2].end(); it++)
    {
    	switch (*it)
    	{
    		case '-':
    		default:
    			break;
    		
    		case 'K':
    			castleFlag |= WK;
    			break;
    		case 'Q':
    			castleFlag |= WQ;
    			break;
    		case 'k':
    			castleFlag |= BK;
    			break;
    		case 'q':
    			castleFlag |= BQ;
    			break;
    	}
    }
    
    if (splitFEN[3] != "-")
    {
    	enpassantSquare = (splitFEN[3][1] - '1') * 8 + (splitFEN[3][0] - 'a');
    }
    else
    {
    	enpassantSquare = -1;
    }
    
    halfMoveCounter = std::stoi(splitFEN[4]);
    fullMoveCounter = std::stoi(splitFEN[5]);
    
    return bitmaps;
}

Position::Position(std::string fen)
{
    initAttacks();
    pieceBB = GenerateBBFromFEN(fen);
}


std::array<uint64_t, NUM_PIECES> Position::getBitboards(void)
{
    return pieceBB;
}

// is square attacked by side?
bool Position::isSquareAttacked(int square, bool side)
{
    return
        (
               ((side == White) && (getPawnAttacks(square, Black) & pieceBB[Pawn] & pieceBB[White]))
            || ((side == Black) && (getPawnAttacks(square, White) & pieceBB[Pawn] & pieceBB[Black]))
            || (getBishopAttacks(square, pieceBB[side] | pieceBB[!side]) & pieceBB[Bishop] & pieceBB[side])
            || (getRookAttacks(square, pieceBB[side] | pieceBB[!side]) & pieceBB[Rook] & pieceBB[side])
            || (getQueenAttacks(square, pieceBB[side] | pieceBB[!side]) & pieceBB[Queen] & pieceBB[side])
            || (getKingAttacks(square) & pieceBB[side] & pieceBB[King])
            || (getKnightAttacks(square) & pieceBB[side] & pieceBB[Knight])
        );
}


void printAttackedBoard(Position position, bool side) {
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
            std::cout << " " << ((position.isSquareAttacked(square, side)) ? '1' : '0') << " |";
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

void Position::printGameBoard(void) {
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
            bool found = false;
            for (int piece = Pawn; found == false && piece <= King; piece++)
            {
                for (int side = White; side <= Black; side++)
                {
                    uint64_t bitboard = pieceBB[side] & pieceBB[piece];
                    if (bitboard & mask)
                    {

                        std::cout << " " << pieceRepresentation[(6*side) + (piece - Pawn)];
                        found = true;
                    }
                }
            }
            std::cout << (found ? "" : "  " ) << " |";

            // std::cout << " " << ((bitboard & mask) ? '1' : '0') << " |";
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


std::vector<Move> Position::generateAllMoves()
{
    std::vector<Move> movesList;
	for (int piece = Pawn; piece <= King; piece++)
	{

		uint64_t bb = pieceBB[piece] & pieceBB[side];
		while (bb)
		{
			int start = getPositionFromLSB(bb);

            if (piece == Pawn)
            {
                int pushSquare, doublePushSquare;
                bool inPromotionTerritory, inDoublePushTerritory;
                if (side == White)
                {
                    pushSquare = (start + 8);
                    doublePushSquare = (start + 16);
                    inPromotionTerritory = (start >= a7 && start <= h7);
                    inDoublePushTerritory = (start >= a2 && start <= h2);
                }
                else //Black
                {
                    pushSquare = (start - 8);
                    doublePushSquare = (start - 16);
                    inPromotionTerritory = (start >= a2 && start <= h2);
                    inDoublePushTerritory = (start >= a7 && start <= h7);


                }
                bool pushSquareOccupied = (((1ULL << pushSquare) & occupancies) != 0);
                bool doublePushSquareOccupied = (((1ULL << doublePushSquare) & occupancies) != 0);
                if (!pushSquareOccupied)
                {
                    if (inPromotionTerritory)
                    {
                        //single push promotion
                        for (int piece = Knight; piece <= Queen; piece++)
                        {
                            // std::cout << coordinates[start] << coordinates[pushSquare] << pieceString[piece] << std::endl;
                            movesList.push_back(Move(Pawn, start, pushSquare, piece, false, false, false, false));
                        }
                    }
                    else
                    {
                        if (inDoublePushTerritory && !doublePushSquareOccupied)
                        {
                            //double push
                            // std::cout << coordinates[start] << coordinates[doublePushSquare] << std::endl;
                            movesList.push_back(Move(Pawn, start, doublePushSquare, 0, true, false, false, false));

                        }
                        //standard single push
                        // std::cout << coordinates[start] << coordinates[pushSquare] << std::endl;
                        movesList.push_back(Move(Pawn, start, pushSquare, 0, false, false, false, false));                        
                    }
                }
            }
            else if (piece == King)
            {
                uint64_t kingsideCastleFlag, queensideCastleFlag;
                int startSquare;

                if (side == White)
                {
                    kingsideCastleFlag = WK;
                    queensideCastleFlag = WQ;

                    startSquare = e1;
                }
                else
                {
                    kingsideCastleFlag = BK;
                    queensideCastleFlag = BQ;

                    startSquare = e8;
                }
                
                bool kingsideSquaresBlocked = ((occupancies & kingsideCastleFlag) != 0);
                bool queensideSquaresBlocked = ((occupancies & queensideCastleFlag) != 0);

                
                if ((castleFlag & kingsideCastleFlag) && (!kingsideSquaresBlocked))
                {
                    bool kingsideAttacked = (isSquareAttacked(startSquare, !side) || isSquareAttacked(startSquare+1, !side));
                    if (!kingsideAttacked)
                    {
                        // std::cout << "Castle kingside" << std::endl;
                        movesList.push_back(Move(King, startSquare, startSquare+2, 0, false, false, true, false));
                    }
                }                
                if ((castleFlag & queensideCastleFlag) && (!queensideSquaresBlocked))
                {
                    bool queensideAttacked = (isSquareAttacked(startSquare, !side) || isSquareAttacked(startSquare-1, !side));
                    if (!queensideAttacked)
                    {
                        // std::cout << "Castle queenside" << std::endl;
                        movesList.push_back(Move(King, startSquare, startSquare-2, 0, false, false, true, false));
                    }
                }
            }	
				
		
			uint64_t attacks;
            int attackSquare;
            uint64_t attackable = pieceBB[!side];
			switch(piece)
			{
				case Pawn:
                    if (enpassantSquare >= 0) attackable |= (1ULL << enpassantSquare);

					attacks = (getPawnAttacks(start, side) & attackable);
					while (attacks)
					{
						attackSquare = getPositionFromLSB(attacks);
                        if (((side == White) && (attackSquare >= a8) && (attackSquare <= h8))
                        || ((side == Black) && (attackSquare >= a1)  && (attackSquare <= h1)))
                        {
                            //capture promotion
                            for (int piece = Knight; piece <= Queen; piece++)
                            {
                                // std::cout << coordinates[start][0] << "x" << coordinates[attackSquare] << pieceString[piece] << std::endl;
                                movesList.push_back(Move(Pawn, start, attackSquare, piece, false, true, false, false));
                            }
                        }
                        else
                        {
                            if (attackSquare == enpassantSquare)
                            {
                                //en passant
                                // std::cout << coordinates[start][0] << "x" << coordinates[attackSquare] << " EN PASSANT" << std::endl;
                                movesList.push_back(Move(Pawn, start, enpassantSquare, 0, false, true, false, true));
                            }
                            else
                            {
                                //standard capture
                                // std::cout << coordinates[start][0] << "x" << coordinates[attackSquare] << std::endl;
                                movesList.push_back(Move(Pawn, start, attackSquare, 0, false, true, false, false));
                            }
                        }
						clearBit(attacks, attackSquare);
					}
					break;

				case Knight:
					attacks = getKnightAttacks(start) & ~pieceBB[side];
                    break;
                case King:
					attacks = getKingAttacks(start) & ~pieceBB[side];
                    break;
                case Rook:
					attacks = getRookAttacks(start, occupancies) & ~pieceBB[side];
                    break;
                case Bishop:
					attacks = getBishopAttacks(start, occupancies) & ~pieceBB[side];
                    break;
                case Queen:
					attacks = getQueenAttacks(start, occupancies) & ~pieceBB[side];
                    break;
			}

            while (attacks)
            {
                attackSquare = getPositionFromLSB(attacks);
                bool capture = ((1ULL << attackSquare) & pieceBB[!side]);
                // std::cout << pieceString[piece] << " " << coordinates[start] << coordinates[attackSquare] << std::endl;
                movesList.push_back(Move(piece, start, attackSquare, 0, false, capture, false, false));
                clearBit(attacks, attackSquare);
            }

			clearBit(bb, start);
		}
	}
    return movesList;
}


int Position::getPieceOnSquare(int square)
{
    for (int piece = Pawn; piece <= King; piece++)
    {
        if (getBit(pieceBB[piece], square))
            return piece;
    }
    return -1;
}
void Position::makeMove(Move move)
{
    //remove piece from start square
    clearBit(pieceBB[move.piece], move.start);
    clearBit(pieceBB[side], move.start);

    if (move.castle)
    {
        int file = move.end % 8;
        if (file > 4)
        {
            //kingside
            clearBit(pieceBB[Rook], move.end+1);
            clearBit(pieceBB[side], move.end+1);
            
            setBit(pieceBB[Rook], move.end-1);
            setBit(pieceBB[side], move.end-1);
        }
        else
        {
            //queenside
            clearBit(pieceBB[Rook], move.end-2);
            clearBit(pieceBB[side], move.end-2);
            
            setBit(pieceBB[Rook], move.end+1);
            setBit(pieceBB[side], move.end+1);


        }
    }
    if (move.capture)
    {
        int removed;
        if (move.enpassant)
        {
            if (side == White) removed = move.end - 8;
            else removed = move.end + 8;
        }
        else
        {
            removed = move.end;
        }

        if (getPieceOnSquare(removed) < 0)
        {
            std::cout << "Side: " << pieceString[side] << std::endl;
            move.printDebug();
            undoMove().undoMove().printGameBoard();
            undoMove().printGameBoard();
            printGameBoard();
            throw std::invalid_argument(coordinates[removed] + " square unoccupied");
        }
        clearBit(pieceBB[!side], removed);
        clearBit(pieceBB[getPieceOnSquare(removed)], removed);

    }
    if (move.promotion)
    {
        //add new piece to end square
        setBit(pieceBB[move.promotion], move.end);
        setBit(pieceBB[side], move.end);
    }
    else 
    {
        //add piece to end square
        setBit(pieceBB[move.piece], move.end);
        setBit(pieceBB[side], move.end);
    }

    if (move.doublePush)
    {
        if (side == White)
        {
            enpassantSquare = move.start + 8;
        }
        else
        {
            enpassantSquare = move.start - 8;
        }
    }
    else
    {
        enpassantSquare = -1;
    }

    if (move.piece == King)
    {
        if (side == White)
            castleFlag &= (~WQ & ~WK);
        else
            castleFlag &= (~BQ & ~BK);

        kingSquare[side] = move.end;
    }
    if (move.start == a1 || move.end == a1)
        castleFlag &= ~WQ;
    else if (move.start == a8 || move.end == a8)
        castleFlag &= ~BQ;
    else if (move.start == h1 || move.end == h1)
        castleFlag &= ~WK;
    else if (move.start == h8 || move.end == h8)
        castleFlag &= ~BK;

    occupancies = pieceBB[White] | pieceBB[Black];

    //switch player
    side = !side;

    //TODO Update move counters
}

void Position::makeNextMove(void)
{
    makeMove(generateAllMoves().at(0));
}



Position Position::undoMove(void)
{
    if (oldPosition)
    {
        return *this->oldPosition;
    }
    else
    {
        throw std::invalid_argument("Cannot undo move, no saved position");
    }
}

bool Position::isLegal(void)
{
    if(isSquareAttacked(kingSquare[!side], side))
        return false;
    return true;
}

char Position::getOccupancy(int square)
{
    uint64_t squareBB = (1ULL << square);
    uint64_t occupancies = squareBB & (pieceBB[White] | pieceBB[Black]);
    if (occupancies)
    {
        for (int piece = Pawn; piece <= King; piece++)
        {
            int pieceRep = (piece - Pawn);
            if (squareBB & pieceBB[piece])
            {
                if (squareBB & pieceBB[Black])
                {
                    pieceRep += 6;
                }
                return pieceRepresentation[pieceRep];
            }
        }
        throw std::invalid_argument("getOccupancy function failed to determine occupancy of square " + square);

    }
    else
    {
        return '0';
    }
}

bool PositionManager::tryMove(int start, int end)
{
    std::vector<Move> moves = position.generateAllMoves();
    int move = 0;
    for (auto it = moves.begin(); it != moves.end(); it++)
    {
        if (it->start == start && it->end == end)
        {

            backup = position;
            position.makeMove(*it);
            if (position.isLegal())
            {
                position.lastMove = *it;
                return true;
            }
            else
            {
                position = backup;
            }

        }        
    }
    return false;
}

bool PositionManager::tryPromotion(int start, int end, int promotion)
{
    std::vector<Move> moves = position.generateAllMoves();
    for (auto it = moves.begin(); it != moves.end(); it++)
    {
        if (it->start == start && it->end == end && it->promotion == promotion)
        {
            backup = position;
            position.makeMove(*it);
            if (position.isLegal())
            {
                position.lastMove = *it;
                return true;
            }
            else
            {
                position = backup;
            }

        }        
    }
    return false;
}

void PositionManager::undoMove(void)
{
    position = backup;
}

std::vector<Move> PositionManager::generateAllLegalMoves(void)
{
    std::vector<Move> legalMoves, moves = position.generateAllMoves();
    for (auto it = moves.begin(); it != moves.end(); it++)
    {
        backup = position;
        position.makeMove(*it);
        if (position.isLegal())
        {
            legalMoves.push_back(*it);
        }
        position = backup;
    }
    return legalMoves;

}
Move PositionManager::playRandomLegalMove(void)
{
    std::vector<Move> moves = generateAllLegalMoves();
    Move move = moves.at(std::rand() % moves.size());
    position.makeMove(move);
    
    return move;
}


int PositionManager::printGameBoard(void)
{
    position.printGameBoard();
    return 0;
}

char PositionManager::getOccupancy(int square)
{
    return position.getOccupancy(square);
}

Move PositionManager::getLastMove(void)
{
    return position.lastMove;
}

