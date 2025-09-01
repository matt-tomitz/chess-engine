from PIL import Image, ImageTk
import os
import sys

def resource_path(relative_path):
    """Get absolute path to resource, works for dev and PyInstaller."""
    if getattr(sys, 'frozen', False):
        # Running in PyInstaller bundle
        base_path = sys._MEIPASS
    else:
        # Running as normal Python script
        base_path = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(base_path, relative_path)

def getImages(size):
    images = {}
    images['P'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/whitepawn.png")).resize((size,size), Image.LANCZOS))
    images['B'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/whitebishop.png")).resize((size,size), Image.LANCZOS))
    images['N'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/whiteknight.png")).resize((size,size), Image.LANCZOS))
    images['R'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/whiterook.png")).resize((size,size), Image.LANCZOS))
    images['Q'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/whitequeen.png")).resize((size,size), Image.LANCZOS))
    images['K'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/whiteking.png")).resize((size,size), Image.LANCZOS))
    images['p'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/blackpawn.png")).resize((size,size), Image.LANCZOS))
    images['b'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/blackbishop.png")).resize((size,size), Image.LANCZOS))
    images['n'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/blackknight.png")).resize((size,size), Image.LANCZOS))
    images['r'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/blackrook.png")).resize((size,size), Image.LANCZOS))
    images['q'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/blackqueen.png")).resize((size,size), Image.LANCZOS))
    images['k'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/blackking.png")).resize((size,size), Image.LANCZOS))
    images['0'] = ImageTk.PhotoImage(Image.open(resource_path("chesspieces/empty.png")).resize((size,size), Image.LANCZOS))
    return images