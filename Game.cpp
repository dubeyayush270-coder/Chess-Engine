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

	int capturedPiece = board[toRow][toColumn];

	if (movingPiece == EMPTY)
	{
		return false;
	}

	if (game.whiteTurn && !IsWhitePiece(movingPiece))
	{
		return false;
	}

	if (!game.whiteTurn && !IsBlackPiece(movingPiece))
	{
		return false;
	}


	int enemyKing = IsWhitePiece(movingPiece) ? BLACK_KING : WHITE_KING;

	if (capturedPiece == enemyKing)
	{
		return false;
	}

	if (!IsValidMove(fromRow, fromColumn, toRow, toColumn, movingPiece, board, game))
	{
		return false;
	}

	bool isEnPassant = false;

	if (movingPiece == WHITE_PAWN || movingPiece == BLACK_PAWN) 
	{
		isEnPassant = IsValidEnPassant(fromRow, fromColumn, toRow, toColumn, movingPiece, board, game);
	}

	int enPassantCapturedPiece = EMPTY;

	if (isEnPassant) 
	{
		enPassantCapturedPiece = board[game.enPassantRow][game.enPassantColumn];
		board[game.enPassantRow][game.enPassantColumn] = EMPTY;
	}

	board[toRow][toColumn] = movingPiece;
	board[fromRow][fromColumn] = EMPTY;

	int kingPiece = (IsWhitePiece(movingPiece)) ? WHITE_KING : BLACK_KING;

	bool kingInCheck = IsKingInCheck(board, kingPiece);

	board[fromRow][fromColumn] = movingPiece;
	board[toRow][toColumn] = capturedPiece;

	if (isEnPassant)
	{
		board[game.enPassantRow][game.enPassantColumn] = enPassantCapturedPiece;
	}

	return !kingInCheck;
}

void MakeMove(int fromRow, int fromColumn, int toRow, int toColumn, int board[8][8], GameState& game, bool isRedo)
{
	if (!isRedo)
	{
		game.redoHistory.clear();
	}

	int movingPiece = board[fromRow][fromColumn];
	int capturedPiece = board[toRow][toColumn];


	bool isEnPassant = IsValidEnPassant(fromRow, fromColumn, toRow, toColumn, movingPiece, board, game);

	Move move{};

	move.wasCastling = false;

	move.rookFromRow = -1;
	move.rookFromColumn = -1;
	move.rookToRow = -1;
	move.rookToColumn = -1;

	move.fromRow = fromRow;
	move.fromColumn = fromColumn;

	move.toRow = toRow;
	move.toColumn = toColumn;

	move.movedPiece = movingPiece;
	move.capturedPiece = capturedPiece;

	// Default: not an en passant capture
	move.enPassantCapturedRow = -1;
	move.enPassantCapturedColumn = -1;

	//Save game state before the move
	move.whiteTurn = game.whiteTurn;
	move.gameOver = game.gameOver;

	move.halfMoveClock = game.halfMoveClock;

	move.whiteKingMoved = game.whiteKingMoved;
	move.blackKingMoved = game.blackKingMoved;

	move.whiteKingSideRookMoved = game.whiteKingSideRookMoved;
	move.whiteQueenSideRookMoved = game.whiteQueenSideRookMoved;

	move.blackKingSideRookMoved = game.blackKingSideRookMoved;
	move.blackQueenSideRookMoved = game.blackQueenSideRookMoved;

	move.enPassantAvailable = game.enPassantAvailable;
	move.enPassantRow = game.enPassantRow;
	move.enPassantColumn = game.enPassantColumn;

	move.promotionPending = game.promotionPending;
	move.promotionRow = game.promotionRow;
	move.promotionColumn = game.promotionColumn;

	move.promotedPiece = EMPTY;


	
	if (isEnPassant)
	{
		move.enPassantCapturedRow = game.enPassantRow;
		move.enPassantCapturedColumn = game.enPassantColumn;
	}

	if ((movingPiece == WHITE_KING || movingPiece == BLACK_KING) && std::abs(toColumn - fromColumn) == 2)
	{
		move.wasCastling = true;

		move.rookFromRow = fromRow;

		if (toColumn > fromColumn)
		{
			move.rookFromColumn = 7;
			move.rookToColumn = 5;
		}
		else
		{
			move.rookFromColumn = 0;
			move.rookToColumn = 3;
		}

		move.rookToRow = fromRow;
	}


	// Determine SAN disambiguation while the original board position still exists.
	SetMoveDisambiguation(board, move, game);

	//Store the move
	game.moveHistory.push_back(move);

	bool isPawnMove = movingPiece == WHITE_PAWN || movingPiece == BLACK_PAWN;
	bool isCapture = capturedPiece != EMPTY;

	if (isPawnMove || isCapture)
	{
		game.halfMoveClock = 0;
	}
	else
	{
		game.halfMoveClock++;
	}

	//bool isEnPassant = IsValidEnPassant(fromRow, fromColumn, toRow, toColumn, movingPiece, board, game);
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

	if (capturedPiece == WHITE_ROOK)
	{
		if (toRow == 7 && toColumn == 0)
		{
			game.whiteQueenSideRookMoved = true;
		}
		else if (toRow == 7 && toColumn == 7)
		{
			game.whiteKingSideRookMoved = true;
		}
	}

	if (capturedPiece == BLACK_ROOK)
	{
		if (toRow == 0 && toColumn == 0)
		{
			game.blackQueenSideRookMoved = true;
		}
		else if (toRow == 0 && toColumn == 7)
		{
			game.blackKingSideRookMoved = true;
		}
	}
}


