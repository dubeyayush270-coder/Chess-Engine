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

bool isEnemyPiece(int movingPiece, int targetPiece) {
	if (movingPiece <= WHITE_KING && movingPiece > EMPTY)
	{
		if (targetPiece > WHITE_KING)
		{
			return true;
		}
	}
	else
	{
		if (targetPiece < BLACK_PAWN && targetPiece > EMPTY)
		{
			return true;
		}
	}
	return false;
}
