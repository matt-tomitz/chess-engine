

ranks = ["1", "2", "3", "4", "5", "6", "7", "8"]
files = ["a", "b", "c", "d", "e", "f", "g", "h"]

class Square:
    def __init__(self, board, widget, square):
        self.board = board
        self.widget = widget #reference to the widget object representing that square
        self.square = square
    
    def getCoordinates(self):
        return files[self.square % 8] + ranks[self.square // 8]
    
    def clear(self):
        self.widget["image"] = self.board.images["0"]
    
    def setPiece(self, char):
        self.widget["image"] = self.board.images[char]
    
        



        


class Board:
    def __init__(self, frame, images):
        self.board = {}
        self.frame = frame
        self.images = images
        self.activePlayer = 0
        self.selectedSquare = None
        self.lastMovedPiece = None
    
    def getSquare(self, square)-> Square:
        return self.board[square]
    