void FinishMove(int board[8][8], GameState& game)
{
	game.whiteTurn = !game.whiteTurn;

	RecordPosition(board, game);

	int sideToMoveKing = game.whiteTurn ? WHITE_KING : BLACK_KING;

	bool givesCheck = IsKingInCheck(board, sideToMoveKing);
	bool givesCheckmate = false;

	if (givesCheck)
	{
		givesCheckmate = IsCheckmate(board, sideToMoveKing, game);
	}

	if (!game.moveHistory.empty())
	{
		game.moveHistory.back().givesCheck = givesCheck;
		game.moveHistory.back().givesCheckmate = givesCheckmate;
	}

	if (givesCheckmate)
	{
		std::cout << "CHECKMATE!\n";

		if (sideToMoveKing == WHITE_KING)
		{
			std::cout << "Black wins!\n";
		}
		else
		{
			std::cout << "White wins!\n";
		}

		game.gameOver = true;
	}
	else if (IsStalemate(board, sideToMoveKing, game))
	{
		std::cout << "It's A Draw.\n";
		game.gameOver = true;
	}
	else if (IsInsufficientMaterial(board))
	{
		std::cout << "Draw by insufficient material.\n";
		game.gameOver = true;
	}
	else if (IsFiftyMoveRule(game))
	{
		std::cout << "Draw by fifty-move rule.\n";
		game.gameOver = true;
	}
	else if (IsThreefoldRepetition(game))
	{
		std::cout << "Draw by threefold repetition.\n";
		game.gameOver = true;
	}
	else 
	{
		if (givesCheck)
		{
			std::cout << "CHECK!\n";
		}

		std::cout << (game.whiteTurn ? "White's turn\n" : "Black's turn\n");

	}
}

