#include "MoveValidation.h"

#include <cmath>


const int knightMoves[8][2] = { {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{ 1,2 },{ 2,-1 },{ 2,1 } };

const int kingMoves[8][2] = { {-1, -1},{-1,  0},{-1,  1},{ 0, -1},{ 0,  1},{ 1, -1},{ 1,  0},{ 1,  1} };


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



bool findKing(int board[8][8], int kingPiece, int& kingRow, int& kingColumn)
{
	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			if (board[row][column] == kingPiece)
			{
				kingRow = row;
				kingColumn = column;
				return true;
			}
		}
	}
	return false;
}



bool IsValidPawnMove(int selectedRow, int selectedColumn, int destinationRow, int destinationColumn, int pieceID, int board[8][8])
{
	int direction;
	int startingRow;

	// check the pawn color
	if (pieceID == WHITE_PAWN) {
		direction = -1;
		startingRow = 6;
	}
	else {
		direction = 1;
		startingRow = 1;
	}
	// For single square
	if ((destinationRow - selectedRow) == direction && destinationColumn == selectedColumn && board[destinationRow][destinationColumn] == EMPTY) {
		return true;
	}

	// For double squares
	if (selectedRow == startingRow && (destinationRow - selectedRow) == 2 * direction && destinationColumn == selectedColumn && board[selectedRow + direction][selectedColumn] == EMPTY && board[destinationRow][destinationColumn] == EMPTY) {
		return true;
	}

	// For diagonal capture
	if ((destinationRow - selectedRow) == direction && std::abs(destinationColumn - selectedColumn) == 1 && isEnemyPiece(pieceID, board[destinationRow][destinationColumn])) {
		return true;
	}
	return false;
}

