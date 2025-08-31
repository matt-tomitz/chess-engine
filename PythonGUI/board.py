from PIL.Image import init
from values import DARK, LIGHT, coverageColors
from pieces import Piece, createPiece



class Square:
    def __init__(self, board, widget, x, y, char: str):
        self.board = board
        self.widget = widget #reference to the widget object representing that square
        self.color = LIGHT if (x+y)%2 == 0 else DARK
        self.x = x
        self.y = y
        self.occupyingPiece = createPiece(board, char, x, y)
        self.attackingSquares = set()
    
    def addAttacker(self, square):
        self.attackingSquares.add(square)
        self.updateTerritory()
    
    def removeAttacker(self, square):
        self.attackingSquares.remove(square)
        self.updateTerritory()

    def updateAttackers(self):
        attackers = list(self.attackingSquares)
        for attacker in attackers:
            self.board.getPiece(attacker).ping()
    
    def updateTerritory(self):
        value = 0
        for sq in self.attackingSquares:
            piece = self.board.getSquare(sq).occupyingPiece
            value += piece.offset

        # self.widget["bg"] = coverageColors[value]

    def getEnumSquare(self):
        if (self.y < 4):
            y = self.y + (2 * (3 - self.y)) + 1
        else:
            y = self.y - ((2 * (3 - (7 - self.y))) + 1)
        return y*8 + self.x
        


class Board:
    def __init__(self):
        self.board = {}
        self.activePlayer = 'White'
        self.selectedSquare = None
        self.lastMovedPiece = None
    
    def getSquare(self, x, y=None)-> Square:
        if y == None: #argument is a tuple
            return self.board[(x[0], x[1])]
        return self.board[(x,y)]
    
    def getPiece(self, x, y=None)-> Piece:   
        return self.getSquare(x,y).occupyingPiece




