//--------------------------------------------------------------------------------------
// File: Game.cpp
//
// Developer unit test for DirectXTK Keyboard
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

#include "pch.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

static const XMVECTORF32 START_POSITION = { 0.f, -1.5f, 0.f, 0.f };
static const XMVECTORF32 ROOM_BOUNDS = { 8.f, 6.f, 12.f, 0.f };

#define MOVEMENT_GAIN 0.07f

// Constructor.
Game::Game() :
    m_lastStr(nullptr)
{
    m_cameraPos = START_POSITION.v;

    *m_lastStrBuff = 0;

    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
    IUnknown* window,
#else
    HWND window,
#endif
    int width, int height, DXGI_MODE_ROTATION rotation)
{
    m_keyboard = std::make_unique<Keyboard>();

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
    m_deviceResources->SetWindow(window, width, height, rotation);
    m_keyboard->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));
#else
    UNREFERENCED_PARAMETER(rotation);
    m_deviceResources->SetWindow(window, width, height);
#endif

    // Singleton test
    {
        bool thrown = false;

        try
        {
            std::unique_ptr<Keyboard> kb2(new Keyboard);
        }
        catch (...)
        {
            thrown = true;
        }

        if (!thrown)
        {
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
            throw std::exception("Keyboard not acting like a singleton");
#else
            MessageBox(window, L"Keyboard not acting like a singleton", L"KeyboardTest", MB_ICONERROR);
#endif
        }

        auto state = Keyboard::Get().GetState();
        state;
    }

    OutputDebugStringA(m_keyboard->IsConnected() ? "INFO: Keyboard is connected\n" : "INFO: No keyboard found\n");

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const&)
{
    auto kb = m_keyboard->GetState();

#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
    if (kb.Escape)
    {
        PostQuitMessage(0);
    }
#endif

    if (kb.Home)
        m_keyboard->Reset();

    m_tracker.Update(kb);

    if (m_tracker.pressed.Q)
        m_lastStr = L"Q was pressed";
    else if (m_tracker.released.Q)
        m_lastStr = L"Q was released";
    else if (m_tracker.pressed.W)
        m_lastStr = L"W was pressed";
    else if (m_tracker.released.W)
        m_lastStr = L"W was released";
    else if (m_tracker.pressed.E)
        m_lastStr = L"E was pressed";
    else if (m_tracker.released.E)
        m_lastStr = L"E was released";
    else if (m_tracker.pressed.R)
        m_lastStr = L"R was pressed";
    else if (m_tracker.released.R)
        m_lastStr = L"R was released";
    else if (m_tracker.pressed.T)
        m_lastStr = L"T was pressed";
    else if (m_tracker.released.T)
        m_lastStr = L"T was released";
    else if (m_tracker.pressed.Y)
        m_lastStr = L"Y was pressed";
    else if (m_tracker.released.Y)
        m_lastStr = L"Y was released";
    else if (m_tracker.pressed.LeftShift)
        m_lastStr = L"LeftShift was pressed";
    else if (m_tracker.released.LeftShift)
        m_lastStr = L"LeftShift was released";
    else if (m_tracker.pressed.LeftAlt)
        m_lastStr = L"LeftAlt was pressed";
    else if (m_tracker.released.LeftAlt)
        m_lastStr = L"LeftAlt was released";
    else if (m_tracker.pressed.LeftControl)
        m_lastStr = L"LeftCtrl was pressed";
    else if (m_tracker.released.LeftControl)
        m_lastStr = L"LeftCtrl was released";
    else if (m_tracker.pressed.RightShift)
        m_lastStr = L"RightShift was pressed";
    else if (m_tracker.released.RightShift)
        m_lastStr = L"RightShift was released";
    else if (m_tracker.pressed.RightAlt)
        m_lastStr = L"RightAlt was pressed";
    else if (m_tracker.released.RightAlt)
        m_lastStr = L"RightAlt was released";
    else if (m_tracker.pressed.RightControl)
        m_lastStr = L"RightCtrl was pressed";
    else if (m_tracker.released.RightControl)
        m_lastStr = L"RightCtrl was released";
    else if (m_tracker.pressed.Space)
        m_lastStr = L"Space was pressed";
    else if (m_tracker.released.Space)
        m_lastStr = L"Space was released";
    else if (m_tracker.pressed.Up)
        m_lastStr = L"Up was pressed";
    else if (m_tracker.released.Up)
        m_lastStr = L"Up was released";
    else if (m_tracker.pressed.Down)
        m_lastStr = L"Down was presssed";
    else if (m_tracker.released.Down)
        m_lastStr = L"Down was released";
    else if (m_tracker.pressed.Left)
        m_lastStr = L"Left was pressed";
    else if (m_tracker.released.Left)
        m_lastStr = L"Left was released";
    else if (m_tracker.pressed.Right)
        m_lastStr = L"Right was pressed";
    else if (m_tracker.released.Right)
        m_lastStr = L"Right was released";
    else if (m_tracker.pressed.PageUp)
        m_lastStr = L"PageUp was pressed";
    else if (m_tracker.released.PageUp)
        m_lastStr = L"PageUp was released";
    else if (m_tracker.pressed.PageDown)
        m_lastStr = L"PageDown was pressed";
    else if (m_tracker.released.PageDown)
        m_lastStr = L"PageDown was released";

    for (int vk = VK_F1; vk <= VK_F10; ++vk)
    {
        if (m_tracker.IsKeyPressed(static_cast<DirectX::Keyboard::Keys>(vk)))
        {
            wchar_t buff[5];
            swprintf_s(buff, L"F%d", vk - VK_F1 + 1);
            swprintf_s(m_lastStrBuff, L"%s was pressed", buff);
            m_lastStr = m_lastStrBuff;
        }
        else if (m_tracker.IsKeyReleased(static_cast<DirectX::Keyboard::Keys>(vk)))
        {
            wchar_t buff[5];
            swprintf_s(buff, L"F%d", vk - VK_F1 + 1);
            swprintf_s(m_lastStrBuff, L"%s was released", buff);
            m_lastStr = m_lastStrBuff;
        }
    }

    for (int vk = 0x30; vk <= 0x39; ++vk)
    {
        if (m_tracker.IsKeyPressed(static_cast<DirectX::Keyboard::Keys>(vk)))
        {
            wchar_t buff[3];
            swprintf_s(buff, L"%d", vk - 0x30);
            swprintf_s(m_lastStrBuff, L"%s was pressed", buff);
            m_lastStr = m_lastStrBuff;
        }
        else if (m_tracker.IsKeyReleased(static_cast<DirectX::Keyboard::Keys>(vk)))
        {
            wchar_t buff[3];
            swprintf_s(buff, L"%d", vk - 0x30);
            swprintf_s(m_lastStrBuff, L"%s was released", buff);
            m_lastStr = m_lastStrBuff;
        }
    }

    Vector3 move = Vector3::Zero;

    if (kb.Up)
        move.y += 1.f;

    if (kb.Down)
        move.y -= 1.f;

    if (kb.Left)
        move.x -= 1.f;

    if (kb.Right)
        move.x += 1.f;

    if (kb.PageUp)
        move.z += 1.f;

    if (kb.PageDown)
        move.z -= 1.f;

    move *= MOVEMENT_GAIN;

    m_cameraPos += move;

    Vector3 halfBound = (Vector3(ROOM_BOUNDS.v) / Vector3(2.f) ) - Vector3(0.1f, 0.1f, 0.1f);

    m_cameraPos = Vector3::Min(m_cameraPos, halfBound);
    m_cameraPos = Vector3::Max(m_cameraPos, -halfBound);

    m_kb = kb;
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    XMVECTOR lookAt = XMVectorAdd(m_cameraPos, Vector3::Backward);

    XMMATRIX view = XMMatrixLookAtRH(m_cameraPos, lookAt, Vector3::Up);

    m_room->Draw(Matrix::Identity, view, m_proj, Colors::White, m_roomTex.Get());

    XMVECTOR xsize = m_comicFont->MeasureString(L"X");

    float width = XMVectorGetX(xsize);
    float height = XMVectorGetY(xsize);

    m_spriteBatch->Begin();

    XMFLOAT2 pos(50, 50);

    // Row 0
    for (int vk = VK_F1; vk <= VK_F10; ++vk)
    {
        wchar_t buff[5] = {};
        swprintf_s(buff, L"F%d", vk - VK_F1 + 1);
        m_comicFont->DrawString(m_spriteBatch.get(), buff, pos, m_kb.IsKeyDown(static_cast<DirectX::Keyboard::Keys>(vk)) ? Colors::Red : Colors::LightGray);

        pos.x += width * 3;
    }

    // Row 1
    pos.x = 50;
    pos.y += height * 2;

    for (int vk = 0x30; vk <= 0x39; ++vk)
    {
        wchar_t buff[3] = {};
        swprintf_s(buff, L"%d", vk - 0x30);
        m_comicFont->DrawString(m_spriteBatch.get(), buff, pos, m_kb.IsKeyDown(static_cast<DirectX::Keyboard::Keys>(vk)) ? Colors::Red : Colors::LightGray);

        pos.x += width * 2;
    }

    // Row 2
    pos.x = 50;
    pos.y += height * 2;

    m_comicFont->DrawString(m_spriteBatch.get(), L"Q", pos, m_kb.Q ? Colors::Red : Colors::LightGray);

    pos.x += width * 2;

    m_comicFont->DrawString(m_spriteBatch.get(), L"W", pos, m_kb.W ? Colors::Red : Colors::LightGray);

    pos.x += width * 2;

    m_comicFont->DrawString(m_spriteBatch.get(), L"E", pos, m_kb.E ? Colors::Red : Colors::LightGray);

    pos.x += width * 2;

    m_comicFont->DrawString(m_spriteBatch.get(), L"R", pos, m_kb.R ? Colors::Red : Colors::LightGray);

    pos.x += width * 2;

    m_comicFont->DrawString(m_spriteBatch.get(), L"T", pos, m_kb.T ? Colors::Red : Colors::LightGray);

    pos.x += width * 2;

    m_comicFont->DrawString(m_spriteBatch.get(), L"Y", pos, m_kb.Y ? Colors::Red : Colors::LightGray);

    // Row 3
    pos.x = 50;
    pos.y += height * 2;

    m_comicFont->DrawString(m_spriteBatch.get(), L"LeftShift", pos, m_kb.LeftShift ? Colors::Red : Colors::LightGray);

    pos.x += width * 10;

    m_comicFont->DrawString(m_spriteBatch.get(), L"RightShift", pos, m_kb.RightShift ? Colors::Red : Colors::LightGray);

    // Row 4
    pos.x = 50;
    pos.y += height * 2;

    m_comicFont->DrawString(m_spriteBatch.get(), L"LeftCtrl", pos, m_kb.LeftControl ? Colors::Red : Colors::LightGray);

    pos.x += width * 10;

    m_comicFont->DrawString(m_spriteBatch.get(), L"RightCtrl", pos, m_kb.RightControl ? Colors::Red : Colors::LightGray);

    // Row 5
    pos.x = 50;
    pos.y += height * 2;

    m_comicFont->DrawString(m_spriteBatch.get(), L"LeftAlt", pos, m_kb.LeftAlt ? Colors::Red : Colors::LightGray);

    pos.x += width * 10;

    m_comicFont->DrawString(m_spriteBatch.get(), L"RightAlt", pos, m_kb.RightAlt ? Colors::Red : Colors::LightGray);

    // Row 6
    pos.x = 50;
    pos.y += height * 2;

    m_comicFont->DrawString(m_spriteBatch.get(), L"Space", pos, m_kb.Space ? Colors::Red : Colors::LightGray);

    if (m_lastStr)
    {
        m_comicFont->DrawString(m_spriteBatch.get(), m_lastStr, XMFLOAT2(50, 650), Colors::Yellow);
    }

    m_spriteBatch->End();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
}

