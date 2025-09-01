
cdef extern from "../src/moves.cpp":
    pass
cdef extern from "../inc/moves.hpp":
    cdef cppclass Move:
            Move() except +
            int start, end, piece, promotion, castle, enpassant;
    pass
cdef extern from "../src/helper.cpp":
    pass
cdef extern from "../inc/helper.hpp":
    pass
cdef extern from "../src/position.cpp":
    pass

cdef extern from "../inc/position.hpp":
    cdef cppclass PositionManager:
        PositionManager() except +
        int tryMove(int, int)
        int tryPromotion(int, int, int)
        void undoMove()
        void printGameBoard()
        Move playRandomLegalMove()
        Move getLastMove()
        char getOccupancy(int)
    

    pass
        



