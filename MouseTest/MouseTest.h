//--------------------------------------------------------------------------------------
// File: MouseTest.h
//
// Developer unit test for DirectXTK Mouse
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

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#include "DeviceResourcesUWP.h"
#else
#include "DeviceResourcesPC.h"
#endif
#include "StepTimer.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop
class Game : public DX::IDeviceNotify
{
public:

    Game();

    // Initialization and management
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
    void Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation);
#else
    void Initialize(HWND window, int width, int height, DXGI_MODE_ROTATION rotation);
#endif

    // Basic game loop
    void Tick();
    void Render();

    // Rendering helpers
    void Clear();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation);
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
    void ValidateDevice();
#endif

    void OnHome();
    void OnEnd();

    void SetDPI(float dpi);

    // Properites
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Game state
    DX::StepTimer                                   m_timer;

    // Test content
    std::unique_ptr<DirectX::SpriteBatch>               m_spriteBatch;
    std::unique_ptr<DirectX::SpriteFont>                m_comicFont;
    std::unique_ptr<DirectX::Mouse>                     m_mouse;
    DirectX::Mouse::ButtonStateTracker                  m_tracker;
    DirectX::Mouse::State                               m_ms;
    DirectX::Mouse::Mode                                m_lastMode;
    std::unique_ptr<DirectX::GeometricPrimitive>        m_room;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_roomTex;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_cursor;

    float                                               m_pitch;
    float                                               m_yaw;

    DirectX::SimpleMath::Vector3                        m_cameraPos;
    DirectX::SimpleMath::Matrix                         m_proj;

    const wchar_t *                                     m_lastStr;
    wchar_t                                             m_lastStrBuff[128];
};