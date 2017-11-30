#include "TrackCreator.h"
#include <limits>
#include <unordered_set>
#include <OgreMath.h>
#include <string>
#include <iostream>
#include <unordered_set>
#include <cmath>

std::hash<std::string> hasher;

bool operator==(const struct coord lhs, const struct coord rhs)
{
	return rhs.x == lhs.x && rhs.y == lhs.y;
}

namespace std {
    template <> struct hash<coord> {
        size_t operator()(const coord& x) const {
        	return (x.x + x.y)*2654435761 % 2^32;
        }
    };
}

std::list<DIRECTION::DIRECTION> TrackCreator::createTrack(int x_bound, int y_bound, unsigned seed)
{
	srand(seed);
	std::list<DIRECTION::DIRECTION> turns = randomListOfTurns(x_bound - 1, y_bound - 1);
	turns.push_front(DIRECTION::DOWN);
	turns.push_back(DIRECTION::RIGHT);
	std::list<DIRECTION::DIRECTION> nextTurns = randomListOfTurns(x_bound - 1, y_bound - 1);
	nextTurns.push_front(DIRECTION::DOWN);
	nextTurns.push_back(DIRECTION::RIGHT);
	for(DIRECTION::DIRECTION turn : nextTurns)
	{
		turn = (DIRECTION::DIRECTION)(((int)turn + 2) % 4);
		turns.push_back(turn);
	}

	return turns;
}

std::list<DIRECTION::DIRECTION> TrackCreator::randomListOfTurns(int x_bound, int y_bound)
{
	float nodes[x_bound][y_bound][6];

	for(int i = 0; i < x_bound; ++i)
	{
		////std::cout << "1:1: " << i << "\n";
		for(int j = 0; j < y_bound; ++j)
		{
			//std::cout << "1:2: " << j << "\n";
			if(i == 0)
			{
				nodes[i][j][0] = std::numeric_limits<float>::infinity();
			}
			else
			{
				nodes[i][j][0] = rand();
			}
			if(j == 0)
			{
				nodes[i][j][3] = std::numeric_limits<float>::infinity();
			}
			else
			{
				nodes[i][j][3] = rand();
			}
			if(i == x_bound-1)
			{
				nodes[i][j][2] = std::numeric_limits<float>::infinity();
			}
			else
			{
				nodes[i][j][2] = rand();
			}
			if(j == y_bound-1)
			{
				nodes[i][j][1] = std::numeric_limits<float>::infinity();
			}
			else
			{
				nodes[i][j][1] = rand();
			}
			nodes[i][j][4] = std::numeric_limits<float>::infinity();
			nodes[i][j][5] = static_cast<float>(DIRECTION::FUCK);
		}
	}

	//std::cout << (std::numeric_limits<float>::infinity() > 0.0f) << "\n";

	nodes[0][0][4] = 0;

	std::unordered_set<struct coord> visited;
	//std::vector<struct coord> to_visit;
	struct coord source = {.x = 0, .y = 0};
	struct coord dest = {.x = x_bound-1, .y = y_bound-1};
	struct coord cur = source;
	while(!visited.count(dest))
	{
		//std::cout << "cur = " << cur.x << ", " << cur.y << "\n";
		float curcost = nodes[cur.x][cur.y][4];
		//std::cout << "Left edge: " << nodes[cur.x][cur.y][0] << "\n";
		if( cur.x > 0 && nodes[cur.x-1][cur.y][4] > curcost + nodes[cur.x][cur.y][0] )
		{
			//std::cout << "Found a new shortest path LEFT.\n";
			nodes[cur.x-1][cur.y][4] = curcost + nodes[cur.x][cur.y][0];
			nodes[cur.x-1][cur.y][5] = DIRECTION::RIGHT;
		}
		//std::cout << "Lower edge: " << nodes[cur.x][cur.y][1] << "\n";
		if( cur.y < y_bound-1 && nodes[cur.x][cur.y+1][4] > curcost + nodes[cur.x][cur.y][1] )
		{
			//std::cout << "Found a new shortest path DOWN.\n";
			nodes[cur.x][cur.y+1][4] = curcost + nodes[cur.x][cur.y][1];
			nodes[cur.x][cur.y+1][5] = DIRECTION::UP;
		}
		//std::cout << "Right edge: " << nodes[cur.x][cur.y][2] << "\n";
		if(cur.y > (int)ceil(((float)(y_bound)/x_bound)*cur.x) && nodes[cur.x+1][cur.y][4] > curcost + nodes[cur.x][cur.y][2] )
		{
			//std::cout << "Found a new shortest path RIGHT.\n";
			nodes[cur.x+1][cur.y][4] = curcost + nodes[cur.x][cur.y][2];
			nodes[cur.x+1][cur.y][5] = DIRECTION::LEFT;
		}
		//std::cout << "Upper edge: " << nodes[cur.x][cur.y][3] << "\n";
		if( cur.y > (int)ceil(((float)(y_bound)/x_bound)*cur.x) && nodes[cur.x][cur.y-1][4] > curcost + nodes[cur.x][cur.y][3] )
		{
			//std::cout << "Found a new shortest path UP.\n";
			nodes[cur.x][cur.y-1][4] = curcost + nodes[cur.x][cur.y][3];
			nodes[cur.x][cur.y-1][5] = DIRECTION::DOWN;
		}
		visited.insert(cur);
		for(int i = 0; i < x_bound; ++i)
		{
			for(int j = 0; j < y_bound; ++j)
			{
				struct coord next = {.x = i, .y = j};
				if((visited.count(cur) || nodes[i][j][4] < nodes[cur.x][cur.y][4]) && !visited.count(next))
				{
					cur = next;
				}
			}
		}
	}
	//std::cout << "Made it past loop 2: cur = " << cur.x << ", " << cur.y << "\n";
	//std::cout << nodes[0][0][5] << "\n";
	std::list<DIRECTION::DIRECTION> turns;
	for(struct coord cur = {.x = x_bound-1, .y = y_bound-1}; nodes[cur.x][cur.y][5] != DIRECTION::FUCK;)
	{
		turns.push_front((DIRECTION::DIRECTION)((int)(nodes[cur.x][cur.y][5] + 2) % 4));
		switch((int)nodes[cur.x][cur.y][5])
		{
			case DIRECTION::UP:
				--(cur.y);
				break;
			case DIRECTION::RIGHT:
				++(cur.x);
				break;
			case DIRECTION::DOWN:
				++(cur.y);
				break;
			case DIRECTION::LEFT:
				--(cur.x);
				break;
		}
	}
	return turns;
}