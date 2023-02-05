#pragma once
#include <string>

namespace Audio {
	class SampleSourse {
	public:
		ALfloat AlPitch;
		ALfloat AlMaxDistance;
		ALfloat AlRolloffFactor;
		ALfloat AlReferenceDistance;
		ALfloat AlMinGain;
		ALfloat AlMaxGain;
		ALfloat AlGainOutCone;
		ALfloat AlAngleInCone;
		ALfloat AlAngleOutCone;

		std::string GainString;
	};
}