// TestWindow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Doppler.h"


int main()
{
    Doppler engine;
    engine.CreateEngineInstance();
    engine.BeginEngineLoop();
    engine.Cleanup();
}