
enum Health { healthy, dead, destroyed, diseased, geas, lycanthropy, charmed }

struct Location {
	int xpos;
	int ypos;
    }

struct Item {
	string name;
    }

struct Player {
	string name;
	struct Location loc;
	enum Health health;
	struct Item* weapon;
	array<struct Item*> backpack;
	bool dead;
	int hp;
	int maxhp;
    }