void PromotePawn(int row, int column, int promotedPiece, int board[8][8], GameState& game)
{
	int pawn = board[row][column];
	bool promotionSuccessful = false;

	if (pawn == WHITE_PAWN)
	{
		if (promotedPiece == WHITE_QUEEN || promotedPiece == WHITE_ROOK || promotedPiece == WHITE_BISHOP || promotedPiece == WHITE_KNIGHT)
		{
			board[row][column] = promotedPiece;
			promotionSuccessful = true;
		}
	}
	else if (pawn == BLACK_PAWN)
	{
		if (promotedPiece == BLACK_QUEEN || promotedPiece == BLACK_ROOK || promotedPiece == BLACK_BISHOP || promotedPiece == BLACK_KNIGHT)
		{
			board[row][column] = promotedPiece;
			promotionSuccessful = true;
		}
	}

	if (!promotionSuccessful) 
	{
		return;
	}

	if (!game.moveHistory.empty())
	{
		game.moveHistory.back().promotedPiece = promotedPiece;
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
			if ((whiteKing && IsWhitePiece(piece)) || (!whiteKing && IsBlackPiece(piece)))
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

void UndoMove(int board[8][8], GameState& game)
{
	if (game.moveHistory.empty())
	{
		std::cout << "No moves to undo.\n";
		return;
	}

	Move move = game.moveHistory.back();

	board[move.fromRow][move.fromColumn] = move.movedPiece;
	board[move.toRow][move.toColumn] = move.capturedPiece;

	if (move.enPassantCapturedRow != -1)
	{
		int capturedPawn;

		if (move.movedPiece == WHITE_PAWN)
		{
			capturedPawn = BLACK_PAWN;
		}
		else
		{
			capturedPawn = WHITE_PAWN;
		}

		board[move.enPassantCapturedRow][move.enPassantCapturedColumn] = capturedPawn;
	}

	if (move.wasCastling)
	{
		board[move.rookFromRow][move.rookFromColumn] = board[move.rookToRow][move.rookToColumn];
		board[move.rookToRow][move.rookToColumn] = EMPTY;
	}

	game.whiteTurn = move.whiteTurn;
	game.gameOver = move.gameOver;

	game.halfMoveClock = move.halfMoveClock;

	game.whiteKingMoved = move.whiteKingMoved;
	game.blackKingMoved = move.blackKingMoved;

	game.whiteKingSideRookMoved = move.whiteKingSideRookMoved;
	game.whiteQueenSideRookMoved = move.whiteQueenSideRookMoved;

	game.blackKingSideRookMoved = move.blackKingSideRookMoved;
	game.blackQueenSideRookMoved = move.blackQueenSideRookMoved;

	game.enPassantAvailable = move.enPassantAvailable;
	game.enPassantRow = move.enPassantRow;
	game.enPassantColumn = move.enPassantColumn;

	game.promotionPending = move.promotionPending;
	game.promotionRow = move.promotionRow;
	game.promotionColumn = move.promotionColumn;

	if (game.positionHistory.size() > 1)
	{
		game.positionHistory.pop_back();
	}

	game.moveHistory.pop_back();

	game.redoHistory.push_back(move);

	std::cout << "Move undone.\n";
}

void RedoMove(int board[8][8], GameState& game)
{
	if (game.redoHistory.empty())
	{
		std::cout << "No moves to redo.\n";
		return;
	}

	Move move = game.redoHistory.back();
	
	game.redoHistory.pop_back();

	MakeMove(move.fromRow, move.fromColumn, move.toRow, move.toColumn, board, game, true);

	//If there was a promotion
	if (move.promotedPiece != EMPTY)
	{
		PromotePawn(move.toRow, move.toColumn, move.promotedPiece, board, game);
	}

	FinishMove(board, game);

	std::cout << "Move redone.\n";
}

bool IsInsufficientMaterial(int board[8][8])
{
	int nonKingPieces = 0;
	int remainingPiece = EMPTY;

	int bishopCount = 0;
	int firstBishopColor = -1;
	int secondBishopColor = -1;

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			int piece = board[row][column];

			if (piece == EMPTY)
			{
				continue;
			}

			if (piece == WHITE_KING || piece == BLACK_KING)
			{
				continue;
			}

			nonKingPieces++;
			remainingPiece = piece;

			if (piece == WHITE_BISHOP || piece == BLACK_BISHOP)
			{
				int squareColor = (row + column) % 2;

				if (bishopCount == 0)
				{
					firstBishopColor = squareColor;
				}
				else if (bishopCount == 1)
				{
					secondBishopColor = squareColor;
				}

				bishopCount++;
			}

		}
	}

	if (nonKingPieces == 0)
	{
		return true;
	}

	if (nonKingPieces == 1)
	{
		if (remainingPiece == WHITE_BISHOP || remainingPiece == BLACK_BISHOP ||
			remainingPiece == WHITE_KNIGHT || remainingPiece == BLACK_KNIGHT)
		{
			return true;
		}
	}

	if (nonKingPieces == 2 && bishopCount == 2)
	{
		if (firstBishopColor == secondBishopColor)
		{
			return true;
		}
	}

	return false;
}

