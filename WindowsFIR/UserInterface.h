#pragma once
#include <Windows.h>
#include <GdiPlus.h>
#include "ChessBoard.h"
#include "Resources/resource.h"
#include "FIR.h"
using namespace Gdiplus;


class UserInterface
{
public:
	bool needRePaint[15][15];
	bool rePaint;
	ChessMove lastMove;
	Rect gameRect[15][15];
public:
	UserInterface(void);
	~UserInterface(void);

	void InitUI();
	void PaintChess(HDC);
};

