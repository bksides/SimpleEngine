#ifndef TRACK_CREATOR_H
#define TRACK_CREATOR_H

#include <list>
#include <unordered_set>
#include <string>
#include <ctime>

extern std::hash<std::string> hasher;

enum TileType
{
	STRAIGHT,
	RIGHT,
	LEFT
};

class TrackCreator
{
public:
	std::list<TileType> createTrack(int x_bound = 11, int y_bound = 11, unsigned seed = (unsigned)(std::time(NULL)));//(int)hasher("gjrihtruie4wlohtfgurweightuirwe4htu5wi4hty"));
};

#endif