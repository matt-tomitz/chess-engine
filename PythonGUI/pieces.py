class Piece:
    def __init__(self, board, team, x, y, char=None):
        self.board = board
        self.team = team
        self.x = x
        self.y = y
        self.char = char
        self.moves = set()
        self.coverage = set()
        self.offset = 1 if team == "White" else -1

    
    def cover(self, x, y):
        coverSq = self.board.getSquare(x, y)
        coverSq.addAttacker((self.x, self.y))
        self.coverage.add((x,y))


    def pingRookMoves(self):
        self.pingSlideMoves("+", self.x, self.y)

    def pingBishopMoves(self):
        self.pingSlideMoves("/", self.x, self.y)

    def pingSlideMoves(self, orientation, x ,y, maximum=999):
        offset = 1 if orientation == "/" else 0 #1 For bishop moves and 0 for rook moves
        for dir in [(-1, offset), (1, -offset), (offset,1), (-offset, -1)]:
            maxMoves = maximum
            a,b = dir
            xIndex = x
            yIndex = y
            while onBoard(xIndex+a, yIndex+b):
                xIndex += a
                yIndex += b

                self.cover(xIndex, yIndex)

                piece = self.board.getPiece(xIndex, yIndex)
                if piece:
                    if piece.team != self.team:
                        self.moves.add((xIndex, yIndex))
                    break
                

                self.moves.add((xIndex, yIndex))
                maxMoves -= 1
                if maxMoves == 0:
                    break

    def vacate(self):
        for sq in self.coverage:
            self.board.getSquare(sq).removeAttacker((self.x, self.y))
        self.coverage.clear()
        self.moves.clear()


class Pawn(Piece):
    def ping(self):
        self.vacate()
        self.moves = set()
        if self.board.getPiece(self.x, self.y-self.offset) == None:
            self.moves.add((self.x, self.y-self.offset))

            if (self.y == 1 and self.offset == -1) or (self.y==6 and self.offset==1):
                if self.board.getPiece(self.x, self.y - (2* self.offset)) == None:
                    self.moves.add((self.x, self.y - (2* self.offset)))
        
        for move in [1, -1]:
            x = self.x + move
            y = self.y - self.offset
            if onBoard(x, y):
                self.cover(x, y)
                if self.board.getPiece(x,y) and self.team != self.board.getPiece(x,y).team:
                    self.moves.add((x,y))
        
        # self.updateSquares()

class Knight(Piece):
    def ping(self):
        self.vacate()
        self.moves = set()
        for move in [(1,2), (1,-2), (-1,2), (-1,-2), (2,1), (2, -1), (-2, 1), (-2, -1)]:
            x = self.x + move[0]
            y = self.y + move[1]
            if onBoard(x, y):
                self.cover(x, y)
                piece = self.board.getPiece(x,y)
                if piece and self.team == piece.team:
                    continue
                self.moves.add((x,y))

class Bishop(Piece):
    def ping(self):
        self.vacate()
        self.pingBishopMoves()

class Rook(Piece):
    def ping(self):
        self.vacate()
        self.pingRookMoves()


class Queen(Piece):
    def ping(self):
        self.vacate()
        self.pingRookMoves()
        self.pingBishopMoves()

class King(Piece):
    def ping(self):
        self.vacate()
        self.pingSlideMoves('+', self.x, self.y, 1)
        self.pingSlideMoves('/', self.x, self.y, 1)
        self.getLazyCastles()
    
    def getLazyCastles(self): #not even remotely close to fully correct
        y = 0 if self.team =="Black" else 7
        if self.x == 4 and self.y == y:
            for corner in [(0, y) ,(7, y)]:
                sq = self.board.getSquare(corner)
                if type(sq.occupyingPiece) == Rook:
                    self.moves.add((1,y) if corner[0] == 0 else (6,y))

        

def createPiece(board, char,x,y)-> Piece:  
    if char == 'e':
        return None
    else:
        if char == char.lower():
            team = 'Black'
        else:
            team = 'White'
        if 'p' == char.lower():
            piece = Pawn(board, team, x, y)
        elif 'n' == char.lower():
            piece = Knight(board, team, x, y)
        elif 'b' == char.lower():
            piece = Bishop(board, team, x, y)
        elif 'r' == char.lower():
            piece = Rook(board, team, x, y)
        elif 'q' == char.lower():
            piece = Queen(board, team, x, y)
        elif 'k' == char.lower():
            piece = King(board, team, x, y)

        piece.char = char

    return piece

def onBoard(x,y):
    return 0<=x<=7 and 0<=y<=7


def opposing(piece1, piece2):
    return piece1.team != piece2.team



