#pragma once
#include"../Audio/FileOfSound.h"
#include"../Audio/SoundDevice.h"
#include"../Audio/SoundEffectsLibrary.h"
#include"../Audio/SoundEffectsPlayer.h"
#include"../Audio/SampleSourse.h"

static Audio::SoundDevice* GlobalSoundDevice = SOUND_DEVICE;
static Audio::SoundEffectsLibrary* LibSound = GETLIB;