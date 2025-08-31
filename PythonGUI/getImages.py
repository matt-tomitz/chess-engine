from PIL import Image, ImageTk
def getImages(size):
    images = {}
    images['P'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/whitepawn.png").resize((size,size), Image.LANCZOS))
    images['B'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/whitebishop.png").resize((size,size), Image.LANCZOS))
    images['N'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/whiteknight.png").resize((size,size), Image.LANCZOS))
    images['R'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/whiterook.png").resize((size,size), Image.LANCZOS))
    images['Q'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/whitequeen.png").resize((size,size), Image.LANCZOS))
    images['K'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/whiteking.png").resize((size,size), Image.LANCZOS))
    images['p'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/blackpawn.png").resize((size,size), Image.LANCZOS))
    images['b'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/blackbishop.png").resize((size,size), Image.LANCZOS))
    images['n'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/blackknight.png").resize((size,size), Image.LANCZOS))
    images['r'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/blackrook.png").resize((size,size), Image.LANCZOS))
    images['q'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/blackqueen.png").resize((size,size), Image.LANCZOS))
    images['k'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/blackking.png").resize((size,size), Image.LANCZOS))
    images['e'] = ImageTk.PhotoImage(Image.open("PythonGUI/chesspieces/empty.png").resize((size,size), Image.LANCZOS))
    return images