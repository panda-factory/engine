// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/shell/platform/windows/testing/mock_window_win32.h"

namespace flutter {
namespace testing {
MockWin32Window::MockWin32Window() : WindowWin32(){};
MockWin32Window::MockWin32Window(
    std::unique_ptr<TextInputManagerWin32> text_input_manager)
    : WindowWin32(std::move(text_input_manager)){};

MockWin32Window::~MockWin32Window() = default;

UINT MockWin32Window::GetDpi() {
  return GetCurrentDPI();
}

LRESULT MockWin32Window::Win32DefWindowProc(HWND hWnd,
                                            UINT Msg,
                                            WPARAM wParam,
                                            LPARAM lParam) {
  return kWmResultDefault;
}

LRESULT MockWin32Window::InjectWindowMessage(UINT const message,
                                             WPARAM const wparam,
                                             LPARAM const lparam) {
  return HandleMessage(message, wparam, lparam);
}

LRESULT MockWin32Window::Win32SendMessage(HWND hWnd,
                                          UINT const message,
                                          WPARAM const wparam,
                                          LPARAM const lparam) {
  return HandleMessage(message, wparam, lparam);
}

void MockWin32Window::CallOnImeComposition(UINT const message,
                                           WPARAM const wparam,
                                           LPARAM const lparam) {
  WindowWin32::OnImeComposition(message, wparam, lparam);
}

}  // namespace testing
}  // namespace flutter
