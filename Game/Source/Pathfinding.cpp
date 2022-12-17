#include "App.h"
#include "PathFinding.h"
#include "Render.h"
#include "Map.h"
#include "Defs.h"
#include "Log.h"

PathFinding::PathFinding() : Module(), map(NULL), lastPath(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.Create("pathfinding");
}

// Destructor
PathFinding::~PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	lastPath.Clear();
	RELEASE_ARRAY(map);

	return true;
}

void PathFinding::DrawPath(const DynArray<iPoint>* path, int r, int g, int b)
{
	int c = path->Count() - 1;
	for (int i = 0; i < c; i++)
	{
		iPoint ap = (*path)[i];
		iPoint bp = (*path)[i + 1];

		ap.x = ap.x * app->map->mapData.tileWidth + app->map->mapData.tileWidth / 2;
		ap.y = ap.y * app->map->mapData.tileHeight + app->map->mapData.tileHeight / 2;
		bp.x = bp.x * app->map->mapData.tileWidth + app->map->mapData.tileWidth / 2;
		bp.y = bp.y * app->map->mapData.tileHeight + app->map->mapData.tileHeight / 2;

		app->render->DrawLine(ap.x, ap.y, bp.x, bp.y, r, g, b);
	}
}

// Sets up the walkability map
void PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

// Utility: return true if pos is inside the map boundaries
bool PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const DynArray<iPoint>* PathFinding::GetLastPath() const
{
	return &lastPath;
}

// To request all tiles involved in the last generated path
void PathFinding::ClearLastPath()
{
	lastPath.Clear();
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
	ListItem<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return item;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
	ListItem<PathNode>* ret = NULL;
	int min = 65535;

	ListItem<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL), jump(0) {}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent, int jump) : g(g), h(h), pos(pos), parent(parent), jump(jump)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent), jump(node.jump)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& listToFill, bool useGravity, int maxJump) const
{
	iPoint cell;
	iPoint ground;
	uint before = listToFill.list.Count();

	// right
	if (jump % 2 == 0 || !useGravity)
	{
		cell.Create(pos.x + 1, pos.y);
		if (app->pathfinding->IsWalkable(cell))
		{
			ground.Create(cell.x, cell.y + 1);
			if (!app->pathfinding->IsWalkable(ground) || !useGravity)
			{
				listToFill.list.Add(PathNode(-1, -1, cell, this));
			}
			else
			{
				listToFill.list.Add(PathNode(-1, -1, cell, this, jump + 1));
			}
		}
	}

	// left
	if (jump % 2 == 0 || !useGravity)
	{
		cell.Create(pos.x - 1, pos.y);
		if (app->pathfinding->IsWalkable(cell))
		{
			ground.Create(cell.x, cell.y + 1);
			if (!app->pathfinding->IsWalkable(ground) || !useGravity)
			{
				listToFill.list.Add(PathNode(-1, -1, cell, this));
			}
			else
			{
				listToFill.list.Add(PathNode(-1, -1, cell, this, jump + 1));
			}
		}
	}

	// down
	cell.Create(pos.x, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell))
	{
		ground.Create(cell.x, cell.y + 1);
		if (!app->pathfinding->IsWalkable(ground) || !useGravity)
		{
			listToFill.list.Add(PathNode(-1, -1, cell, this));
		}
		else
		{
			listToFill.list.Add(PathNode(-1, -1, cell, this, maxJump * 2));
		}
	}

	// up
	if (jump < maxJump * 2 || !useGravity)
	{
		cell.Create(pos.x, pos.y - 1);
		if (app->pathfinding->IsWalkable(cell) && useGravity)
		{
			listToFill.list.Add(PathNode(-1, -1, cell, this, (jump % 2) ? jump + 1 : jump + 2));
		}
		else if (app->pathfinding->IsWalkable(cell) && !useGravity)
		{
			listToFill.list.Add(PathNode(-1, -1, cell, this));
		}
	}

	return listToFill.list.Count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination, bool useGravity)
{
	if (parent == NULL)
		g = 0;
	else
		g = parent->g + 1;
	if (!useGravity)
		h = pos.DistanceTo(destination);
	else
		h = pos.DistanceManhattan(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int PathFinding::CreatePath(const iPoint& origin, const iPoint& destination, bool useGravity, int maxJump, int maxLength, int maxNodes)
{
	// L12b: TODO 1: if origin or destination are not walkable, return -1
	if (!IsWalkable(origin) || !IsWalkable(destination))
		return -1;

	if (origin == destination)
		return -1;

	lastPath.Clear();

	PathList open;
	PathList close;
	PathNode* o = new PathNode(-1, -1, origin, NULL);
	o->CalculateF(destination, useGravity);
	open.list.Add(*o);

	int k = 0;

	while (open.list.Count() > 0)
	{
		ListItem<PathNode>* lowest = open.GetNodeLowestScore();

		if (lowest->data.g > maxLength)
		{
			open.list.Del(lowest);
			continue;
		}

		int j = lowest->data.jump;

		close.list.Add(lowest->data);
		open.list.Del(lowest);

		if (lowest->data.pos == destination)
		{
			lastPath.PushBack(close.list[close.list.Count() - 1].pos);
			PathNode p = *close.list[close.list.Count() - 1].parent;
			lastPath.PushBack(p.pos);
			while (p.pos != origin)
			{
				p = *p.parent;
				lastPath.PushBack(p.pos);
			}
			lastPath.Flip();
			break;
		}

		k++;
		if (k == maxNodes)
			break;

		PathList adjacent;
		close.list[close.list.Count() - 1].FindWalkableAdjacents(adjacent, useGravity, maxJump);		for (int i = 0; i < adjacent.list.Count(); i++)
		{
			if (close.list.Find(adjacent.list[i]) != -1)
			{
				continue;
			}

			PathNode n = adjacent.list[i];
			n.CalculateF(destination, useGravity);
			int index = open.list.Find(n);
			if (index != -1)
			{
				if (open.list[index].g > n.g)
				{
					open.list[index].parent = n.parent;
					open.list[index].g = n.g;
				}
			}
			else
			{
				open.list.Add(n);
			}
		}
	}

	if (lastPath.Count() == 0)
	{
		return -1;
	}

	return lastPath.Count();
}

