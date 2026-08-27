#pragma once

#include "ChessPieces.h"
#include "Game.h"

bool isEnemyPiece(int pieceID, int targetPiece);

bool findKing(int board[8][8], int kingPiece, int& kingRow, int& kingColumn);

bool IsValidPawnMove(
    int selectedRow,
    int selectedColumn,
    int destinationRow,
    int destinationColumn,
    int pieceID,
    int board[8][8]
);

bool IsValidRookMove(
    int selectedRow,
    int selectedColumn,
    int destinationRow,
    int destinationColumn,
    int pieceID,
    int board[8][8]
);

bool IsValidBishopMove(
    int selectedRow,
    int selectedColumn,
    int destinationRow,
    int destinationColumn,
    int pieceID,
    int board[8][8]
);

bool IsValidKnightMove(
    int selectedRow,
    int selectedColumn,
    int destinationRow,
    int destinationColumn,
    int pieceID,
    int board[8][8]
);

bool IsValidQueenMove(
    int selectedRow,
    int selectedColumn,
    int destinationRow,
    int destinationColumn,
    int pieceID,
    int board[8][8]
);

bool IsValidKingMove(
    int selectedRow,
    int selectedColumn,
    int destinationRow,
    int destinationColumn,
    int pieceID,
    int board[8][8]
);

bool IsValidMove(
    int selectedRow,
    int selectedColumn,
    int destinationRow,
    int destinationColumn,
    int pieceID,
    int board[8][8],
    GameState& game
);

bool IsValidCastle(
    int fromRow,
    int fromColumn,
    int toRow,
    int toColumn,
    int pieceID,
    int board[8][8],
    GameState& game
);

bool IsValidEnPassant(
    int fromRow,
    int fromColumn,
    int toRow,
    int toColumn,
    int PieceID,
    int board[8][8],
    GameState& game
);

bool IsKingInCheck(int board[8][8], int kingPiece);