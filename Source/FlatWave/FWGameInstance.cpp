// Fill out your copyright notice in the Description page of Project Settings.

#include "FWGameInstance.h"
#include "Sound/SoundClass.h"

void UFWGameInstance::SetMouseSensitivityScale(float NewScale)
{
	MouseSensitivityScale = NewScale;
}

void UFWGameInstance::IncreaseSensitivity(float Interval /*= 0.2f*/)
{
	MouseSensitivityScale = FMath::Clamp(MouseSensitivityScale + Interval, 0.2f, 2.f);
}

void UFWGameInstance::DecreaseSensitivity(float Interval /*= -.2f*/)
{
	MouseSensitivityScale = FMath::Clamp(MouseSensitivityScale + Interval, 0.2f, 2.f);
}

float UFWGameInstance::GetMouseSensitivityScale()
{
	return MouseSensitivityScale;
}

void UFWGameInstance::Init()
{
	Super::Init();
	if (MasterClass)
		MasterClass->Properties.Volume = 1.f;
	if (EffectsClass)
		EffectsClass->Properties.Volume = 1.f;
	if (MusicClass)
		MusicClass->Properties.Volume = 1.f;
}
