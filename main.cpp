#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>

const int EMPTY = 0;

const int WHITE_PAWN       = 1;
const int WHITE_ROOK       = 2;
const int WHITE_KNIGHT     = 3;
const int WHITE_BISHOP     = 4;
const int WHITE_QUEEN      = 5;
const int WHITE_KING       = 6;

const int BLACK_PAWN       = 7;
const int BLACK_ROOK       = 8;
const int BLACK_KNIGHT     = 9;
const int BLACK_BISHOP     = 10;
const int BLACK_QUEEN      = 11;
const int BLACK_KING       = 12;

int board[8][8] = {
	{BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK},
	{BLACK_PAWN, BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN,  BLACK_PAWN, BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN},
	{EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY},
	{EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY},
	{EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY},
	{EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY},
	{WHITE_PAWN, WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,  WHITE_PAWN, WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN},
	{WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}
};

// variables for selected pieces (Game-state variables)

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

// variables for selected pieces (Game-state variables)

const int knightMoves[8][2] = { {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{ 1,2 },{ 2,-1 },{ 2,1 } };

const int kingMoves[8][2] = { {-1, -1},{-1,  0},{-1,  1},{ 0, -1},{ 0,  1},{ 1, -1},{ 1,  0},{ 1,  1} };


SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* filename) {

	SDL_Surface* surface = IMG_Load(filename);
	if (surface == nullptr) {
		std::cout << "failed to load image :" << filename << std::endl;
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	if (texture == nullptr)
	{
		std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
	}
	
	SDL_FreeSurface(surface);

	return texture;
}

bool IsValidCastle(int fromRow, int fromColumn, int toRow, int toColumn, int pieceID, int board[8][8]);
bool IsValidEnPassant(int fromRow, int fromColumn, int toRow, int toColumn, int PieceID, int board[8][8]);
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
	if (selectedRow == startingRow && (destinationRow - selectedRow) == 2*direction && destinationColumn == selectedColumn && board[selectedRow + direction][selectedColumn] == EMPTY && board[destinationRow][destinationColumn] == EMPTY) {
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

bool IsValidMove(int selectedRow, int selectedColumn, int destinationRow, int destinationColumn, int pieceID, int board[8][8]) 
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
		if (IsValidEnPassant(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board))
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
			return IsValidCastle(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board);
		}
		return IsValidKingMove(selectedRow, selectedColumn, destinationRow, destinationColumn, pieceID, board);
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

bool IsLegalMove(int fromRow, int fromColumn, int toRow, int toColumn, int board[8][8])
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

	if (!IsValidMove(fromRow, fromColumn, toRow, toColumn, movingPiece, board))
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

void MakeMove(int fromRow,int fromColumn,int toRow,int toColumn,int board[8][8])
{
	int movingPiece = board[fromRow][fromColumn];

	bool isEnPassant = IsValidEnPassant(fromRow, fromColumn, toRow, toColumn, movingPiece, board);
	bool createsEnPassant = (movingPiece == WHITE_PAWN || movingPiece == BLACK_PAWN) && std::abs(toRow - fromRow) == 2;

	board[toRow][toColumn] = movingPiece;
	board[fromRow][fromColumn] = EMPTY;

	if (isEnPassant)
	{
		board[enPassantRow][enPassantColumn] = EMPTY;
	}

	enPassantAvailable = false;
	enPassantRow = -1;
	enPassantColumn = -1;

	if (createsEnPassant)
	{
		enPassantAvailable = true;
		enPassantRow = toRow;
		enPassantColumn = toColumn;
	}

	if (movingPiece == WHITE_PAWN && toRow == 0)
	{
		promotionPending = true;
		promotionRow = toRow;
		promotionColumn = toColumn;
	}
	else if (movingPiece == BLACK_PAWN && toRow == 7)
	{
		promotionPending = true;
		promotionRow = toRow;
		promotionColumn = toColumn;
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
		whiteKingMoved = true;
	}
	else if (movingPiece == BLACK_KING)
	{
		blackKingMoved = true;
	}

	if (movingPiece == WHITE_ROOK)
	{
		if (fromRow == 7 && fromColumn == 0)
		{
			whiteQueenSideRookMoved = true;
		}
		else if (fromRow == 7 && fromColumn == 7)
		{
			whiteKingSideRookMoved = true;
		}
	}
	if (movingPiece == BLACK_ROOK)
	{
		if (fromRow == 0 && fromColumn == 0)
		{
			blackQueenSideRookMoved = true;
		}
		else if (fromRow == 0 && fromColumn == 7)
		{
			blackKingSideRookMoved = true;
		}
	}
}

bool IsCheckmate(int board[8][8], int kingPiece)
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
						if (IsLegalMove(row, column, destinationRow, destinationColumn, board))
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

bool IsStalemate(int board[8][8], int kingPiece)
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
						if (IsLegalMove(row, column, destinationRow, destinationColumn, board))
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

bool IsValidCastle(int fromRow, int fromColumn, int toRow, int toColumn, int pieceID, int board[8][8])
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

	if (pieceID == WHITE_KING && whiteKingMoved)
	{
		return false;
	}

	if (pieceID == BLACK_KING && blackKingMoved)
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
			if (whiteKingSideRookMoved || board[7][7] != WHITE_ROOK)
			{
				return false;
			}
		}
		else if(queenSide)
		{
			if (whiteQueenSideRookMoved || board[7][0] != WHITE_ROOK)
			{
				return false;
			}
		}
	}
	else
	{
		if (kingSide)
		{
			if (blackKingSideRookMoved ||
				board[0][7] != BLACK_ROOK)
			{
				return false;
			}
		}
		else if (queenSide)
		{
			if (blackQueenSideRookMoved ||
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

bool IsValidEnPassant(int fromRow, int fromColumn, int toRow, int toColumn, int PieceID, int board[8][8])
{
	
	if (!enPassantAvailable)
	{
		return false;
	}

	if (PieceID != WHITE_PAWN && PieceID != BLACK_PAWN)
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

	if (PieceID == WHITE_PAWN)
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

	if (enPassantRow != fromRow || enPassantColumn != toColumn)
	{
		return false;
	}

	int capturedPawn = board[enPassantRow][enPassantColumn];

	if (PieceID == WHITE_PAWN && capturedPawn != BLACK_PAWN)
	{
		return false;
	}

	if (PieceID == BLACK_PAWN && capturedPawn != WHITE_PAWN)
	{
		return false;
	}

	return true;
}

void PromotePawn(int row, int column, int promotedPiece, int board[8][8])
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

	promotionPending = false;
	promotionRow = -1;
	promotionColumn = -1;
}

void FinishMove(int board[8][8])
{
	whiteTurn = !whiteTurn;
	int opponentKing = whiteTurn ? WHITE_KING : BLACK_KING;

	if (IsCheckmate(board, opponentKing))
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

		gameOver = true;
	}
	else if (IsStalemate(board, opponentKing))
	{
		std::cout << "It's A Draw.\n";
		gameOver = true;
	}
	else
	{
		if (IsKingInCheck(board, opponentKing))
		{
			std::cout << "CHECK!\n";
		}

		std::cout << (whiteTurn ? "White's turn\n" : "Black's turn\n");

	}
}

int main(int argc, char* argv[])
{
	// This will initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL Initialization Failed : " << SDL_GetError() << std::endl;
		return -1;
	}

	// Create Window
	//std::cout << "Creating Window...\n";
	SDL_Window* window = SDL_CreateWindow(
		"Ayush's Chess Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		800,
		SDL_WINDOW_SHOWN
	);
	//std::cout << "Window Created Successfully!\n";

	if (window == nullptr)
	{
		std::cout << "Window Creation Failed : " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// Creating Renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		std::cout << "Renderer could not be created! SDL_error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Code For Square

	SDL_Rect square;

	// Code For Square

	// Creating texture Array

	SDL_Texture* pieceTextures[13];
	for (int i = 0; i < 13; i++) {
		pieceTextures[i] = nullptr;
	}

	pieceTextures[WHITE_PAWN]   = LoadTexture(renderer, "assets/images/white_pawn.png");
	pieceTextures[WHITE_ROOK]   = LoadTexture(renderer, "assets/images/white_rook.png");
	pieceTextures[WHITE_KNIGHT] = LoadTexture(renderer, "assets/images/white_knight.png");
	pieceTextures[WHITE_BISHOP] = LoadTexture(renderer, "assets/images/white_bishop.png");
	pieceTextures[WHITE_QUEEN]  = LoadTexture(renderer, "assets/images/white_queen.png");
	pieceTextures[WHITE_KING]   = LoadTexture(renderer, "assets/images/white_king.png");
	pieceTextures[BLACK_PAWN]   = LoadTexture(renderer, "assets/images/black_pawn.png");
	pieceTextures[BLACK_ROOK]   = LoadTexture(renderer, "assets/images/black_rook.png");
	pieceTextures[BLACK_KNIGHT] = LoadTexture(renderer, "assets/images/black_knight.png");
	pieceTextures[BLACK_BISHOP] = LoadTexture(renderer, "assets/images/black_bishop.png");
	pieceTextures[BLACK_QUEEN]  = LoadTexture(renderer, "assets/images/black_queen.png");
	pieceTextures[BLACK_KING]   = LoadTexture(renderer, "assets/images/black_king.png");

	// Creating texture Array
	
	
	bool pieceSelected = false;
	int selectedRow = -1;
	int selectedColumn = -1;

	bool running = true;
	SDL_Event event;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}

			// PROMOTION INPUT
			if (promotionPending)
			{
				if (event.type == SDL_KEYDOWN)
				{
					int promotedPiece = EMPTY;

					if (event.key.keysym.sym == SDLK_q)
					{
						promotedPiece = whiteTurn ? WHITE_QUEEN : BLACK_QUEEN;
					}
					else if (event.key.keysym.sym == SDLK_r)
					{
						promotedPiece = whiteTurn ? WHITE_ROOK : BLACK_ROOK;
					}
					else if (event.key.keysym.sym == SDLK_b)
					{
						promotedPiece = whiteTurn ? WHITE_BISHOP : BLACK_BISHOP;
					}
					else if (event.key.keysym.sym == SDLK_n)
					{
						promotedPiece = whiteTurn ? WHITE_KNIGHT : BLACK_KNIGHT;
					}

					if (promotedPiece != EMPTY)
					{
						PromotePawn(promotionRow, promotionColumn, promotedPiece, board);

						std::cout << "Pawn promoted!\n";

						FinishMove(board);
					}
				}
			}
			else
			{
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					if (gameOver)
					{
						std::cout << "Game is over!\n";
						return 0;
					}
					std::cout << "X = " << event.button.x << std::endl;
					std::cout << "Y = " << event.button.y << std::endl;

					int column = event.button.x / 100;
					int row = event.button.y / 100;
					std::cout << "Row = " << row << std::endl;
					std::cout << "Column = " << column << std::endl;

					int piece = board[row][column];

					if (!pieceSelected) {
						if (piece == EMPTY) {
							std::cout << "Empty square selected" << std::endl;
						}
						else if (whiteTurn && IsWhitePiece(piece) || !whiteTurn && IsBlackPiece(piece))
						{
							pieceSelected = true;
							selectedRow = row;
							selectedColumn = column;
						}
						else
						{
							std::cout << "Not your turn\n";
						}
					}
					else {
						int pieceID = board[selectedRow][selectedColumn];
						if (selectedRow == row && selectedColumn == column)
						{
							std::cout << "Deselecting piece\n";

							pieceSelected = false;
							selectedRow = -1;
							selectedColumn = -1;

						}

						else if (isFriendlyPiece(pieceID, piece) && ((whiteTurn && IsWhitePiece(piece)) || (!whiteTurn && IsBlackPiece(piece))))
						{
							selectedRow = row;
							selectedColumn = column;
							pieceSelected = true;

						}

						else if (IsLegalMove(selectedRow, selectedColumn, row, column, board))
						{
							MakeMove(selectedRow, selectedColumn, row, column, board);

							if (promotionPending)
							{
								std::cout << "Choose promotion piece: Q = Queen, R = Rook, B = Bishop, N = Knight\n";
							}
							else
							{
								FinishMove(board);
							}

							pieceSelected = false;
							selectedColumn = -1;
							selectedRow = -1;
						}
						else
						{
							std::cout << "ILLEGAL MOVE\n";
						}
					}
				}
			}
		}

		// Clear the Screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//Draw the chess Board
		
		for (int row = 0; row < 8; row++) 
		{
			for (int column = 0; column < 8; column++) 
			{
				square.x = column * 100;
				square.y = row * 100;
				square.w = 100;
				square.h = 100;

				if ((row + column) % 2 == 0) 
				{
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				}
				else 
				{
					SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
				}
				SDL_RenderFillRect(renderer, &square);
			}

		}

		//Draw the chess Board
		
		
		// For creating pieces
		
		for (int row = 0; row < 8; row++) {
			for (int column = 0; column < 8; column++) {
				int pieceID = board[row][column];
				if (pieceID == EMPTY) {
					continue;
				}
				SDL_Rect destination{};

				destination.x = column * 100;
				destination.y = row * 100;
				destination.w = 100;
				destination.h = 100;
				SDL_RenderCopy(renderer, pieceTextures[pieceID], nullptr, &destination);
			}
		}
		
		// For creating pieces

		// Display Everything
		SDL_RenderPresent(renderer);
		// Display Everything
	}

	for (int i = 1; i < 13; i++) {
		SDL_DestroyTexture(pieceTextures[i]);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Closing Game...\n";
	return 0;
}