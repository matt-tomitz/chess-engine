from Position cimport PositionManager
from Position cimport Move
from libcpp.vector cimport vector

cdef class PyMove:
    cdef Move move;

    def start(self):
        return self.move.start
    def end(self):
        return self.move.end
    def piece(self):
        return self.move.piece
    def promotion(self):
        return self.move.promotion
    def castle(self):
        return self.move.castle
    def enpassant(self):
        return self.move.enpassant

cdef class PyPosition:
    cdef PositionManager position
    cdef PyMove pyMove

    def __init__(self):
        self.position = PositionManager()
        self.pyMove = PyMove()

    def printGameBoard(self):
        self.position.printGameBoard()
    
    def tryMove(self, start, end):
        success = self.position.tryMove(start, end)
        return success



    def tryPromotion(self, start, end, piece):
        success = self.position.tryPromotion(start, end, piece)
        return success
    
    def undoMove(self):
        self.position.undoMove()
    
    def playRandomLegalMove(self):
        self.pyMove.move = self.position.playRandomLegalMove()
        return self.pyMove

    def getLastMove(self):
        self.pyMove.move = self.position.getLastMove()
        return self.pyMove

    def getOccupancy(self, square):
        return self.position.getOccupancy(square)
    


