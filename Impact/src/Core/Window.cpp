#include "Window.h"
#ifdef _DEBUG
#include "Debug/WindowsMessageMap.h"
#endif // _DEBUG
#include <sstream>
#include "../SandBox/resource.h"

#include "Core/Exceptions/WindowThrow.h"
#include "Core/Keyboard.h"
#include "Core/Mouse.h"
#include "Graphics/Graphics.h"
#include <imgui_impl_win32.h>
//#include "backends/imgui_impl_dx11.h"

namespace Impact
{
	//Window::UserData Window::m_Data{};
	//Input& Window::m_Input = Input::Get();
	std::vector<BYTE> Window::m_RawBuffer{};

	// TODO: move Keyboard and Mouse ++ into Input
	Keyboard& Window::m_Keyboard = Keyboard::Get();
	Mouse& Window::m_Mouse = Mouse::Get();

	Window::Window(uint16_t width, uint16_t height, const wchar_t* name) //noexcept
		: m_Width{ width }
		, m_Height{ height }
		, m_hWnd{nullptr}
	{
		RECT wRect{};
		wRect.top = 100;
		wRect.left = 100;
		wRect.bottom = height + wRect.top;
		wRect.right = width + wRect.left;

		const long windowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;

		if( AdjustWindowRect(&wRect, windowStyle, FALSE) != 1)
		{
			throw WND_LAST_EXCEPTION(__LINE__, __FILE__);
		}
		m_hWnd = CreateWindowExW(
			0,								// Optional window styles.
			WindowClass::GetName(),			// Window class
			name,							// Window text/title.

			windowStyle,					// Window styles.

			CW_USEDEFAULT, CW_USEDEFAULT,	// Window Position (x,y)
			wRect.right - wRect.left,		// Window Widths
			wRect.bottom - wRect.top,		// Window Height

			nullptr,						// Parent Window
			nullptr,						// Menu
			WindowClass::GetInstance(),		// Instance handle
			this							// Additional application data
		);

		if ( m_hWnd == nullptr )
		{
			#ifdef _DEBUG
			OutputDebugStringA("HWND was NULL");
			#endif // DEBUG
		
			WND_LAST_EXCEPTION(__LINE__, __FILE__);
		}
		else
		{
			RegisteRawInputDevice();

			ShowWindow(m_hWnd, SW_SHOWNORMAL);
			UpdateWindow(m_hWnd);

			pGfx = std::make_unique<Graphics>(m_hWnd, m_Width, m_Height);
		}
	}

	Window::~Window()
	{
		DestroyWindow(m_hWnd);
	}

	void Window::RegisteRawInputDevice()
	{
		static bool rawInputInitalized = false;
		if ( rawInputInitalized == false )
		{
			RAWINPUTDEVICE rid{};

			rid.usUsagePage = 0x01;	// mouse page
			rid.usUsage = 0x02;		// mouse usage
			rid.dwFlags = 0;
			rid.hwndTarget = nullptr;

			if ( RegisterRawInputDevices(&rid, 1, sizeof(rid)) == false)
			{
				WND_LAST_EXCEPTION(__LINE__, __FILE__);
			}
			rawInputInitalized = true;
		}
	}


	void Window::SetTitle(const std::wstring& title)
	{
			if ( SetWindowText(m_hWnd, title.c_str()) == 0 )
				throw WND_LAST_EXCEPTION(__LINE__, __FILE__);
	}

