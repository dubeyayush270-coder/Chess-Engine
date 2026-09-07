#pragma once
#include <vector>
#include <string>
#include "Move.h"

struct GameState
{
	

	bool whiteTurn = true;
	bool gameOver = false;

	bool whiteKingMoved = false;
	bool blackKingMoved = false;

	bool whiteKingSideRookMoved = false;
	bool whiteQueenSideRookMoved = false;

	bool blackKingSideRookMoved = false;
	bool blackQueenSideRookMoved = false;

	bool enPassantAvailable = false;
	int enPassantRow = -1;
	int enPassantColumn = -1;

	bool promotionPending = false;
	int promotionRow = -1;
	int promotionColumn = -1;

	int halfMoveClock = 0;

	std::vector<Move> moveHistory;

	std::vector<Move> redoHistory;

	std::vector<std::string> positionHistory;
};

bool IsLegalMove(int fromRow, int fromColumn, int toRow, int toColumn, int board[8][8], GameState& game);

void MakeMove(int fromRow, int fromColumn, int toRow, int toColumn, int board[8][8], GameState& game, bool isRedo = false);

void FinishMove(int board[8][8], GameState& game);

void PromotePawn(int row, int column, int promotedPiece, int board[8][8], GameState& game);

bool IsCheckmate(int board[8][8], int kingPiece, GameState& game);

bool IsStalemate(int board[8][8], int kingPiece, GameState& game);

void UndoMove(int board[8][8], GameState& game);

void RedoMove(int board[8][8], GameState& game);

bool IsInsufficientMaterial(int board[8][8]);

bool IsFiftyMoveRule(const GameState& game);

bool IsThreefoldRepetition(const GameState& game);

std::string GeneratePositionKey(int board[8][8], const GameState& game);

void RecordPosition(int board[8][8], GameState& game);