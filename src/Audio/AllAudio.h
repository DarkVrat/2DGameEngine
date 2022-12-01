#pragma once
#include"../Audio/FileOfSound.h"
#include"../Audio/SoundDevice.h"
#include"../Audio/SoundEffectsLibrary.h"
#include"../Audio/SoundEffectsPlayer.h"
#include"../Audio/SampleSourse.h"
#include"../Audio/PlayerManager.h"

Audio::SoundDevice* GlobalSoundDevice = GETDEVICE;
Audio::SoundEffectsLibrary* LibSound = GETLIB;
Audio::PlayerManager* PlayerSound = GETPLAYER;