import tkinter as tk
import cython
from getImages import getImages
from board import Square, Board
from time import sleep
from pieces import King, Piece

from values import DARK, LIGHT

import pyposition
pos = pyposition.PyPosition()
pos.printGameBoard()



def makeButton(frm, board: Board, x, y, char):
    color = LIGHT if (x+y)%2 == 0 else DARK
    btn = tk.Button(frm, bg=color, image=images[char])
    btn.grid(row=y, column=x)
    square = Square(board, btn, x, y, char)
    board.board[(x,y)] = square
    btn["command"] = lambda sq=square: onClick(sq, board)


def onClick(square: Square, board:Board):
    #hideMoves(board.lastMovedPiece, board)
    selected = board.selectedSquare
    if selected:
        start = selected.getEnumSquare()
        end = square.getEnumSquare()
        if (pos.tryMove(start, end)):
        # if (square.x, square.y) in selected.occupyingPiece.moves:
            deselectSquare(board)
            movePiece(selected, square, board)

            board.lastMovedPiece = square.occupyingPiece
            board.lastMovedPiece.ping()

            #play black's move
            move = pos.playRandomLegalMove()
            movePieceFromBackend(move[0], move[1], board)

        else:
            #flash red or something
            deselectSquare(board)
            if board.getPiece(square.x, square.y):
                selectSquare(square, board)
                

    else:
        selectSquare(square, board)


def selectSquare(square, board):
    piece = square.occupyingPiece
    if piece:
        if piece.team == board.activePlayer:
            board.selectedSquare = square
        else:
            #if board.selectedSquare: hideMoves(board.selectedSquare.piece, board)
            board.selectedSquare = square
        #showMoves(piece, board)


def deselectSquare(board):
    #hideMoves(board.selectedSquare.occupyingPiece, board)
    board.selectedSquare = None

def showMoves(piece: Piece, board):
    if piece:
        piece.ping() #inefficient and will probably be removed
        for move in piece.coverage:
            board.getSquare(move).widget["bg"] = "#2a55a1"

def hideMoves(piece, board):
    if piece:
        for move in piece.coverage:
            sq = board.getSquare(move)
            sq.widget["bg"] = sq.color


def movePiece(origin: Square, destination: Square, board: Board):
    #castling
    y = 0 if origin.occupyingPiece.team == "Black" else 7
    castle = type(origin.occupyingPiece) == King and (origin.x, origin.y) == (4,y) and destination in [board.getSquare(1, y), board.getSquare(6, y)]

    if destination.occupyingPiece:
        destination.occupyingPiece.vacate()

    destination.occupyingPiece = origin.occupyingPiece
    destination.widget["image"] = images[origin.occupyingPiece.char]
    board.getSquare(destination.x, destination.y).occupyingPiece = origin.occupyingPiece
    
    origin.occupyingPiece.vacate()
    origin.occupyingPiece = None
    origin.widget["image"] = images['e']
    board.getSquare(origin.x, origin.y).occupyingPiece = None

    destination.occupyingPiece.x = destination.x
    destination.occupyingPiece.y = destination.y

    destination.occupyingPiece.ping() 

    origin.updateAttackers()
    destination.updateAttackers()


    if castle:
        oldRookSquare = 0 if destination.x == 1 else 7
        newRookSquare = 2 if destination.x == 1 else 5
        movePiece(board.getSquare(oldRookSquare, origin.y), board.getSquare(newRookSquare, origin.y), board)

    board.activePlayer = 'White' if board.activePlayer == 'Black' else 'Black'

def movePieceFromBackend(origin: int, destination: int, board: Board):
    origin_y = origin // 8
    origin_x = origin % 8
    destination_y = destination // 8
    destination_x = destination % 8
    print(origin_x, origin_y,destination_x, destination_y)
    if (origin_y < 4):
        origin_y = origin_y + (2 * (3 - origin_y)) + 1
    else:
        origin_y = origin_y - ((2 * (3 - (7 - origin_y))) + 1)

    if (destination_y < 4):
        destination_y = destination_y + (2 * (3 - destination_y)) + 1
    else:
        destination_y = destination_y - ((2 * (3 - (7 - destination_y))) + 1)
    
    # print(origin_x, origin,y,destination_x, destination_y)
    movePiece(board.getSquare(origin_x, origin_y), board.getSquare(destination_x, destination_y), board)


def generateBoard(frm, fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"):  #I'm assuming that the fen will always be valid
    board = Board()
    x = y = 0

    for char in fen:
        try:
            num = int(char)
            while num>0:
                makeButton(frm, board, x, y, 'e')
                num -= 1
                x += 1

        except ValueError:
            if char == '/':
                x = 0
                y += 1

            else:
                makeButton(frm, board, x, y, char)
                x += 1
  
    return board


if __name__ == '__main__':

    window = tk.Tk()
    frame = tk.Frame(window)
    frame.grid()
    images = getImages(80)
    board = generateBoard(frame)

    for y in range(8):
        for x in range(8):
            if board.getPiece(x,y):
                board.getPiece(x, y).ping()

    # for coords in board.board:
    #     if board.board[(coords)].occupyingPiece:
    #         board.board[(coords)].occupyingPiece.getMoves(board)

    
    window.mainloop()