void Game::OnResuming()
{
    m_tracker.Reset();
    m_timer.ResetElapsedTime();
}

void Game::OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation)
{
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
    if (!m_deviceResources->WindowSizeChanged(width, height, rotation))
        return;
#else
    UNREFERENCED_PARAMETER(rotation);
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;
#endif

    CreateWindowSizeDependentResources();
}

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
void Game::ValidateDevice()
{
    m_deviceResources->ValidateDevice();
}
#endif

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    width = 1280;
    height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    m_spriteBatch = std::make_unique<SpriteBatch>(context);

    m_comicFont = std::make_unique<SpriteFont>(device, L"comic.spritefont");

    m_room = GeometricPrimitive::CreateBox(context, XMFLOAT3(ROOM_BOUNDS[0], ROOM_BOUNDS[1], ROOM_BOUNDS[2]), false, true);

    DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"texture.dds", nullptr, m_roomTex.GetAddressOf()));
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    auto size = m_deviceResources->GetOutputSize();
    m_proj = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(70.f), float(size.right) / float(size.bottom), 0.01f, 100.f);

    auto viewPort = m_deviceResources->GetScreenViewport();
    m_spriteBatch->SetViewport(viewPort);

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP) 
    m_spriteBatch->SetRotation(m_deviceResources->GetRotation());
#endif
}

void Game::OnDeviceLost()
{
    m_room.reset();
    m_spriteBatch.reset();
    m_comicFont.reset();

    m_roomTex.Reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
