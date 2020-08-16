#pragma once

#include <opencv2/core.hpp>
#include <SDL.h>
#include "rod.h"
#include "cone.h"

class Eye {
	Eye();
	~Eye();

	public:
		cv::Rect viewPort;
		Rod rodNerve;
		Cone coneNerve;
};
