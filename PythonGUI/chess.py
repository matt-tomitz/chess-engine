from tkinter import *
import cython
from getImages import getImages
from board import Square, Board
from time import sleep
from enum import Enum

import pyposition


LIGHT='#edb798'
DARK='#5c4232'

class eSquare(Enum):
    a1 = 0; b1 = 1; c1 = 2; d1 = 3; e1 = 4; f1 = 5; g1 = 6; h1 = 7
    a2 = 8; b2 = 9; c2 = 10; d2 = 11; e2 = 12; f2 = 13; g2 = 14; h2 = 15
    a3 = 16; b3 = 17; c3 = 18; d3 = 19; e3 = 20; f3 = 21; g3 = 22; h3 = 23
    a4 = 24; b4 = 25; c4 = 26; d4 = 27; e4 = 28; f4 = 29; g4 = 30; h4 = 31
    a5 = 32; b5 = 33; c5 = 34; d5 = 35; e5 = 36; f5 = 37; g5 = 38; h5 = 39
    a6 = 40; b6 = 41; c6 = 42; d6 = 43; e6 = 44; f6 = 45; g6 = 46; h6 = 47
    a7 = 48; b7 = 49; c7 = 50; d7 = 51; e7 = 52; f7 = 53; g7 = 54; h7 = 55
    a8 = 56; b8 = 57; c8 = 58; d8 = 59; e8 = 60; f8 = 61; g8 = 62; h8 = 63

class Piece(Enum):
    White = 0; Black = 1; Pawn = 2; Knight = 3
    Bishop = 4; Rook = 5; Queen = 6; King = 7

pos = pyposition.PyPosition()
pos.printGameBoard()

def getPiece(square: Square):
    return chr(pos.getOccupancy(square.square))
def getPieceBeforeMove(square: Square):
    return chr(pos.getPriorOccupancy(square.square))

def makeButton(frm, board: Board, x, y, squareEnum: int, colorBool, char):
    color = LIGHT if (colorBool) else DARK
    btn = Button(frm, bg=color, image=images[char])
    btn.grid(row=y, column=x)
    square = Square(board, btn, squareEnum)
    board.board[squareEnum] = square
    btn["command"] = lambda sq=square: onClick(sq, board)


def onClick(square: Square, board:Board):
    charPiece = getPiece(square)
    selected = board.selectedSquare
    promotion = False

    piece = charPiece != '0'
    side = (charPiece >= 'a')

    if (piece and (side == board.activePlayer)):
        selectSquare(square, board, charPiece)
    elif selected:
        start = selected.square
        end = square.square
        if ((getPiece(selected) == "P" and end >= eSquare.a8.value) or 
            (getPiece(selected) == "p" and end <= eSquare.h1.value)):
            promotion = True
        if (pos.tryMove(start, end)):
            if promotion:
                pos.undoMove()
                d = simpledialog(board)
                board.frame.wait_window(d.root)
                pos.tryPromotion(start, end, d.promotionPiece)

            movePiece(pos.getLastMove(), board)
            
            #opponent move
            move = pos.playRandomLegalMove()
            movePiece(move, board)
            
        deselectSquare(board)
            


def selectSquare(square, board, char):
    board.selectedSquare = square
    board.selectedSquare.char = char


def deselectSquare(board):
    board.selectedSquare = None

def movePiece(move: pyposition.PyMove, board: Board):
    start = board.getSquare(move.start())
    end = board.getSquare(move.end())

    end.setPiece(getPiece(end))
    start.clear()

    if (move.castle()):
        if (move.end() == eSquare.g1.value):
            rookNewSquare = board.getSquare(eSquare.f1.value)
            rookOldSquare = board.getSquare(eSquare.h1.value)
            rookPiece = "R"
        elif (move.end() == eSquare.c1.value):
            rookNewSquare = board.getSquare(eSquare.d1.value)
            rookOldSquare = board.getSquare(eSquare.a1.value)
            rookPiece = "R"
        elif (move.end() == eSquare.g8.value):
            rookNewSquare = board.getSquare(eSquare.f8.value)
            rookOldSquare = board.getSquare(eSquare.h8.value)
            rookPiece = "r"
        elif (move.end() == eSquare.c8.value):
            rookNewSquare = board.getSquare(eSquare.d8.value)
            rookOldSquare = board.getSquare(eSquare.a8.value)
            rookPiece = "r"
        
        rookNewSquare.setPiece(rookPiece)
        rookOldSquare.clear()

    if (move.enpassant()):
        if move.start() <= eSquare.h4.value:
            enpassantOffset = 8 #Black
        else:
            enpassantOffset = -8 #White
        
        removalSquare = move.end() + enpassantOffset
        board.getSquare(removalSquare).clear()
        

    pos.printGameBoard()
    board.activePlayer = not board.activePlayer



def generateBoard(frm, images, fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"):  #I'm assuming that the fen will always be valid
    board = Board(frm, images)
    x = y = 0
    square = 56

    color = False
    for char in fen:
        try:
            num = int(char)
            while num>0:
                color = not color
                makeButton(frm, board, x, y, square, color, '0')
                num -= 1
                x += 1
                square += 1

        except ValueError:
            if char == '/':
                x = 0
                y += 1
                square -= 16
                color = not color

            else:
                color = not color
                makeButton(frm, board, x, y, square, color, char)
                x += 1
                square += 1


  
    return board


class simpledialog(object):
    def __init__(self, board:Board):
        self.root=Toplevel(board.frame)
        self.entry = Entry(self.root)
        self.entry.pack()
        if board.activePlayer:
            self.knight_btn = Button(self.root, image=board.images["n"], command=self.knight)
            self.bishop_btn = Button(self.root, image=board.images["b"], command=self.bishop)
            self.rook_btn = Button(self.root, image=board.images["r"], command=self.rook)
            self.queen_btn = Button(self.root, image=board.images["q"], command=self.queen)
        else:
            self.knight_btn = Button(self.root, image=board.images["N"], command=self.knight)
            self.bishop_btn = Button(self.root, image=board.images["B"], command=self.bishop)
            self.rook_btn = Button(self.root, image=board.images["R"], command=self.rook)
            self.queen_btn = Button(self.root, image=board.images["Q"], command=self.queen)
        self.knight_btn.pack()
        self.rook_btn.pack()
        self.bishop_btn.pack()
        self.queen_btn.pack()

        # Modal window.
        # Wait for visibility or grab_set doesn't seem to work.
        self.root.wait_visibility()   # <<< NOTE
        self.root.grab_set()          # <<< NOTE
        self.root.transient(board.frame)   # <<< NOTE

        self.parent = board.frame

    def ok(self):
        self.data = self.entry.get()
        self.root.grab_release()      # <<< NOTE
        self.root.destroy()

    
    def knight(self):
        self.promotionPiece = Piece.Knight.value
        self.ok()
    def bishop(self):
        self.promotionPiece = Piece.Bishop.value
        self.ok()
    def rook(self):
        self.promotionPiece = Piece.Rook.value
        self.ok()
    def queen(self):
        self.promotionPiece = Piece.Queen.value
        self.ok()


if __name__ == '__main__':

    window = Tk()
    images = getImages(80)
    frame = Frame(window)
    frame.grid()
    board = generateBoard(frame, images)
    window.mainloop()
