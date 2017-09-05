#include "Evaluation.h"
#include "ChessBoard.h"
#include <string>

extern GameBoard gGameBoard;

int Evaluation::Count = 0;

Evaluation::Evaluation(void)
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			for (int k = 0; k < 4; k++)
				InitType[i][j][k] = UnAnalysised;

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 7; j++)
			InitCount[i][j] = 0;
	for (int i = 0; i < 15; i++)
		InitLine[i] = UnAnalysised;
}

Evaluation::~Evaluation(void)
{
}

int Evaluation::Eval(Chess chess)
{
	Score = 0;
	Evaluation::Count++;
	memcpy(TypeRecord, InitType, sizeof(EvaType) * 900);
	memcpy(TotalCount, InitCount, sizeof(int) * 14);

	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 15; x++)
		{
			if (gGameBoard.boardState[y][x] != NONE)
			{
				if (TypeRecord[y][x][0] == UnAnalysised)
					AnalysisHorizon(y, x);
				if (TypeRecord[y][x][1] == UnAnalysised)
					AnalysisVertical(y, x);
				if (TypeRecord[y][x][2] == UnAnalysised)
					AnalysisLeft(y, x);
				if (TypeRecord[y][x][3] == UnAnalysised)
					AnalysisRight(y, x);
			}
		}
	}

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				if (gGameBoard.boardState[i][j] == BLACK)
				{
					switch (TypeRecord[i][j][k])
					{
					case Five:
						TotalCount[BLACK][Five]++;
						break;
					case FreeFour:
						TotalCount[BLACK][FreeFour]++;
						break;
					case SleepFour:
						TotalCount[BLACK][SleepFour]++;
						break;
					case FreeThree:
						TotalCount[BLACK][FreeThree]++;
						break;
					case SleepThree:
						TotalCount[BLACK][SleepThree]++;
						break;
					case FreeTwo:
						TotalCount[BLACK][FreeTwo]++;
						break;
					case SleepTwo:
						TotalCount[BLACK][SleepTwo]++;
						break;
					}
				}
				else if (gGameBoard.boardState[i][j] == WHITE)
				{
					switch (TypeRecord[i][j][k])
					{
					case Five:
						TotalCount[WHITE][Five]++;
						break;
					case FreeFour:
						TotalCount[WHITE][FreeFour]++;
						break;
					case SleepFour:
						TotalCount[WHITE][SleepFour]++;
						break;
					case FreeThree:
						TotalCount[WHITE][FreeThree]++;
						break;
					case SleepThree:
						TotalCount[WHITE][SleepThree]++;
						break;
					case FreeTwo:
						TotalCount[WHITE][FreeTwo]++;
						break;
					case SleepTwo:
						TotalCount[WHITE][SleepTwo]++;
						break;
					}
				}
			}
		}
	}


	if (TotalCount[BLACK][SleepFour] > 1)
		TotalCount[BLACK][FreeFour]++;
	if (TotalCount[WHITE][SleepFour] > 1)
		TotalCount[WHITE][FreeFour]++;

	if (chess == BLACK)
	{
		if (TotalCount[WHITE][FreeThree] > 1 &&
			TotalCount[BLACK][FreeThree] == 0 &&
			TotalCount[BLACK][SleepThree] == 0 &&
			TotalCount[BLACK][SleepFour] == 0)
			Score = -91000;
		if (TotalCount[BLACK][FreeThree] > 0 && TotalCount[WHITE][SleepFour] == 0)
			Score = 93000;
		if (TotalCount[WHITE][SleepFour] > 0 && TotalCount[WHITE][FreeThree] > 0)
			Score = -95000;
		if (TotalCount[WHITE][FreeFour] > 0)
			Score = -99000;
		if (TotalCount[BLACK][SleepFour] > 0)
			Score = 97000;
		if (TotalCount[BLACK][FreeFour] > 0)
			Score = 99000;
		if (TotalCount[BLACK][Five] > 0)
			Score = 200000;
		if (TotalCount[WHITE][Five] > 0)
			Score = -200000;
	}
	else if (chess == WHITE)
	{
		if (TotalCount[BLACK][FreeThree] > 1 &&
			TotalCount[WHITE][FreeThree] == 0 &&
			TotalCount[WHITE][SleepThree] == 0 &&
			TotalCount[WHITE][SleepFour] == 0)
			Score = 91000;
		if (TotalCount[WHITE][FreeThree] > 0 && TotalCount[BLACK][SleepFour] == 0)
			Score = -93000;
		if (TotalCount[BLACK][SleepFour] > 0 && TotalCount[BLACK][FreeThree] > 0)
			Score = 95000;
		if (TotalCount[BLACK][FreeFour] > 0)
			Score = 99000;
		if (TotalCount[WHITE][SleepFour] > 0)
			Score = -97000;
		if (TotalCount[WHITE][FreeFour] > 0)
			Score = -99000;
		if (TotalCount[WHITE][Five] > 0)
			Score = -200000;
		if (TotalCount[BLACK][Five] > 0)
			Score = 200000;
	}

	if (TotalCount[BLACK][FreeThree] > 1)
		Score += TotalCount[BLACK][FreeThree] * 2000;
	else if (TotalCount[BLACK][FreeThree] == 1)
		Score += TotalCount[BLACK][FreeThree] * 1400;
	if (TotalCount[WHITE][FreeThree] > 1)
		Score -= TotalCount[WHITE][FreeThree] * 2000;
	else if (TotalCount[WHITE][FreeThree] == 1)
		Score -= TotalCount[WHITE][FreeThree] * 1400;

	Score += (TotalCount[BLACK][Five] - TotalCount[WHITE][Five]) * 150000 + (TotalCount[BLACK][FreeFour] - TotalCount[WHITE][FreeFour]) * 4000 +
		(TotalCount[BLACK][SleepFour] - TotalCount[WHITE][SleepFour]) * 1600 + (TotalCount[BLACK][SleepThree] - TotalCount[WHITE][SleepThree]) * 200 +
		(TotalCount[BLACK][FreeTwo] - TotalCount[WHITE][FreeTwo]) * 150 + (TotalCount[BLACK][SleepTwo] - TotalCount[WHITE][SleepTwo]) * 20;

	return Score;
}

