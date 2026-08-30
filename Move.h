#pragma once

struct Move
{
	int fromRow;
	int fromColumn;

	int toRow;
	int toColumn;

	int movedPiece;
	int capturedPiece;


	//States before the move
	bool whiteTurn;
	bool gameOver;

	bool whiteKingMoved;
	bool blackKingMoved;

	bool whiteKingSideRookMoved;
	bool whiteQueenSideRookMoved;

	bool blackKingSideRookMoved;
	bool blackQueenSideRookMoved;

	bool enPassantAvailable;
	int enPassantRow;
	int enPassantColumn;

	// Castling information
	bool wasCastling;
	int rookFromRow;
	int rookFromColumn;
	int rookToRow;
	int rookToColumn;

	// En Passant capture information
	int enPassantCapturedRow;
	int enPassantCapturedColumn;

	bool promotionPending;
	int promotionRow;
	int promotionColumn;

	int promotedPiece;

	int halfMoveClock;
};