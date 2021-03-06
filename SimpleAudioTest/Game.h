//--------------------------------------------------------------------------------------
// File: Game.h
//
// Developer unit test for DirectXTK - Simple Audio Test (UI-based BasicAudioTest)
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------
#pragma once

#if defined(_XBOX_ONE) && defined(_TITLE)
#include "DeviceResourcesXDK.h"
#elif defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#include "DeviceResourcesUWP.h"
#else
#include "DeviceResourcesPC.h"
#endif
#include "StepTimer.h"
#include "TextConsole.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
#if !defined(_XBOX_ONE) || !defined(_TITLE)
    : public DX::IDeviceNotify
#endif
{
public:

    Game();
    ~Game();

    // Initialization and management
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP) 
    void Initialize(HWND window, int width, int height, DXGI_MODE_ROTATION rotation);
#else
    void Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation);
#endif

    // Basic game loop
    void Tick();

#if !defined(_XBOX_ONE) || !defined(_TITLE)
    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;
#endif

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();

#if !defined(_XBOX_ONE) || !defined(_TITLE)
    void OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation);
#endif

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
    void ValidateDevice();
#endif

    void OnAudioDeviceChange();

    // Properties
    void GetDefaultSize( int& width, int& height ) const;
    const wchar_t* GetAppName() const { return L"SimpleAudioTest (DirectX 11)"; }

private:

    void Update(DX::StepTimer const& timer);
    void Render();
    void AudioRender();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

    // Input devices.
    std::unique_ptr<DirectX::GamePad>       m_gamePad;
    std::unique_ptr<DirectX::Keyboard>      m_keyboard;
    DirectX::Keyboard::KeyboardStateTracker m_keyboardButtons;
    DirectX::GamePad::ButtonStateTracker    m_gamepadButtons;

    // DirectXTK Test Objects
#if defined(_XBOX_ONE) && defined(_TITLE)
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;
#endif

    std::unique_ptr<DirectX::AudioEngine>   m_audEngine;
    std::unique_ptr<DirectX::SpriteBatch>   m_spriteBatch;
    std::unique_ptr<DirectX::SpriteFont>    m_comicFont;

    std::unique_ptr<DirectX::SoundEffect>   m_alarmPCM;
    std::unique_ptr<DirectX::SoundEffect>   m_alarmADPCM;
    std::unique_ptr<DirectX::SoundEffect>   m_alarmFLOAT;

#if defined(_XBOX_ONE) || (_WIN32_WINNT < _WIN32_WINNT_WIN8) || (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)
    std::unique_ptr<DirectX::SoundEffect>   m_alarmXWMA;
    std::unique_ptr<DirectX::WaveBank>      m_wbXWMA;
#endif

#if defined(_XBOX_ONE) && defined(_TITLE)
    std::unique_ptr<DirectX::SoundEffect>   m_alarmXMA;
    std::unique_ptr<DirectX::WaveBank>      m_wbXMA;
#endif

    std::unique_ptr<DirectX::WaveBank>      m_wbPCM;
    std::unique_ptr<DirectX::WaveBank>      m_wbADPCM;

    std::unique_ptr<DX::TextConsole> m_console;

    bool m_critError;
    bool m_retrydefault;
    bool m_newAudio;

    wchar_t m_deviceStr[256];

    bool m_gamepadPresent;
};