void Evaluation::AnalysisHorizon(int y, int x)
{
	memcpy(TempLine, gGameBoard.boardState[y], 15 * sizeof(Chess));
	AnalysisLine(TempLine, 15, x);
	for (int i = 0; i < 15; i++)
	{
		if (LineRecord[i] != UnAnalysised)
			TypeRecord[y][i][0] = LineRecord[i];
	}
	return;
}

void Evaluation::AnalysisVertical(int y, int x)
{
	for (int i = 0; i < 15; i++)
		TempLine[i] = gGameBoard.boardState[i][x];
	AnalysisLine(TempLine, 15, y);
	for (int i = 0; i < 15; i++)
	{
		if (LineRecord[i] != UnAnalysised)
			TypeRecord[i][x][1] = LineRecord[i];
	}
	return;
}

void Evaluation::AnalysisLeft(int y, int x)
{
	int X, Y, k;
	if (x >= y)
	{
		Y = 0;
		X = x - y;
	}
	else
	{
		X = 0;
		Y = y - x;
	}
	for (k = 0; k < 15; k++)
	{
		if (X + k > 14 || Y + k > 14)
			break;
		TempLine[k] = gGameBoard.boardState[Y + k][X + k];
	}
	AnalysisLine(TempLine, k, x - X);
	for (int i = 0; i < k; i++)
	{
		if (LineRecord[i] != UnAnalysised)
			TypeRecord[Y + i][X + i][2] = LineRecord[i];
	}
	return;
}

void Evaluation::AnalysisRight(int y, int x)
{
	int X, Y, k;
	if (14 - x < y)
	{
		Y = 14;
		X = x - 14 + y;
	}
	else
	{
		X = 0;
		Y = x + y;
	}
	for (k = 0; k < 15; k++)
	{
		if (X + k > 14 || Y - k < 0)
			break;
		TempLine[k] = gGameBoard.boardState[Y - k][X + k];
	}
	AnalysisLine(TempLine, k, x - X);
	for (int i = 0; i < k; i++)
	{
		if (LineRecord[i] != UnAnalysised)
			TypeRecord[Y - i][X + i][3] = LineRecord[i];
	}
	return;
}

