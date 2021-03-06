//--------------------------------------------------------------------------------------
// File: Game.h
//
// Developer unit test for DirectXTK GamePad
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


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
#if !defined(_XBOX_ONE) || !defined(_TITLE)
    : public DX::IDeviceNotify
#endif
{
public:

    Game();

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

    // Properties
    void GetDefaultSize( int& width, int& height ) const;
    const wchar_t* GetAppName() const { return L"GamePadTest (DirectX 11)"; }

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

    // DirectXTK Test Objects
#if defined(_XBOX_ONE) && defined(_TITLE)
    std::unique_ptr<DirectX::GraphicsMemory>            m_graphicsMemory;
#endif

    std::unique_ptr<DirectX::SpriteBatch>               m_spriteBatch;
    std::unique_ptr<DirectX::SpriteFont>                m_comicFont;
    std::unique_ptr<DirectX::SpriteFont>                m_ctrlFont;
    std::unique_ptr<DirectX::GamePad>                   m_gamePad;
    DirectX::GamePad::ButtonStateTracker                m_tracker;
    DirectX::GamePad::State                             m_state;
    std::unique_ptr<bool[]>                             m_found;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_defaultTex;

    Microsoft::WRL::Wrappers::Event                 m_ctrlChanged;
    Microsoft::WRL::Wrappers::Event                 m_userChanged;

    const wchar_t *                                 m_lastStr;
    wchar_t                                         m_lastStrBuff[128];
};