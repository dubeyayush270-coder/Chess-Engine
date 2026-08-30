#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>

#include "ChessPieces.h"
#include "Board.h"
#include "Game.h"
#include "MoveValidation.h"

int board[8][8];

GameState game;

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


int main(int argc, char* argv[])
{
	InitializeBoard(board);
	

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
			if (game.promotionPending)
			{
				if (event.type == SDL_KEYDOWN)
				{
					int promotedPiece = EMPTY;

					if (event.key.keysym.sym == SDLK_q)
					{
						promotedPiece = game.whiteTurn ? WHITE_QUEEN : BLACK_QUEEN;
					}
					else if (event.key.keysym.sym == SDLK_r)
					{
						promotedPiece = game.whiteTurn ? WHITE_ROOK : BLACK_ROOK;
					}
					else if (event.key.keysym.sym == SDLK_b)
					{
						promotedPiece = game.whiteTurn ? WHITE_BISHOP : BLACK_BISHOP;
					}
					else if (event.key.keysym.sym == SDLK_n)
					{
						promotedPiece = game.whiteTurn ? WHITE_KNIGHT : BLACK_KNIGHT;
					}

					if (promotedPiece != EMPTY)
					{
						PromotePawn(game.promotionRow, game.promotionColumn, promotedPiece, board, game);

						std::cout << "Pawn promoted!\n";

						FinishMove(board, game);
					}
				}
			}
			else
			{
				if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_u)
					{
						UndoMove(board, game);

						//Reset Selection
						pieceSelected = false;
						selectedRow = -1;
						selectedColumn = -1;
					}
					else if (event.key.keysym.sym == SDLK_r)
					{
						RedoMove(board, game);

						//Reset Selection
						pieceSelected = false;
						selectedRow = -1;
						selectedColumn = -1;
					}
				}

				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					if (game.gameOver)
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
						else if (game.whiteTurn && IsWhitePiece(piece) || !game.whiteTurn && IsBlackPiece(piece))
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

						else if (isFriendlyPiece(pieceID, piece) && ((game.whiteTurn && IsWhitePiece(piece)) || (!game.whiteTurn && IsBlackPiece(piece))))
						{
							selectedRow = row;
							selectedColumn = column;
							pieceSelected = true;

						}

						else if (IsLegalMove(selectedRow, selectedColumn, row, column, board, game))
						{
							MakeMove(selectedRow, selectedColumn, row, column, board, game);

							if (game.promotionPending)
							{
								std::cout << "Choose promotion piece: Q = Queen, R = Rook, B = Bishop, N = Knight\n";
							}
							else
							{
								FinishMove(board, game);
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