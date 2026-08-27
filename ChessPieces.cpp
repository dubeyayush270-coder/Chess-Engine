#include "ChessPieces.h"


bool IsWhitePiece(int piece)
{
	return piece >= WHITE_PAWN && piece <= WHITE_KING;
}

bool IsBlackPiece(int piece)
{
	return piece >= BLACK_PAWN && piece <= BLACK_KING;
}

bool isFriendlyPiece(int movingPiece, int targetPiece)
{
	if (IsWhitePiece(movingPiece) && IsWhitePiece(targetPiece))
	{
		return true;
	}

	if (IsBlackPiece(movingPiece) && IsBlackPiece(targetPiece))
	{
		return true;
	}

	return false;
}
