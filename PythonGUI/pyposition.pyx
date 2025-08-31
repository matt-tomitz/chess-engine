from Position cimport PositionManager
from libcpp.vector cimport vector

cdef class PyPosition:
    cdef PositionManager position

    def __init__(self):
        self.position = PositionManager()

    def printGameBoard(self):
        self.position.printGameBoard()
    
    def tryMove(self, start, end):
        print(start, end)
        success = self.position.tryMove(start, end)
        self.printGameBoard()
        return success
    
    def playRandomLegalMove(self):
        move = self.position.playRandomLegalMove()
        self.printGameBoard()
        return (move.start, move.end)



