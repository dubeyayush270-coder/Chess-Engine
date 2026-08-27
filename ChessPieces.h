#pragma once

enum Piece
{
    EMPTY = 0,

    WHITE_PAWN = 1,
    WHITE_ROOK,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_QUEEN,
    WHITE_KING,

    BLACK_PAWN,
    BLACK_ROOK,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_QUEEN,
    BLACK_KING
};

bool IsWhitePiece(int piece);

bool IsBlackPiece(int piece);

bool isFriendlyPiece(int movingPiece, int targetPiece);