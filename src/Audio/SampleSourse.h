#pragma once

#include <string>

namespace Audio {
	struct SampleSourse {
	public:
		ALfloat AlPitch = 1.0;
		ALfloat AlMaxDistance = 5.0;
		ALfloat AlRolloffFactor = 1.0;
		ALfloat AlReferenceDistance = 1.0;
		ALfloat AlMinGain = 0.0;
		ALfloat AlMaxGain = 1.0;
		ALfloat AlGainOutCone = 0.0;
		ALfloat AlAngleInCone = 360.0;
		ALfloat AlAngleOutCone = 360.0;

		std::string GainString = "default";
	};
}