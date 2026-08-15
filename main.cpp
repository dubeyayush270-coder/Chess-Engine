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

bool isFriendlyPiece(int movingPiece, int targetPiece) 
{
	if (movingPiece <= WHITE_KING && movingPiece > EMPTY) 
	{
		if (targetPiece <= WHITE_KING && targetPiece > EMPTY) 
		{
			return true;
		}
	}
	else
	{
		if (targetPiece > WHITE_KING) 
		{
			return true;
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
const int knightMoves[8][2] = { {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{ 1,2 },{ 2,-1 },{ 2,1 } };

const int kingMoves[8][2] = { {-1, -1},{-1,  0},{-1,  1},{ 0, -1},{ 0,  1},{ 1, -1},{ 1,  0},{ 1,  1} };

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

//bool IsValidMove (int fromRow,int fromColumn,int toRow,int toColumn,int board[8][8])
//{
//	int piece = board[fromRow][fromColumn];
//
//	if (piece == EMPTY)
//	{
//		return false;
//	}
//	else if (piece == WHITE_PAWN || piece == BLACK_PAWN)
//	{
//		if (!IsValidPawnMove(fromRow, fromColumn, toRow, toColumn, piece, board))
//		{
//			return false;
//		}
//	}
//	else if (piece == WHITE_KNIGHT || piece == BLACK_KNIGHT)
//	{
//		if (!IsValidKnightMove(fromRow, fromColumn, toRow, toColumn, piece, board))
//		{
//			return false;
//		}
//	}
//	else if (piece == WHITE_BISHOP || piece == BLACK_BISHOP)
//	{
//		if (!IsValidBishopMove(fromRow, fromColumn, toRow, toColumn, piece, board))
//		{
//			return false;
//		}
//	}
//	else if (piece == WHITE_ROOK || piece == BLACK_ROOK)
//	{
//		if (!IsValidRookMove(fromRow, fromColumn, toRow, toColumn, piece, board))
//		{
//			return false;
//		}
//	}
//	else if (piece == WHITE_QUEEN || piece == BLACK_QUEEN)
//	{
//		if (!IsValidQueenMove(fromRow, fromColumn, toRow, toColumn, piece, board))
//		{
//			return false;
//		}
//	}
//	else if (piece == WHITE_KING || piece == BLACK_KING)
//	{
//		if (!IsValidKingMove(fromRow, fromColumn, toRow, toColumn, piece, board))
//		{
//			return false;
//		}
//	}
//
//	return true;
//}

void MakeMove(int fromRow,int fromColumn,int toRow,int toColumn,int board[8][8])
{
	board[toRow][toColumn] = board[fromRow][fromColumn];
	board[fromRow][fromColumn] = EMPTY;
}

bool IsWhitePiece(int piece)
{
	return piece >= WHITE_PAWN && piece <= WHITE_KING;
}

bool IsBlackPiece(int piece)
{
	return piece >= BLACK_PAWN && piece <= BLACK_KING;
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
	
	// variables for selected pieces
	bool whiteTurn = true;

	bool pieceSelected = false;
	int selectedRow = -1;
	int selectedColumn = -1;

	// variables for selected pieces

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

			if (event.type == SDL_MOUSEBUTTONDOWN) {
				std::cout << "X = " << event.button.x <<std::endl;
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

						whiteTurn = !whiteTurn;

						std::cout << (whiteTurn ? "White's turn\n" : "Black's turn\n");

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