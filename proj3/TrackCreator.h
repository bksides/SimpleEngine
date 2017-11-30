#ifndef TRACK_CREATOR_H
#define TRACK_CREATOR_H

#include <list>
#include <unordered_set>
#include <string>
#include <ctime>

extern std::hash<std::string> hasher;

namespace DIRECTION
{
	enum DIRECTION
	{
		LEFT = 0,
		DOWN = 1,
		RIGHT = 2,
		UP = 3,
		FUCK = 4
	};
}

class TrackCreator
{
private:
	static std::list<DIRECTION::DIRECTION> randomListOfTurns(int x_bound, int y_bound);
public:
	std::list<DIRECTION::DIRECTION> createTrack(int x_bound = 25, int y_bound = 25, unsigned seed = (unsigned)(std::time(NULL)));//(int)hasher("gjrihtruie4wlohtfgurweightuirwe4htu5wi4hty"));
};

#endif