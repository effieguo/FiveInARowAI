#include "HistoryHeuristic.h"
#include <string>
#include <ctime>


int HistoryHeuristic::ScorePattern[15][15] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
	{ 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
	{ 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

HistoryHeuristic::HistoryHeuristic(void)
{
}

HistoryHeuristic::~HistoryHeuristic(void)
{
}

void HistoryHeuristic::ResetHistoryTable()
{
	memcpy(historyTable, HistoryHeuristic::ScorePattern, 225 * sizeof(int));
	srand((unsigned int)time(NULL));
}

void HistoryHeuristic::EnterHistoryTable(ChessMove CM, int depth)
{
	historyTable[CM.ChessPos.y][CM.ChessPos.x] += 2 << depth;
}

void HistoryHeuristic::Merge(ChessMove source[], ChessMove target[], int left, int mid, int right, int Tab[][15])
{
	int l = left, m = mid + 1, n = left;

	while (l <= mid && m <= right)
	{
		if (Tab[source[l].ChessPos.y][source[l].ChessPos.x] < Tab[source[m].ChessPos.y][source[m].ChessPos.x])
			target[n++] = source[m++];
		else if (Tab[source[l].ChessPos.y][source[l].ChessPos.x] == Tab[source[m].ChessPos.y][source[m].ChessPos.x])
		{
			if (rand() % 4 == 1)
				target[n++] = source[m++];
			else
				target[n++] = source[l++];
		}
		else
			target[n++] = source[l++];
	}

	if (l > mid)
		while (m <= right)
			target[n++] = source[m++];
	else
		while (l <= mid)
			target[n++] = source[l++];
}

void HistoryHeuristic::MergePass(ChessMove source[], ChessMove target[], int count, int length, int Tab[][15])
{
	int start = 0;
	while (count - start >= 2 * length)
	{
		Merge(source, target, start, start + length - 1, start + 2 * length - 1, Tab);
		start += 2 * length;
	}
	if (start + length < count)
		Merge(source, target, start, start + length - 1, count - 1, Tab);
	else
	{
		while (start < count)
		{
			target[start] = source[start];
			start++;
		}
	}
}

void HistoryHeuristic::MergeSort(ChessMove source[], int count, int Tab[][15])
{
	int length = 1;
	static ChessMove SortBuff[255];
	while (length < count)
	{
		MergePass(source, SortBuff, count, length, Tab);
		length += length;
		MergePass(SortBuff, source, count, length, Tab);
		length += length;
	}
}