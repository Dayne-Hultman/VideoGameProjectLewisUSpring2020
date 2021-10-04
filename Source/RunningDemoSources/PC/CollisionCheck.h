

class CollisionCheck{
public:
	CollisionCheck(Tile* tile[], Player* player);

	//void Init();
	//void Deinit();
	

protected:
	Point2F playerPos, tilePos;
	float tileWidth, tileHeight, playerWidth, playerHeight;
};