bool IsValidRookMove(int selectedRow, int selectedColumn, int destinationRow, int destinationColumn, int pieceID, int board[8][8])
{
	if (selectedColumn == destinationColumn && selectedRow == destinationRow) {
		return false;
	}
	if (selectedColumn == destinationColumn) {
		int step = (destinationRow > selectedRow) ? 1 : -1;
		for (int row = selectedRow + step; row != destinationRow; row += step) {
			if (board[row][destinationColumn] != EMPTY) {
				return false;
			}
		}
	}
	else if (selectedRow == destinationRow) {
		int step = (destinationColumn > selectedColumn) ? 1 : -1;
		for (int column = selectedColumn + step; column != destinationColumn; column += step) {
			if (board[destinationRow][column] != EMPTY) {
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	int targetPiece = board[destinationRow][destinationColumn];

	if (targetPiece == EMPTY) {
		return true;
	}

	if (isEnemyPiece(pieceID, targetPiece)) {
		return true;
	}

	return false;
}

bool IsValidBishopMove(int selectedRow, int selectedColumn, int destinationRow, int destinationColumn, int pieceID, int board[8][8])
{
	if (selectedColumn < 0 || selectedRow < 0 || destinationColumn < 0 || destinationRow < 0 ||
		selectedColumn >= 8 || selectedRow >= 8 || destinationColumn >= 8 || destinationRow >= 8)
	{
		return false;
	}

	if (selectedRow == destinationRow &&
		selectedColumn == destinationColumn)
	{
		return false;
	}

	if (std::abs(selectedColumn - destinationColumn) != std::abs(selectedRow - destinationRow))
	{
		return false;
	}

	int rowStep = (destinationRow > selectedRow) ? 1 : -1;
	int columnStep = (destinationColumn > selectedColumn) ? 1 : -1;

	int row = selectedRow + rowStep;
	int column = selectedColumn + columnStep;

	while (row != destinationRow && column != destinationColumn)
	{
		if (board[row][column] != EMPTY)
		{
			return false;
		}
		row += rowStep;
		column += columnStep;
	}

	int targetPiece = board[destinationRow][destinationColumn];

	if (targetPiece == EMPTY)
	{
		return true;
	}

	if (isEnemyPiece(pieceID, targetPiece))
	{
		return true;
	}

	return false;
}



bool IsValidKnightMove(int selectedRow, int selectedColumn, int destinationRow, int destinationColumn, int pieceID, int board[8][8])
{
	int rowDifference = std::abs(destinationRow - selectedRow);
	int columnDifference = std::abs(destinationColumn - selectedColumn);

	if (!((rowDifference == 1 && columnDifference == 2) || (rowDifference == 2 && columnDifference == 1)))
	{
		return false;
	}

	int targetPiece = board[destinationRow][destinationColumn];

	if (targetPiece == EMPTY)
	{
		return true;
	}

	if (isEnemyPiece(pieceID, targetPiece))
	{
		return true;
	}

	return false;

}

bool IsValidQueenMove(int selectedRow, int selectedColumn, int destinationRow, int destinationColumn, int pieceID, int board[8][8])
{
	if (IsValidRookMove(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board))
	{
		return true;
	}

	if (IsValidBishopMove(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board))
	{
		return true;
	}

	return false;
}

bool IsValidKingMove(int selectedRow, int selectedColumn, int destinationRow, int destinationColumn, int pieceID, int board[8][8])
{
	int enemyKing = (pieceID == WHITE_KING) ? BLACK_KING : WHITE_KING;

	if (board[destinationRow][destinationColumn] == enemyKing)
	{
		return false;
	}

	int rowDifference = std::abs(destinationRow - selectedRow);
	int columnDifference = std::abs(destinationColumn - selectedColumn);

	if (rowDifference > 1 || columnDifference > 1)
	{
		return false;
	}

	if (rowDifference == 0 && columnDifference == 0)
	{
		return false;
	}

	int targetPiece = board[destinationRow][destinationColumn];

	if (targetPiece == EMPTY)
	{
		return true;
	}

	if (isEnemyPiece(pieceID, targetPiece))
	{
		return true;
	}

	return false;
}


bool IsKingInCheck(int board[8][8], int kingPiece)
{
	int kingRow;
	int kingColumn;

	if (!findKing(board, kingPiece, kingRow, kingColumn))
	{
		return false;
	}

	if (kingPiece == WHITE_KING)
	{
		if (kingRow > 0)
		{
			if (kingColumn > 0)
			{
				if (board[kingRow - 1][kingColumn - 1] == BLACK_PAWN)
				{
					return true;
				}
			}
			if (kingColumn < 7)
			{
				if (board[kingRow - 1][kingColumn + 1] == BLACK_PAWN)
				{
					return true;
				}
			}
		}
	}
	else
	{
		if (kingRow < 7)
		{
			if (kingColumn > 0)
			{
				if (board[kingRow + 1][kingColumn - 1] == WHITE_PAWN)
				{
					return true;
				}
			}
			if (kingColumn < 7)
			{
				if (board[kingRow + 1][kingColumn + 1] == WHITE_PAWN)
				{
					return true;
				}
			}
		}
	}

	int enemyKnight = (kingPiece == WHITE_KING) ? BLACK_KNIGHT : WHITE_KNIGHT;

	for (int i = 0; i < 8; i++)
	{
		int row = kingRow + knightMoves[i][0];
		int column = kingColumn + knightMoves[i][1];

		if (row >= 0 && row < 8 &&
			column >= 0 && column < 8)
		{
			if (board[row][column] == enemyKnight)
			{
				return true;
			}
		}
	}

	int enemyRook = (kingPiece == WHITE_KING) ? BLACK_ROOK : WHITE_ROOK;

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			if (board[row][column] == enemyRook)
			{
				if (IsValidRookMove(row, column, kingRow, kingColumn, enemyRook, board))
				{
					return true;
				}
			}
		}
	}

	int enemyBishop = (kingPiece == WHITE_KING) ? BLACK_BISHOP : WHITE_BISHOP;

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			if (board[row][column] == enemyBishop)
			{
				if (IsValidBishopMove(row, column, kingRow, kingColumn, enemyBishop, board))
				{
					return true;
				}
			}
		}
	}

	int enemyQueen = (kingPiece == WHITE_KING) ? BLACK_QUEEN : WHITE_QUEEN;

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			if (board[row][column] == enemyQueen)
			{
				if (IsValidQueenMove(row, column, kingRow, kingColumn, enemyQueen, board))
				{
					return true;
				}
			}
		}
	}

	int enemyKing = (kingPiece == WHITE_KING) ? BLACK_KING : WHITE_KING;

	for (int i = 0; i < 8; i++)
	{
		int row = kingRow + kingMoves[i][0];
		int column = kingColumn + kingMoves[i][1];

		if (row >= 0 && row < 8 &&
			column >= 0 && column < 8)
		{
			if (board[row][column] == enemyKing)
			{
				return true;
			}
		}
	}

	return false;
}


bool IsValidMove(int selectedRow, int selectedColumn, int destinationRow, int destinationColumn, int pieceID, int board[8][8], GameState& game)
{

	if (selectedRow < 0 || selectedRow >= 8 ||
		selectedColumn < 0 || selectedColumn >= 8 ||
		destinationRow < 0 || destinationRow >= 8 ||
		destinationColumn < 0 || destinationColumn >= 8)
	{
		return false;
	}


	switch (pieceID)
	{
	case WHITE_PAWN:
	case BLACK_PAWN:
		if (IsValidEnPassant(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board, game))
		{
			return true;
		}
		return IsValidPawnMove(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board);
	case WHITE_ROOK:
	case BLACK_ROOK:
		return IsValidRookMove(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board);
	case WHITE_BISHOP:
	case BLACK_BISHOP:
		return IsValidBishopMove(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board);
	case WHITE_KNIGHT:
	case BLACK_KNIGHT:
		return IsValidKnightMove(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board);
	case WHITE_QUEEN:
	case BLACK_QUEEN:
		return IsValidQueenMove(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board);
	case WHITE_KING:
	case BLACK_KING:
		if (abs(destinationColumn - selectedColumn) == 2) {
			return IsValidCastle(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board, game);
		}
		return IsValidKingMove(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board);
	}

	return false;
}

