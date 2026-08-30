#include "Board.h"

void InitializeBoard(int board[8][8])
{
	//int initialBoard[8][8] = {
	//{BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK},
	//{BLACK_PAWN, BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN,  BLACK_PAWN, BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN},
	//{EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY},
	//{EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY},
	//{EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY},
	//{EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY},
	//{WHITE_PAWN, WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,  WHITE_PAWN, WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN},
	//{WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}
	//};
	
	//THIS IS A TEST BOARD
	int initialBoard[8][8] = {
		{EMPTY,      EMPTY,        EMPTY,        EMPTY,			   EMPTY,      EMPTY,        EMPTY,        BLACK_KING},
		{EMPTY,      EMPTY,        EMPTY,        EMPTY,			   WHITE_QUEEN, EMPTY,       EMPTY,        EMPTY},
		{EMPTY,      EMPTY,        EMPTY,        EMPTY,            EMPTY,      EMPTY,        EMPTY,        EMPTY},
		{EMPTY,      EMPTY,        EMPTY,        EMPTY,            EMPTY,      EMPTY,        EMPTY,        EMPTY},
		{EMPTY,      EMPTY,        EMPTY,        EMPTY,			   EMPTY,      EMPTY,        EMPTY,        EMPTY},
		{EMPTY,      EMPTY,        EMPTY,        EMPTY,            EMPTY,      EMPTY,        EMPTY,        EMPTY},
		{EMPTY,      EMPTY,        EMPTY,        EMPTY,            EMPTY,      EMPTY,        EMPTY,        EMPTY},
		{EMPTY,      EMPTY,        EMPTY,        WHITE_KING,       EMPTY,      EMPTY,        WHITE_ROOK,        EMPTY},
	};

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			board[row][column] = initialBoard[row][column];
		}
	}
}