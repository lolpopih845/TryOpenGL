#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>

#include "AssetObject.h"

namespace Asset {

struct KeyPosition {
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation {
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale {
	glm::vec3 scale;
	float timeStamp;
};

struct Channel {
	std::string boneName;             // maps to Skeleton bone
	std::vector<KeyPosition> positions;
	std::vector<KeyRotation> rotations;
	std::vector<KeyScale> scales;
};

class Animation:public AssetObject
{
public:
	float duration = 0.f;            // animation length
	float ticksPerSecond = 0.f;      // optional

	std::vector<Channel> channels;   // all bone channels
	Animation(const std::string& name,const std::string& path);
	// Helper to find a channel for a bone
	Channel* FindChannel(const std::string& boneName);
	friend std::ostream& operator<<(std::ostream& os, const Animation& anim);
private:
	void LoadAnimation(const std::string& path);
};

} // Asset



#endif //ANIMATION_H