bool IsValidCastle(int fromRow, int fromColumn, int toRow, int toColumn, int pieceID, int board[8][8],GameState& game)
{
	if (pieceID != WHITE_KING && pieceID != BLACK_KING)
	{
		return false;
	}

	bool kingSide = (toColumn > fromColumn);
	bool queenSide = (toColumn < fromColumn);

	if (std::abs(toColumn - fromColumn) != 2)
	{
		return false;
	}

	if (pieceID == WHITE_KING && game.whiteKingMoved)
	{
		return false;
	}

	if (pieceID == BLACK_KING && game.blackKingMoved)
	{
		return false;
	}

	if (IsKingInCheck(board, pieceID))
	{
		return false;
	}

	if (pieceID == WHITE_KING)
	{
		if (kingSide)
		{
			if (game.whiteKingSideRookMoved || board[7][7] != WHITE_ROOK)
			{
				return false;
			}
		}
		else if (queenSide)
		{
			if (game.whiteQueenSideRookMoved || board[7][0] != WHITE_ROOK)
			{
				return false;
			}
		}
	}
	else
	{
		if (kingSide)
		{
			if (game.blackKingSideRookMoved ||
				board[0][7] != BLACK_ROOK)
			{
				return false;
			}
		}
		else if (queenSide)
		{
			if (game.blackQueenSideRookMoved ||
				board[0][0] != BLACK_ROOK)
			{
				return false;
			}
		}
	}

	if (pieceID == WHITE_KING)
	{
		if (kingSide)
		{
			if (board[7][5] != EMPTY || board[7][6] != EMPTY)
			{
				return false;
			}
		}
		else if (queenSide)
		{
			if (board[7][1] != EMPTY || board[7][2] != EMPTY || board[7][3] != EMPTY)
			{
				return false;
			}
		}
	}
	else
	{
		if (kingSide)
		{
			if (board[0][5] != EMPTY || board[0][6] != EMPTY)
			{
				return false;
			}
		}
		else if (queenSide)
		{
			if (board[0][1] != EMPTY || board[0][2] != EMPTY || board[0][3] != EMPTY)
			{
				return false;
			}
		}
	}

	int passColumn;

	if (kingSide)
	{
		passColumn = fromColumn + 1;
	}
	else
	{
		passColumn = fromColumn - 1;
	}

	int capturedPiece = board[fromRow][passColumn];

	board[fromRow][passColumn] = pieceID;
	board[fromRow][fromColumn] = EMPTY;

	bool kingPassesThroughCheck = IsKingInCheck(board, pieceID);

	board[fromRow][fromColumn] = pieceID;
	board[fromRow][passColumn] = capturedPiece;

	if (kingPassesThroughCheck)
	{
		return false;
	}

	int capturedDestination = board[toRow][toColumn];

	board[toRow][toColumn] = pieceID;
	board[fromRow][fromColumn] = EMPTY;

	bool kingEndsInCheck = IsKingInCheck(board, pieceID);

	board[fromRow][fromColumn] = pieceID;
	board[toRow][toColumn] = capturedDestination;

	if (kingEndsInCheck)
	{
		return false;
	}

	return true;
}

bool IsValidEnPassant(int fromRow, int fromColumn, int toRow, int toColumn, int pieceID, int board[8][8], GameState& game)
{

	if (!game.enPassantAvailable)
	{
		return false;
	}

	if (pieceID != WHITE_PAWN && pieceID != BLACK_PAWN)
	{
		return false;
	}

	if (board[toRow][toColumn] != EMPTY)
	{
		return false;
	}

	if (std::abs(toColumn - fromColumn) != 1)
	{
		return false;
	}

	int direction;

	if (pieceID == WHITE_PAWN)
	{
		direction = -1;
	}
	else
	{
		direction = 1;
	}

	if (toRow - fromRow != direction)
	{
		return false;
	}

	if (game.enPassantRow != fromRow || game.enPassantColumn != toColumn)
	{
		return false;
	}

	int capturedPawn = board[game.enPassantRow][game.enPassantColumn];

	if (pieceID == WHITE_PAWN && capturedPawn != BLACK_PAWN)
	{
		return false;
	}

	if (pieceID == BLACK_PAWN && capturedPawn != WHITE_PAWN)
	{
		return false;
	}

	return true;
}