#ifndef STANDARDS_HPP
#define STANDARDS_HPP
#include <cstdint>
#include <array>
#include <iterator>
#include <string> 
#include <iostream>
#include "string.h"
#include <vector>

typedef enum Square {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
} square;

const std::string coordinates[] = {
    "a1","b1","c1","d1","e1","f1","g1","h1",
    "a2","b2","c2","d2","e2","f2","g2","h2",
    "a3","b3","c3","d3","e3","f3","g3","h3",
    "a4","b4","c4","d4","e4","f4","g4","h4",
    "a5","b5","c5","d5","e5","f5","g5","h5",
    "a6","b6","c6","d6","e6","f6","g6","h6",
    "a7","b7","c7","d7","e7","f7","g7","h7",
    "a8","b8","c8","d8","e8","f8","g8","h8"
};

enum Piece
{
    White,
    Black,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,

    NUM_PIECES
};

const std::string pieceString[NUM_PIECES] = 
{
    "White",
    "Black",
    "Pawn",
    "Knight",
    "Bishop",
    "Rook",
    "Queen",
    "King",

};

enum AllPieces
{
    P, N, B, R, Q, K,
    p, n, b, r, q, k
};


const char pieceRepresentation[12] = {'P', 'N' , 'B', 'R', 'Q','K', 'p', 'n', 'b', 'r', 'q', 'k'};

const uint64_t AFile = 0x0101010101010101;
const uint64_t BFile = 0x0202020202020202;

const uint64_t notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
const uint64_t notABFile = 0xfcfcfcfcfcfcfcfc; // ~0x303030303030303
const uint64_t notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080
const uint64_t notHGFile = 0x3f3f3f3f3f3f3f3f; // ~0xc0c0c0c0c0c0c0c0

#define WK (1ULL << f1 | 1ULL << g1)
#define WQ (1ULL << b1 | 1ULL << c1 | 1ULL << d1)
#define BK (1ULL << f8 | 1ULL << g8)
#define BQ (1ULL << b8 | 1ULL << c8 | 1ULL << d8)

#endif