void Evaluation::AnalysisLine(Chess AnaLine[], int Length, int ChessPos)
{
	if (Length < 5)
	{
		for (int i = 0; i < Length; i++)
			LineRecord[i] = Analysised;
	}

	Chess ChessType = AnaLine[ChessPos];
	int LeftEdge = ChessPos, RightEdge = ChessPos;
	int LeftRange, RightRange;
	int GridNum = Length - 1;
	memcpy(LineRecord, InitLine, sizeof(EvaType) * 15);

	while (LeftEdge > 0)
	{
		if (AnaLine[LeftEdge - 1] != ChessType)
			break;
		LeftEdge--;
	}
	while (RightEdge < GridNum)
	{
		if (AnaLine[RightEdge + 1] != ChessType)
			break;
		RightEdge++;
	}

	LeftRange = LeftEdge; RightRange = RightEdge;

	while (LeftRange > 0)
	{
		if (AnaLine[LeftRange - 1] == !ChessType)
			break;
		LeftRange--;
	}

	while (RightRange < GridNum)
	{
		if (AnaLine[RightRange + 1] == !ChessType)
			break;
		RightRange++;
	}

	if (RightRange - LeftRange < 4)
	{
		for (int k = LeftRange; k <= RightRange; k++)
			LineRecord[k] = Analysised;
		return;
	}

	for (int k = LeftEdge; k <= RightEdge; k++)
		LineRecord[k] = Analysised;


	if (RightEdge - LeftEdge == 4)
	{
		LineRecord[ChessPos] = Five;
		return;
	}

	if (RightEdge - LeftEdge == 3)
	{
		bool LeftFour = false;
		if (LeftEdge > 0)
		{
			if (AnaLine[LeftEdge - 1] == NONE)
				LeftFour = true;
		}

		if (RightEdge < GridNum)
		{
			if (AnaLine[RightEdge + 1] == NONE)
			{
				if (LeftFour == true)
					LineRecord[ChessPos] = FreeFour;
				else
					LineRecord[ChessPos] = SleepFour;
			}
			else
			{
				if (LeftFour == true)
					LineRecord[ChessPos] = SleepFour;
			}

		}
		else
		{
			if (LeftFour == true)
				LineRecord[ChessPos] = SleepFour;
		}
		return;
	}

	if (RightEdge - LeftEdge == 2)
	{
		bool LeftThree = false;
		if (LeftEdge > 0)
		{
			if (AnaLine[LeftEdge - 1] == NONE)
			{
				if (LeftEdge > 1 && AnaLine[LeftEdge - 2] == AnaLine[LeftEdge])
				{
					LineRecord[LeftEdge] = SleepFour;
					LineRecord[LeftEdge - 2] = Analysised;
				}
				else
					LeftThree = true;
			}
		}

		if (RightEdge < GridNum)
		{
			if (AnaLine[RightEdge + 1] == NONE)
			{
				if (RightEdge < GridNum - 1 && AnaLine[RightEdge + 2] == AnaLine[RightEdge])
				{
					LineRecord[RightEdge] = SleepFour;
					LineRecord[RightEdge + 2] = Analysised;

				}
				else
				{
					if (LeftThree == true)
						LineRecord[RightEdge] = FreeThree;
					else
						LineRecord[RightEdge] = SleepThree;
				}
			}
			else
			{
				if (LineRecord[LeftEdge] == SleepFour)
					return;
				if (LeftThree == true)
					LineRecord[ChessPos] = SleepThree;
			}

		}
		else
		{
			if (LineRecord[LeftEdge] == SleepFour)
				return;
			if (LeftThree == true)
				LineRecord[ChessPos] = SleepThree;

		}
		return;
	}

	if (RightEdge - LeftEdge == 1)
	{
		bool LeftTwo = false;
		if (LeftEdge > 0)
		{
			if (AnaLine[LeftEdge - 1] == NONE)
			{
				if (LeftEdge > 2)
				{
					if (AnaLine[LeftEdge - 2] == AnaLine[LeftEdge])
					{
						if (AnaLine[LeftEdge - 3] == AnaLine[LeftEdge])
						{
							LineRecord[LeftEdge - 3] = Analysised;
							LineRecord[LeftEdge - 2] = Analysised;
							LineRecord[LeftEdge] = SleepFour;
						}
						else if (AnaLine[LeftEdge - 3] == NONE)
						{
							LineRecord[LeftEdge - 2] = Analysised;
							if (RightEdge != RightRange)
								LineRecord[LeftEdge] = FreeThree;
							else
								LineRecord[LeftEdge] = SleepThree;
						}
					}
					else if (AnaLine[LeftEdge - 2] == NONE)
					{
						if (AnaLine[LeftEdge - 3] == AnaLine[LeftEdge])
						{
							LineRecord[LeftEdge - 3] = Analysised;
							LineRecord[LeftEdge] = SleepThree;
						}
					}
				}
				LeftTwo = true;
			}
		}
		if (RightEdge < GridNum)
		{
			if (AnaLine[RightEdge + 1] == NONE)
			{
				if (RightEdge + 2 < GridNum)
				{
					if (AnaLine[RightEdge + 2] == AnaLine[RightEdge])
					{
						if (AnaLine[RightEdge + 3] == AnaLine[RightEdge])
						{
							LineRecord[RightEdge + 3] = Analysised;
							LineRecord[RightEdge + 2] = Analysised;
							LineRecord[RightEdge] = SleepFour;
						}
						else if (AnaLine[RightEdge + 3] == NONE)
						{
							LineRecord[RightEdge + 2] = Analysised;
							if (LeftTwo == true)
								LineRecord[RightEdge] = FreeThree;
							else
								LineRecord[RightEdge] = SleepThree;
						}
					}
					else if (AnaLine[RightEdge + 2] == NONE)
					{
						if (AnaLine[RightEdge + 3] == AnaLine[RightEdge])
						{
							LineRecord[RightEdge + 3] = Analysised;
							LineRecord[RightEdge] = SleepThree;
						}
					}
				}
				if (LineRecord[LeftEdge] == SleepFour ||
					LineRecord[LeftEdge] == SleepThree ||
					LineRecord[LeftEdge] == FreeThree ||
					LineRecord[RightEdge] == SleepFour ||
					LineRecord[RightEdge] == SleepThree ||
					LineRecord[RightEdge] == FreeThree)
					return;

				if (LeftTwo == true)
					LineRecord[ChessPos] = FreeTwo;
				else
					LineRecord[ChessPos] = SleepTwo;
			}
			else
			{
				if (LineRecord[LeftEdge] == SleepFour ||
					LineRecord[LeftEdge] == SleepThree)
					return;
				if (LeftTwo == true)
					LineRecord[ChessPos] = SleepTwo;
			}
		}
		return;
	}

	if (LeftEdge == RightEdge)
	{
		bool LeftOne = false;
		if (ChessPos > 1)
		{
			if (AnaLine[ChessPos - 1] == NONE)
			{
				if (AnaLine[ChessPos - 2] == AnaLine[ChessPos])
				{
					if (ChessPos > 2)
					{
						if (AnaLine[ChessPos - 3] == NONE)
						{
							if (ChessPos > 3)
							{
								if (AnaLine[ChessPos - 4] == AnaLine[ChessPos])
								{
									LineRecord[ChessPos] = SleepThree;
									LineRecord[ChessPos - 2] = Analysised;
									LineRecord[ChessPos - 4] = Analysised;
								}
								else if (ChessPos < GridNum && AnaLine[ChessPos + 1] == NONE)
									LineRecord[ChessPos] = FreeTwo;
								else
									LineRecord[ChessPos] = SleepTwo;
								LineRecord[ChessPos - 2] = Analysised;
							}
							else if (ChessPos < GridNum && AnaLine[ChessPos + 1] == NONE)
								LineRecord[ChessPos] = SleepTwo;
							else
								LineRecord[ChessPos] = SleepTwo;
							LineRecord[ChessPos - 2] = Analysised;
						}
						else if (ChessPos < GridNum && AnaLine[ChessPos + 1] == NONE && AnaLine[ChessPos - 3] != AnaLine[ChessPos])
						{
							LineRecord[ChessPos] = SleepTwo;
							LineRecord[ChessPos - 2] = Analysised;
						}
					}
					else if (ChessPos < GridNum && AnaLine[ChessPos + 1] == NONE)
					{
						LineRecord[ChessPos] = SleepTwo;
						LineRecord[ChessPos - 2] = Analysised;
					}
				}
				else if (AnaLine[ChessPos - 2] == NONE)
				{
					if (ChessPos > 2)
					{
						if (AnaLine[ChessPos - 3] == AnaLine[ChessPos])
						{
							if (ChessPos > 3)
							{
								if (AnaLine[ChessPos - 4] == NONE && ChessPos < GridNum && AnaLine[ChessPos + 1] == NONE)
								{
									LineRecord[ChessPos] = FreeTwo;
									LineRecord[ChessPos - 3] = Analysised;
								}
								else if (ChessPos < GridNum && AnaLine[ChessPos + 1] == NONE)
								{
									LineRecord[ChessPos] = SleepTwo;
									LineRecord[ChessPos - 3] = Analysised;
								}
							}
							else if (ChessPos < GridNum && AnaLine[ChessPos + 1] == NONE)
							{
								LineRecord[ChessPos] = SleepTwo;
								LineRecord[ChessPos - 3] = Analysised;
							}
						}
					}
				}
				LeftOne = true;
			}
		}
		if (ChessPos + 1 < GridNum)
		{
			if (AnaLine[ChessPos + 1] == NONE)
			{
				if (AnaLine[ChessPos + 2] == AnaLine[ChessPos])
				{
					if (ChessPos > 2 && (LineRecord[ChessPos] == SleepThree ||
						LineRecord[ChessPos] == FreeTwo ||
						(LineRecord[ChessPos] == SleepTwo && AnaLine[ChessPos - 3] == NONE)))
						LineRecord[ChessPos + 2] = SleepThree;
					else if (ChessPos + 2 < GridNum)
					{
						if (AnaLine[ChessPos + 3] == NONE)
						{
							if (ChessPos + 3 < GridNum && AnaLine[ChessPos + 4] == AnaLine[ChessPos])
							{
								LineRecord[ChessPos] = SleepThree;
								LineRecord[ChessPos + 2] = Analysised;
								LineRecord[ChessPos + 4] = Analysised;
							}
							else if (LeftOne == true)
							{
								if (LineRecord[ChessPos] == Analysised)
								{
									LineRecord[ChessPos] = FreeTwo;
									LineRecord[ChessPos + 2] = Analysised;
								}
								else
									LineRecord[ChessPos + 2] = FreeTwo;
							}
						}
						else if (LeftOne == true && AnaLine[ChessPos + 3] != AnaLine[ChessPos])
						{
							if (LineRecord[ChessPos] == Analysised)
							{
								LineRecord[ChessPos] = SleepTwo;
								LineRecord[ChessPos + 2] = Analysised;
							}
							else
								LineRecord[ChessPos + 2] = SleepTwo;
						}
					}
				}
				else if (AnaLine[ChessPos + 2] == NONE)
				{
					if (ChessPos + 2 < GridNum)
					{
						if (AnaLine[ChessPos + 3] == AnaLine[ChessPos])
						{
							if (ChessPos + 3 < GridNum)
							{
								if (AnaLine[ChessPos + 4] == NONE)
								{
									if (LeftOne == true)
									{
										if (LineRecord[ChessPos] == Analysised)
										{
											LineRecord[ChessPos] = FreeTwo;
											LineRecord[ChessPos + 3] = Analysised;
										}
										else
											LineRecord[ChessPos + 3] = FreeTwo;
									}
									else
									{
										LineRecord[ChessPos] = SleepTwo;
										LineRecord[ChessPos + 3] = Analysised;
									}
								}
								else if (LeftOne == true)
								{
									if (AnaLine[ChessPos + 4] != AnaLine[ChessPos])
									{
										if (LineRecord[ChessPos] == Analysised)
										{
											LineRecord[ChessPos] = SleepTwo;
											LineRecord[ChessPos + 3] = Analysised;
										}
										else
											LineRecord[ChessPos + 3] = SleepTwo;
									}
								}
							}
							else if (LeftOne == true)
							{
								if (LineRecord[ChessPos] == Analysised)
								{
									LineRecord[ChessPos] = SleepTwo;
									LineRecord[ChessPos + 3] = Analysised;
								}
								else
									LineRecord[ChessPos + 3] = SleepTwo;
							}
						}
					}
				}
			}
		}
		return;
	}
}