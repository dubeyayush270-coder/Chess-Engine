#pragma once

struct Move
{
	int fromRow = -1;
	int fromColumn = -1;

	int toRow = -1;
	int toColumn = -1;

	int movedPiece = 0;
	int capturedPiece = 0;


	//States before the move
	bool whiteTurn = true;
	bool gameOver = false;

	int halfMoveClock = 0;

	bool whiteKingMoved = false;
	bool blackKingMoved = false;

	bool whiteKingSideRookMoved = false;
	bool whiteQueenSideRookMoved = false;

	bool blackKingSideRookMoved = false;
	bool blackQueenSideRookMoved = false;

	bool enPassantAvailable = false;
	int enPassantRow = -1;
	int enPassantColumn = -1;

	// Castling information
	bool wasCastling = false;
	int rookFromRow = -1;
	int rookFromColumn = -1;
	int rookToRow = -1;
	int rookToColumn = -1;

	// En Passant capture information
	int enPassantCapturedRow = -1;
	int enPassantCapturedColumn = -1;

	// Promotion
	bool promotionPending = false;
	int promotionRow = -1;
	int promotionColumn = -1;
	int promotedPiece = 0;


	// Notation
	bool givesCheck = false;
	bool givesCheckmate = false;

	// Disambiguation information
	char disambiguationFile = '\0';
	char disambiguationRank = '\0';
};