#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include "UserInterface.h"
#include "HistoryHeuristic.h"
#include "ArtIntel.h"
#include "ChessBoard.h"
#include "Resources/resource.h"

#pragma comment(lib, "winmm.lib") 
using namespace Gdiplus;
using namespace std;


LRESULT CALLBACK GameProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

void GameInit(int, Chess);

INT_PTR CALLBACK NewGame(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE gWinAppInstance;

// Global variables for game
GameBoard gGameBoard;
ArtificialIntelligence gAI;

// 2D Graphics GUI with WinGDI+
UserInterface gUI;

int depth = 3;
Chess turn = BLACK;

ChessMove Move;

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	HWND HWnd;
	WNDCLASSEX Winclass;
	MSG msg;
	gWinAppInstance = hInstance;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	HBITMAP Board = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BOARD));
	HBRUSH BgHBR = CreatePatternBrush(Board);


	Winclass.cbSize = sizeof(WNDCLASSEX);
	Winclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	Winclass.lpfnWndProc = GameProc;
	Winclass.cbClsExtra = 0;
	Winclass.cbWndExtra = 0;
	Winclass.hInstance = hInstance;
	Winclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONF));
	Winclass.hCursor = LoadCursor(NULL, IDC_HAND);
	Winclass.hbrBackground = BgHBR;
	Winclass.lpszClassName = L"WinClassFIR";
	Winclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	Winclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONF));

	RegisterClassEx(&Winclass);

	HWnd = CreateWindowEx(
		NULL,
		L"WinClassFIR",
		L"Windows Five In A Row",
		WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX&~WS_SIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		545,
		585,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	GameInit(3, BLACK);

	// Set default search as Alpha Beta
	gAI.searchMethod = ArtificialIntelligence::AlphaBeta;

	UpdateWindow(HWnd);
	ShowWindow(HWnd, SW_SHOWNORMAL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);

	return msg.wParam;
}

bool CheckIsGameOver(HWND hWnd)
{
	LPCWSTR Result;

	if (gAI.IsGameOver((Chess)!gGameBoard.turn) != 0)
	{
		if (gGameBoard.turn == BLACK)
			Result = L"Black wins!";
		else
			Result = L"White wins!";
		if (IDOK == MessageBox(hWnd, Result, L"One more game?", MB_OKCANCEL))
		{
			GameInit(depth, turn);
			RECT Refresh;
			GetClientRect(hWnd, &Refresh);
			InvalidateRect(hWnd, &Refresh, true);
			return false;
		}
		else
			return true;
	}

	return false;
}

void MakeMoveAndDrawBoard(HWND hWnd, HDC hDC, const Coordinate& MovePos, Chess chs)
{
	gGameBoard.PlaceStone(MovePos, chs);
	gUI.PaintChess(hDC);
	PlaySound(MAKEINTRESOURCE(IDR_PUT), gWinAppInstance, SND_RESOURCE | SND_ASYNC);

	if (CheckIsGameOver(hWnd))
		PostQuitMessage(0);
}

LRESULT CALLBACK GameProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	HDC hDC;
	PAINTSTRUCT Ps;
	POINTS MousePos;
	int wmId, wmEvent;
	switch (uMsg)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &Ps);
		gUI.rePaint = true;
		gUI.PaintChess(hDC);
		EndPaint(hWnd, &Ps);
		break;
	case WM_LBUTTONDOWN:
		MousePos = MAKEPOINTS(lParam);
		Coordinate MovePos = GameBoard::MousePosToMovePos(MousePos.x, MousePos.y);

		// Place the stone on the board
		if (gGameBoard.IsMoveEmpty(MovePos))
		{
			hDC = GetDC(hWnd);
			gUI.rePaint = false;

			MakeMoveAndDrawBoard(hWnd, hDC, MovePos, gGameBoard.turn);

			gAI.AIMakeMove((Chess)!gGameBoard.turn, ArtificialIntelligence::Depth, -ArtificialIntelligence::Infinity, ArtificialIntelligence::Infinity);

			MakeMoveAndDrawBoard(hWnd, hDC, MoveGenerator::BestMove.ChessPos, (Chess)!gGameBoard.turn);

			ReleaseDC(hWnd, hDC);
		}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_NEWG:
			DialogBox(gWinAppInstance, MAKEINTRESOURCE(IDD_NEWG), hWnd, NewGame);
			break;
		case IDM_RETRACT:
			if (gGameBoard.moveHistory.Count < 1)
				MessageBox(hWnd, L"You haven't placed any rock yet.", L"Error", MB_OK);
			else
			{
				gGameBoard.boardState[gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count].ChessPos.y][gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count].ChessPos.x] = NONE;
				gGameBoard.boardState[gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count - 1].ChessPos.y][gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count - 1].ChessPos.x] = NONE;
				gUI.needRePaint[gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count].ChessPos.y][gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count].ChessPos.x] = true;
				gUI.needRePaint[gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count - 1].ChessPos.y][gGameBoard.moveHistory.Move[gGameBoard.moveHistory.Count - 1].ChessPos.x] = true;

				gGameBoard.moveHistory.Count -= 2;
				RECT Refresh;
				GetClientRect(hWnd, &Refresh);
				InvalidateRect(hWnd, &Refresh, true);
			}
			break;
		case IDM_ABOUT:
			DialogBox(gWinAppInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			if (gGameBoard.moveHistory.Count < 1 || IDYES == MessageBox(hWnd, L"Are you sure you want to exit this game?", L"Game not over yet.", MB_YESNO))
				DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

void GameInit(int depth, Chess turn)
{
	gGameBoard.turn = turn;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
			gGameBoard.boardState[i][j] = NONE;
	}
	if (gGameBoard.turn == BLACK)
		gGameBoard.moveHistory.Count = -1;
	else if (gGameBoard.turn == WHITE)
	{
		gGameBoard.boardState[7][7] = BLACK;
		gGameBoard.moveHistory.Count = 0;
		gGameBoard.moveHistory.Move[0].ChessPos.x = 7;
		gGameBoard.moveHistory.Move[0].ChessPos.y = 7;
		PlaySound(MAKEINTRESOURCE(IDR_PUT), gWinAppInstance, SND_RESOURCE | SND_ASYNC);
	}
	ArtificialIntelligence::Depth = depth;
	gUI.InitUI();
}

BOOL CALLBACK NewGame(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	int wmId, wmEvent;
	switch (message)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_CHOOSEB, IDC_CHOOSEW, IDC_CHOOSEB);
		CheckRadioButton(hDlg, IDC_DIFE, IDC_DIFF, IDC_DIFN);
		CheckRadioButton(hDlg, IDC_MINMAX, IDC_ALPHABETA, IDC_ALPHABETA);
		return TRUE;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDOK:
			if (gGameBoard.moveHistory.Count < 1 || IDYES == MessageBox(hDlg, L"Are you sure you want to exit this game?", L"Game not over yet.", MB_YESNO))
			{
				if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_CHOOSEB))
					turn = BLACK;
				else
					turn = WHITE;
				if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_DIFE))
					depth = 2;
				else if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_DIFN))
					depth = 3;
				else if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_DIFD))
					depth = 4;

				if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_MINMAX))
					gAI.searchMethod = ArtificialIntelligence::MinMax;
				else if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_ALPHABETA))
					gAI.searchMethod = ArtificialIntelligence::AlphaBeta;

				GameInit(depth, turn);
				RECT Refresh;
				GetClientRect(GetParent(hDlg), &Refresh);
				InvalidateRect(GetParent(hDlg), &Refresh, true);
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}