bool IsFiftyMoveRule(const GameState& game)
{
	return game.halfMoveClock >= 100;
}

bool IsThreefoldRepetition(const GameState& game)
{
	if (game.positionHistory.empty())
	{
		return false;
	}

	const std::string& currentPosition = game.positionHistory.back();

	int count = 0;

	for (const std::string& position : game.positionHistory)
	{
		if (position == currentPosition)
		{
			count++;
		}
	}

	return count >= 3;
}

bool HasLegalEnPassantCapture(int board[8][8], const GameState& game)
{
	if (!game.enPassantAvailable) 
	{
		return false;
	}

	int epRow = game.enPassantRow;
	int epColumn = game.enPassantColumn;

	if (epRow < 0 || epRow >= 8 || epColumn < 0 || epColumn >= 8)
	{
		return false;
	}

	int movingPawn = game.whiteTurn ? WHITE_PAWN : BLACK_PAWN;
	int enemyPawn = game.whiteTurn ? BLACK_PAWN : WHITE_PAWN;

	int direction = game.whiteTurn ? -1 : 1;

	if (board[epRow][epColumn] != enemyPawn)
	{
		return false;
	}

	int destinationRow = epRow + direction;

	if (destinationRow < 0 || destinationRow >= 8)
	{
		return false;
	}

	for (int offset = -1; offset <= 1; offset += 2)
	{
		int fromColumn = epColumn + offset;

		if (fromColumn < 0 || fromColumn >= 8)
		{
			continue;
		}

		if (board[epRow][fromColumn] != movingPawn)
		{
			continue;
		}

		board[epRow][fromColumn] = EMPTY;
		board[epRow][epColumn] = EMPTY;
		board[destinationRow][epColumn] = movingPawn;

		int kingPiece =
			game.whiteTurn ? WHITE_KING : BLACK_KING;

		bool kingInCheck =
			IsKingInCheck(board, kingPiece);

		// Restore board.
		board[epRow][fromColumn] = movingPawn;
		board[epRow][epColumn] = enemyPawn;
		board[destinationRow][epColumn] = EMPTY;

		if (!kingInCheck)
		{
			return true;
		}
	}

	return false;
}

std::string GeneratePositionKey(int board[8][8], const GameState& game) 
{
	std::string key;

	// 1. Board position
	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			key += std::to_string(board[row][column]);
			key += ",";
		}
	}

	// 2. Side to move
	key += game.whiteTurn ? "W" : "B";

	// 3. Castling state
	key += game.whiteKingMoved ? "1" : "0";
	key += game.blackKingMoved ? "1" : "0";

	key += game.whiteKingSideRookMoved ? "1" : "0";
	key += game.whiteQueenSideRookMoved ? "1" : "0";

	key += game.blackKingSideRookMoved ? "1" : "0";
	key += game.blackQueenSideRookMoved ? "1" : "0";

	// 4. En passant state
	bool hasEnPassantCapture = HasLegalEnPassantCapture(board, game);

	key += hasEnPassantCapture ? "1" : "0";

	if (hasEnPassantCapture)
	{
		key += std::to_string(game.enPassantRow);
		key += ",";
		key += std::to_string(game.enPassantColumn);
	}

	return key;
}

void RecordPosition(int board[8][8], GameState& game)
{
	std::string key = GeneratePositionKey(board, game);

	game.positionHistory.push_back(key);
}

std::string SquareToNotation(int row, int column)
{
	char file = 'a' + column;
	char rank = '8' - row;

	std::string square;
	square += file;
	square += rank;

	return square;
}