	std::optional<int> Window::ProcessMessages() noexcept
	{
		MSG msg;
		while ( PeekMessage(&msg, nullptr, 0,0, PM_REMOVE ))
		{
			// keep getting SW_NULL & SW_GETTEXT, it is currently a bugg that has been fixed in a alpha version according to this link
			// https://www.autoitscript.com/forum/topic/204937-confused-by-wingettitle/
			if ( msg.message != WM_QUIT )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
				return (int) msg.wParam;
			}
		}
		return {};
	}

	Graphics& Window::GetGraphix() const
	{
		if(pGfx)
			return *pGfx;
		throw WND_LAST_EXCEPTION(__LINE__, __FILE__);
	}

	// this is only to set up the instance of this window on the Win32 side and
	// to install the propper message handler
	LRESULT CALLBACK Window::HandleMessageStartUp(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if ( uMsg == WM_NCCREATE )
		{
			const CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCT*>( lParam );
			Window* pWindow = static_cast<Window*>( pCreate->lpCreateParams );

			// store pointer to Window class in WinApi UserData
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWindow ));
			// insert the normal messageProc
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &Window::HandleMessageInter ));
			// send off message to the Post-StartUp handler
			return pWindow->HandleMessage(hwnd, uMsg, wParam, lParam);
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	// this "transit" function is due to Win32 not knowing much about c++, so we've inserted this static functiono on it's side.
	// as in.. it's an adapter for c++ calling convension, Win32 doesn't do what c++ does.
	// c++ has hidden memberfunction "this", which Win32 doesn't have... 
	// so we're using an intermediat function call to call out member function :)
	LRESULT CALLBACK Window::HandleMessageInter(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// get pointer to window instance
		Window* pWindow = reinterpret_cast<Window*>( GetWindowLongPtr(hwnd, GWLP_USERDATA));
		// send off message to the instances message handler
		return pWindow->HandleMessage(hwnd, uMsg, wParam, lParam);
	}

	LRESULT Window::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
	//#ifdef _DEBUG
	//	static WindowsMessageMap wmm;
	//	OutputDebugStringA(wmm(uMsg, lParam, wParam).c_str());
	//#endif// _DEBUG
	// 
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}

		// this is done to prevent a crash from happening when ImGui Context has not yet been created.
		const ImGuiIO& imguiIO = (ImGui::GetCurrentContext()) ? ImGui::GetIO() : ImGuiIO{};

		switch ( uMsg )
		{
			case WM_CLOSE:
				{
					if(!GetParent(m_hWnd) )
					{
						PostQuitMessage(0x45);
						return 0; 
					}
					else
					{
						CloseWindow(m_hWnd);
						return 0;
					}
					break;
				}// clear keystate when window loses focus to prevent input getting "stuck"
			case WM_KILLFOCUS:
				{
					m_Keyboard.ClearState();
					break;
				}
			//TODO: Handle Resizing.
			//case WM_SIZE:
			//	{
			//		//UINT width = LOWORD(lParam);
			//		//UINT height = HIWORD(lParam);
			//		//return 0;
			//	}
		#pragma region Keyboard
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
				{
					if(imguiIO.WantCaptureKeyboard) break;

					if ( !( lParam & 0x40000000 )  || m_Keyboard.IsAutorepeatEnabled()  ) // filter autorepeat
						m_Keyboard.OnKeyDown(static_cast<uint8_t>( wParam ));
					break;
				}
			case WM_SYSKEYUP:
			case WM_KEYUP:
				{
					if (imguiIO.WantCaptureKeyboard) break;

					m_Keyboard.OnKeyUp(static_cast<uint8_t>( wParam ));
					break;
				}
			case WM_CHAR:
				{
					if (imguiIO.WantCaptureKeyboard) break;

					m_Keyboard.OnChar(static_cast<wchar_t>( wParam ));
					break;
				}
		#pragma endregion
				//mouse
		#pragma region Mouse
			case WM_LBUTTONDOWN:
				{
					if (imguiIO.WantCaptureMouse) break;

					const POINTS point = MAKEPOINTS(lParam);
					m_Mouse.OnLeftPressed(point.x, point.y);
					break;
				}
			case WM_RBUTTONDOWN:
				{
					if (imguiIO.WantCaptureMouse) break;

					const POINTS point = MAKEPOINTS(lParam);
					m_Mouse.OnRightPressed(point.x, point.y);
					break;
				}
			case WM_MBUTTONDOWN:
				{
					if (imguiIO.WantCaptureMouse) break;

					const POINTS point = MAKEPOINTS(lParam);
					m_Mouse.OnMiddlePressed(point.x, point.y);
					break;
				}
			case WM_LBUTTONUP:
				{
					if (imguiIO.WantCaptureMouse) break;

					const POINTS point = MAKEPOINTS(lParam);
					m_Mouse.OnLeftReleased(point.x, point.y);
					break;
				}
			case WM_RBUTTONUP:
				{
					if (imguiIO.WantCaptureMouse) break;

					const POINTS point = MAKEPOINTS(lParam);
					m_Mouse.OnRightReleased(point.x, point.y);
					break;
				}
			case WM_MBUTTONUP:
				{
					if (imguiIO.WantCaptureMouse) break;

					const POINTS point = MAKEPOINTS(lParam);
					m_Mouse.OnMiddleReleased(point.x, point.y); 
					break;
				}
			case WM_MOUSEWHEEL:		// Vertical scroll
				{
					if (imguiIO.WantCaptureMouse) break;

					const POINTS point = MAKEPOINTS(lParam);
					const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
					m_Mouse.OnWheelDelta(point.x, point.y, 0, delta);
					break;
				}
			case WM_MOUSEHWHEEL:	// Horizontal scroll
				{
					if (imguiIO.WantCaptureMouse) break;

					const POINTS point = MAKEPOINTS(lParam);
					const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
					m_Mouse.OnWheelDelta(point.x, point.y, delta, 0);
					break;
				}
			case WM_MOUSEMOVE:
				{
					if (imguiIO.WantCaptureMouse) break;

					if(!m_Mouse.IsInWindow() )
						m_Mouse.OnMouseEnter();
					const POINTS point = MAKEPOINTS(lParam);
					m_Mouse.OnMouseMove(point.x, point.y);
					break;
				}
			case WM_MOUSELEAVE:
				{
					if (imguiIO.WantCaptureMouse) break;

					m_Mouse.OnMouseLeave();
					break;
				}
		#pragma endregion
			// raw input
			case WM_INPUT:
				{
					if (imguiIO.WantCaptureMouse) break;

					UINT dataSize{};
					if( GetRawInputData(
						reinterpret_cast<HRAWINPUT>(lParam),
						RID_INPUT,
						nullptr,
						&dataSize,
						sizeof(RAWINPUTHEADER)) == -1)
					{
						break;
					}
					if ( dataSize > 0 )
					{
						m_RawBuffer.resize(dataSize);
					
						if ( GetRawInputData(
							reinterpret_cast<HRAWINPUT>( lParam ),
							RID_INPUT,
							m_RawBuffer.data(),
							&dataSize,
							sizeof(RAWINPUTHEADER)) == dataSize )
						{
							const RAWINPUT& pRawInput = reinterpret_cast<const RAWINPUT&>( *m_RawBuffer.data() );
							if ( pRawInput.header.dwType == RIM_TYPEMOUSE &&
								( pRawInput.data.mouse.lLastX != 0 || pRawInput.data.mouse.lLastY != 0)	)
							{
								m_Mouse.OnRawDelta(pRawInput.data.mouse.lLastX, pRawInput.data.mouse.lLastY);
							}
						}
					}

					break;
				}
			//case WM_PAINT:
			//	{
			//		PAINTSTRUCT ps;
			//		HDC hdc = BeginPaint(hwnd, &ps);
			//
			//
			//
			//		FillRect(hdc, &ps.rcPaint, (HBRUSH) ( COLOR_WINDOW + 1 ));
			//
			//		EndPaint(hwnd, &ps);
			//		break;
			//	}
			default:
				break;
		}
		// should never get here
		// to remvoe C4715: "not all control paths return a value"
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	#pragma region WindowClass
	Window::WindowClass Window::WindowClass::m_WndClass{};
	Window::WindowClass::WindowClass() noexcept
		: m_hInstance{ GetModuleHandle(nullptr) }
	{
		WNDCLASSEX wc{ 0 };

		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMessageStartUp;
		wc.hInstance = GetInstance();
		wc.lpszClassName = GetName();
		wc.hIcon = static_cast<HICON>(LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR));
		wc.hIconSm = static_cast<HICON>( LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR) );
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = nullptr;
		wc.hCursor = nullptr;
		wc.lpszMenuName = nullptr;

		RegisterClassEx(&wc);
	}

	Window::WindowClass::~WindowClass()
	{
		UnregisterClass(m_WndClassName, GetInstance());
	}
	const wchar_t* Window::WindowClass::GetName() noexcept
	{
		return m_WndClassName;
	}

	HINSTANCE Window::WindowClass::GetInstance() noexcept
	{
		return m_WndClass.m_hInstance;
	}
	#pragma endregion
}