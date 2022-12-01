#pragma once

namespace Audio {
	class SampleSourse {
	public:
		ALfloat AlPitch = 1.f;
		ALfloat AlGain = 1.f;
		ALfloat AlMaxDistance = std::numeric_limits<float>::max();
		ALfloat AlRolloffFactor = 1.f;
		ALfloat AlReferenceDistance = 1.f;
		ALfloat AlMinGain = 0.f;
		ALfloat AlMaxGain = 1.f;
		ALfloat AlGainOutCone = 0.f;
		ALfloat AlAngleInCone = 360.f;
		ALfloat AlAngleOutCone = 360.f;
	};
}