char PromotionPieceToNotation(int piece)
{
	switch (piece)
	{
	case WHITE_QUEEN:
	case BLACK_QUEEN:
		return 'Q';

	case WHITE_ROOK:
	case BLACK_ROOK:
		return 'R';

	case WHITE_BISHOP:
	case BLACK_BISHOP:
		return 'B';

	case WHITE_KNIGHT:
	case BLACK_KNIGHT:
		return 'N';
	}

	return '\0';
}

std::string MoveToNotation(const Move& move)
{
	std::string notation;

	bool isEnPassant = move.enPassantCapturedRow != -1;

	bool isCapture = (move.capturedPiece != EMPTY || isEnPassant);

	if (move.wasCastling) 
	{
		if (move.toColumn == 6)
		{
			notation = "O-O";
		}
		else if (move.toColumn == 2)
		{
			notation = "O-O-O";
		}
	}
	else if (move.movedPiece == WHITE_PAWN || move.movedPiece == BLACK_PAWN)
	{
		if (isCapture)
		{
			notation += static_cast<char>('a' + move.fromColumn);

			notation += "x";
		}

		notation += SquareToNotation(move.toRow, move.toColumn);

		if (move.promotedPiece != EMPTY)
		{
			char promotionPiece = PromotionPieceToNotation(move.promotedPiece);

			if (promotionPiece != '\0')
			{
				notation += "=";
				notation += promotionPiece;
			}
		}
	}
	else
	{
		switch (move.movedPiece)
		{
		case WHITE_KNIGHT:
		case BLACK_KNIGHT:
			notation += "N";
			break;

		case WHITE_BISHOP:
		case BLACK_BISHOP:
			notation += "B";
			break;

		case WHITE_ROOK:
		case BLACK_ROOK:
			notation += "R";
			break;

		case WHITE_QUEEN:
		case BLACK_QUEEN:
			notation += "Q";
			break;

		case WHITE_KING:
		case BLACK_KING:
			notation += "K";
			break;
		}

		if (move.disambiguationFile != '\0')
		{
			notation += move.disambiguationFile;
		}

		if (move.disambiguationRank != '\0')
		{
			notation += move.disambiguationRank;
		}

		if (isCapture)
		{
			notation += "x";
		}

		notation += SquareToNotation(move.toRow, move.toColumn);
	}

	if (move.givesCheckmate)
	{
		notation += "#";
	}
	else if (move.givesCheck)
	{
		notation += "+";
	}

	return notation;
}

void SetMoveDisambiguation(const int board[8][8], Move& move, GameState& game)
{
	if (move.movedPiece == WHITE_PAWN || move.movedPiece == BLACK_PAWN || move.movedPiece == WHITE_KING || move.movedPiece == BLACK_KING)
	{
		return;
	}

	bool anotherPieceFound = false;
	bool sameFileFound = false;
	bool sameRankFound = false;

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			if (row == move.fromRow && column == move.fromColumn)
			{
				continue;
			}

			if (board[row][column] != move.movedPiece)
			{
				continue;
			}

			int tempBoard[8][8];

			for (int r = 0; r < 8; r++)
			{
				for (int c = 0; c < 8; c++)
				{
					tempBoard[r][c] = board[r][c];
				}
			}

			if(IsLegalMove(row, column, move.toRow, move.toColumn, tempBoard, game))
			{
				anotherPieceFound = true;
				if (column == move.fromColumn)
				{
					sameFileFound = true;
				}

				if (row == move.fromRow)
				{
					sameRankFound = true;
				}
			}
		}
	}

	if (!anotherPieceFound)
	{
		return;
	}

	if (!sameFileFound)
	{
		move.disambiguationFile = static_cast<char>('a' + move.fromColumn);
	}
	else if (!sameRankFound)
	{
		move.disambiguationRank = static_cast<char>('8' - move.fromRow);
	}
	else
	{
		move.disambiguationFile = static_cast<char>('a' + move.fromColumn);

		move.disambiguationRank = static_cast<char>('8' - move.fromRow);
	}
}