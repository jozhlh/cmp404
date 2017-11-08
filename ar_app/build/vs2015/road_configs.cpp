#include "road_configs.h"

RoadConfigs::RoadConfigs()
{
}

RoadConfigs::~RoadConfigs()
{
}

bool* RoadConfigs::GetConfigs(RoadType shape, bool* config)
{
	switch (shape)
	{
	case cross:
		return GetCrossConfigs(config);
	case corner:
		return GetCornerConfigs(config);
	default:
		break;
	}
	return config;
}

bool* RoadConfigs::GetCrossConfigs(bool* config)
{
	//config = new bool[9];
	config[0] = true;
	config[1] = false;
	config[2] = true;
	config[3] = false;
	config[4] = false;
	config[5] = false;
	config[6] = true;
	config[7] = false;
	config[8] = true;
	return config;
}

bool * RoadConfigs::GetCornerConfigs(bool * config)
{
	config[0] = true;
	config[1] = false;
	config[2] = true;
	config[3] = true;
	config[4] = false;
	config[5] = false;
	config[6] = true;
	config[7] = true;
	config[8] = true;
	return config;
}
