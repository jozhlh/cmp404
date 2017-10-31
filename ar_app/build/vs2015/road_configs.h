#ifndef _ROAD_CONFIGS_H
#define _ROAD_CONFIGS_H

enum RoadType { cross };

class RoadConfigs
{
public:
	RoadConfigs();
	~RoadConfigs();

	static bool* GetConfigs(RoadType shape, bool* config);
	static bool* GetCrossConfigs(bool* config);
};

#endif
