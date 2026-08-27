#include "Game.h"
#include "ChessPieces.h"
#include "Board.h"

#include "MoveValidation.h"

#include <iostream>
#include <cmath>

bool IsLegalMove(int fromRow, int fromColumn, int toRow, int toColumn, int board[8][8], GameState& game)
{
	if (fromRow < 0 || fromRow >= 8 ||
		fromColumn < 0 || fromColumn >= 8 ||
		toRow < 0 || toRow >= 8 ||
		toColumn < 0 || toColumn >= 8)
	{
		return false;
	}

	int movingPiece = board[fromRow][fromColumn];

	if (movingPiece == EMPTY)
	{
		return false;
	}
	int capturedPiece = board[toRow][toColumn];

	if (!IsValidMove(fromRow, fromColumn, toRow, toColumn, movingPiece, board, game))
	{
		return false;
	}

	board[toRow][toColumn] = movingPiece;
	board[fromRow][fromColumn] = EMPTY;

	int kingPiece = (movingPiece > EMPTY && movingPiece < BLACK_PAWN) ? WHITE_KING : BLACK_KING;

	bool kingInCheck = IsKingInCheck(board, kingPiece);

	board[fromRow][fromColumn] = movingPiece;
	board[toRow][toColumn] = capturedPiece;

	return !kingInCheck;
}

void MakeMove(int fromRow, int fromColumn, int toRow, int toColumn, int board[8][8], GameState& game)
{
	int movingPiece = board[fromRow][fromColumn];

	bool isEnPassant = IsValidEnPassant(fromRow, fromColumn, toRow, toColumn, movingPiece, board, game);
	bool createsEnPassant = (movingPiece == WHITE_PAWN || movingPiece == BLACK_PAWN) && std::abs(toRow - fromRow) == 2;

	board[toRow][toColumn] = movingPiece;
	board[fromRow][fromColumn] = EMPTY;

	if (isEnPassant)
	{
		board[game.enPassantRow][game.enPassantColumn] = EMPTY;
	}

	game.enPassantAvailable = false;
	game.enPassantRow = -1;
	game.enPassantColumn = -1;

	if (createsEnPassant)
	{
		game.enPassantAvailable = true;
		game.enPassantRow = toRow;
		game.enPassantColumn = toColumn;
	}

	if (movingPiece == WHITE_PAWN && toRow == 0)
	{
		game.promotionPending = true;
		game.promotionRow = toRow;
		game.promotionColumn = toColumn;
	}
	else if (movingPiece == BLACK_PAWN && toRow == 7)
	{
		game.promotionPending = true;
		game.promotionRow = toRow;
		game.promotionColumn = toColumn;
	}

	if ((movingPiece == WHITE_KING || movingPiece == BLACK_KING) && std::abs(toColumn - fromColumn) == 2)
	{
		if (toColumn > fromColumn)
		{
			board[fromRow][toColumn - 1] = board[fromRow][7];
			board[fromRow][7] = EMPTY;
		}
		else
		{
			board[fromRow][toColumn + 1] = board[fromRow][0];
			board[fromRow][0] = EMPTY;
		}
	}

	if (movingPiece == WHITE_KING)
	{
		game.whiteKingMoved = true;
	}
	else if (movingPiece == BLACK_KING)
	{
		game.blackKingMoved = true;
	}

	if (movingPiece == WHITE_ROOK)
	{
		if (fromRow == 7 && fromColumn == 0)
		{
			game.whiteQueenSideRookMoved = true;
		}
		else if (fromRow == 7 && fromColumn == 7)
		{
			game.whiteKingSideRookMoved = true;
		}
	}
	if (movingPiece == BLACK_ROOK)
	{
		if (fromRow == 0 && fromColumn == 0)
		{
			game.blackQueenSideRookMoved = true;
		}
		else if (fromRow == 0 && fromColumn == 7)
		{
			game.blackKingSideRookMoved = true;
		}
	}
}


void FinishMove(int board[8][8], GameState& game)
{
	game.whiteTurn = !game.whiteTurn;
	int opponentKing = game.whiteTurn ? WHITE_KING : BLACK_KING;

	if (IsCheckmate(board, opponentKing, game))
	{
		std::cout << "CHECKMATE!\n";

		if (opponentKing == WHITE_KING)
		{
			std::cout << "Black wins!\n";
		}
		else
		{
			std::cout << "White wins!\n";
		}

		game.gameOver = true;
	}
	else if (IsStalemate(board, opponentKing, game))
	{
		std::cout << "It's A Draw.\n";
		game.gameOver = true;
	}
	else
	{
		if (IsKingInCheck(board, opponentKing))
		{
			std::cout << "CHECK!\n";
		}

		std::cout << (game.whiteTurn ? "White's turn\n" : "Black's turn\n");

	}
}

void PromotePawn(int row, int column, int promotedPiece, int board[8][8], GameState& game)
{
	int pawn = board[row][column];

	if (pawn == WHITE_PAWN)
	{
		if (promotedPiece == WHITE_QUEEN || promotedPiece == WHITE_ROOK || promotedPiece == WHITE_BISHOP || promotedPiece == WHITE_KNIGHT)
		{
			board[row][column] = promotedPiece;
		}
	}
	else if (pawn == BLACK_PAWN)
	{
		if (promotedPiece == BLACK_QUEEN || promotedPiece == BLACK_ROOK || promotedPiece == BLACK_BISHOP || promotedPiece == BLACK_KNIGHT)
		{
			board[row][column] = promotedPiece;
		}
	}

	game.promotionPending = false;
	game.promotionRow = -1;
	game.promotionColumn = -1;
}

bool IsCheckmate(int board[8][8], int kingPiece, GameState& game)
{

	int kingRow;
	int kingColumn;

	if (!findKing(board, kingPiece, kingRow, kingColumn))
	{
		return false;
	}

	if (!IsKingInCheck(board, kingPiece)) {
		return false;
	}

	bool whiteKing = (kingPiece == WHITE_KING);

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			int piece = board[row][column];
			if (whiteKing && IsWhitePiece(piece) || (!whiteKing && IsBlackPiece(piece)))
			{
				for (int destinationRow = 0; destinationRow < 8; destinationRow++)
				{
					for (int destinationColumn = 0; destinationColumn < 8; destinationColumn++)
					{
						if (IsLegalMove(row, column, destinationRow, destinationColumn, board, game))
						{
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}

bool IsStalemate(int board[8][8], int kingPiece, GameState& game)
{
	int kingRow;
	int kingColumn;

	if (!findKing(board, kingPiece, kingRow, kingColumn))
	{
		return false;
	}

	if (IsKingInCheck(board, kingPiece))
	{
		return false;
	}

	bool whiteKing = (kingPiece == WHITE_KING);

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			int piece = board[row][column];
			if (whiteKing && IsWhitePiece(piece) || (!whiteKing && IsBlackPiece(piece)))
			{
				for (int destinationRow = 0; destinationRow < 8; destinationRow++)
				{
					for (int destinationColumn = 0; destinationColumn < 8; destinationColumn++)
					{
						if (IsLegalMove(row, column, destinationRow, destinationColumn, board, game))
						{
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}