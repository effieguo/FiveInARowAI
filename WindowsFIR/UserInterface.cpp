#include "UserInterface.h" 
using namespace Gdiplus;

extern GameBoard gGameBoard;

UserInterface::UserInterface(void)
{
	for (int i = 0, y = 7; i < 15; i++, y += 35)
	{
		for (int j = 0, x = 7; j < 15; j++, x += 35)
		{
			gameRect[i][j].X = x;
			gameRect[i][j].Y = y;
			gameRect[i][j].Height = 35;
			gameRect[i][j].Width = 35;
		}
	}
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			needRePaint[i][j] = true;
	rePaint = true;
}

UserInterface::~UserInterface(void)
{
}

void UserInterface::InitUI(void)
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			needRePaint[i][j] = true;
	rePaint = true;
}

void UserInterface::PaintChess(HDC hDC)
{
	Image BlackChess(L"Resources/BlackChess.png");
	Image WhiteChess(L"Resources/WhiteChess.png");
	Image Cross(L"Resources/Cross.png");
	Graphics g(hDC);
	if (gGameBoard.moveHistory.Count > 0)
		needRePaint[gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count - 1].ChessPos.y][gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count - 1].ChessPos.x] = true;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (needRePaint[i][j] == true || rePaint == true)
			{
				if (gGameBoard.boardState[i][j] == BLACK)
					g.DrawImage(&BlackChess, gameRect[i][j]);
				else if (gGameBoard.boardState[i][j] == WHITE)
					g.DrawImage(&WhiteChess, gameRect[i][j]);
				if (gGameBoard.moveHistory.Count >= 0)
				{
					gameRect[gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count].ChessPos.y][gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count].ChessPos.x].Inflate(-13, -13);
					g.DrawImage(&Cross, gameRect[gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count].ChessPos.y][gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count].ChessPos.x]);
					gameRect[gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count].ChessPos.y][gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count].ChessPos.x].Inflate(13, 13);
				}
			}
		}
	}
	if (gGameBoard.moveHistory.Count > 0)
		needRePaint[gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count - 1].ChessPos.y][gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count - 1].ChessPos.x] = false;
}

