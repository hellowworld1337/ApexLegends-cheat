namespace GUI
{
	bool Visible = true;
	WNDPROC NextWndProc;
	enum Button {
		NoPress,
		Pressed,
		Clicked
	};
	struct IO {
		Button LKM;
		Button RKM;
		Button ALT;
		bool Click;

		Vector2 MousePos;
		Vector2 MouseDelta;
		Vector2 OldMousePos;
		USHORT CurElement;
	} IO;
	struct WndData {
		Vector2 WndPos;
		Vector2 Size;
		Vector2 Pos;
	} CurWnd;

	//str hash
	unsigned short __forceinline HashStr(const wchar_t* Str)
	{
		unsigned char i;
		unsigned short crc = 0xFFFF;
		while (wchar_t DChar = *Str++) {
			unsigned char Char = (unsigned char)DChar;
			crc ^= Char << 8;
			for (i = 0; i < 8; i++)
				crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
			Char = (unsigned char)(DChar >> 8);
			crc ^= Char << 8;
			for (i = 0; i < 8; i++)
				crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
		} return crc;
	}

	Vector2 __forceinline CenterLine(const Vector2& Pos) {
		return { (Pos.x + (CurWnd.Size.x / 2)), Pos.y };
	}

	bool __forceinline InRect(Vector2 Rect, Vector2 Size, Vector2 Dot) {
		return (Dot.x > Rect.x && Dot.x < Rect.x + Size.x && Dot.y > Rect.y && Dot.y < Rect.y + Size.y);
	}

	//input
	LRESULT __stdcall WndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		MUTATE
		switch (Msg)
		{
			case WM_LBUTTONDOWN:
				IO.LKM = Pressed;
				break;

			case WM_LBUTTONUP:
				if (IO.LKM == Pressed)
					IO.LKM = Clicked;
				else IO.LKM = NoPress;
				break;

			case WM_RBUTTONDOWN:
				IO.RKM = Pressed;
				break;

			case WM_RBUTTONUP:
				IO.RKM = NoPress;
				break;

			case WM_KEYUP:
				if (wParam == VK_INSERT)
					if (Visible)
						Visible = false;
					else
						Visible = true;
				break;

			case WM_SYSKEYDOWN:
				if (wParam == VK_MENU)
					IO.ALT = Pressed;
				break;

			case WM_SYSKEYUP:
				if (wParam == VK_MENU)
					IO.ALT = NoPress;
				break;

			case WM_MOUSEMOVE:
				IO.MousePos.x = (signed short)(lParam);
				IO.MousePos.y = (signed short)(lParam >> 16);
				break;
		}

		MUTATE_END
		return 0;
		//return FC(user32, CallWindowProcA, NextWndProc, Wnd, Msg, wParam, lParam);
	}

	LRESULT __stdcall WndHook(int nCode, WPARAM wParam, LPARAM lParam)
	{
		MSG* WndProcData = (MSG*)lParam;
		return WndProc(WndProcData->hwnd, WndProcData->message, WndProcData->wParam, WndProcData->lParam);
	}

	void ProcessInput(bool End = false)
	{
		if (!End)
		{
			//calc mouse delta
			//if (IO.LKM == Clicked) sp("111");
			//IO.Updated = true;
			IO.MouseDelta = IO.MousePos - IO.OldMousePos;
			Global::MouseDelta = IO.MouseDelta;
			IO.OldMousePos = IO.MousePos;
		}

		else //if (IO.Updated)
		{
			if (IO.LKM == Clicked) {
				IO.LKM = NoPress;
				//IO.CurElement = 0;
				//sp("click");
			}

			if(IO.LKM == NoPress)
				IO.CurElement = 0;
		//	//update LKM button
		//	if (IO.LKM == Clicked)
		//		IO.LKM = Pressed;
		}
	}

	__forceinline bool InputMgr(const wchar_t* Name, bool Reg = false)
	{
		unsigned short StrHash = HashStr(Name);
		if (Reg && !IO.CurElement) {
			IO.CurElement = StrHash;
			return true;
		}
		else if (!Reg)
			return (IO.CurElement == StrHash);
		return false;
	}

	//main
	void SliderFloat(const wchar_t* Name, float* Current, float Min, float Max)
	{
		MUTATE

		auto Format = xorstr(L"%s : %d"); wchar_t Str[64]; FC(ntdll, _swprintf, Str, Format.crypt_get(), Name, (int)*Current);
		Render::String({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, Str/*FC_FORMAT_W(Format.crypt_get(), Name, (int)*Current)*/);
		CurWnd.Pos.y += 15.f;

		if (IO.LKM == Pressed && InRect({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, { CurWnd.Size.x - 20.f, 16.f }, IO.MousePos)) {
			float Val = ((((IO.MousePos.x - 18.f - CurWnd.Pos.x) / (CurWnd.Size.x - 36.f)) * (Max - Min)) + Min);
			*Current = ((Val > Max) ? Max : ((Val < Min) ? Min : Val)); InputMgr(Name, true);
		}

		float CurOff = (*Current - Min) / (Max - Min);
		Render::FillRoundedRectangle({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, { CurWnd.Size.x - 20.f, 16.f }, Render::Color(127, 255, 0), 8.f);
		Render::FillCircle({ CurWnd.Pos.x + 18.f + ((CurWnd.Size.x - 36.f) * CurOff), CurWnd.Pos.y + 8 }, Render::Color(255, 255, 255), 8.f);
		CurWnd.Pos.y += 20.f;

		MUTATE_END
	}

	void CheckBox(const wchar_t* Name, bool& Switch)
	{
		MUTATE

		Render::String({ CurWnd.Pos.x + 42.f, CurWnd.Pos.y }, (wchar_t*)Name);
		if (IO.LKM == Pressed && InRect({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, { 30.f, 16.f }, IO.MousePos) && InputMgr(Name, true)) Switch = !Switch;
		Render::FillRoundedRectangle({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, { 30.f, 16.f }, Switch ? Render::Color(127, 255, 0) : Render::Color(255, 0, 0), 8.f);
		Render::FillCircle({ CurWnd.Pos.x + 18.f + (Switch ? 14.f : 0.f), CurWnd.Pos.y + 8.f }, Render::Color(255, 255, 255), 8.f); CurWnd.Pos.y += 20.f;

		MUTATE_END
	}

	void Spacing(const wchar_t* Name) 
	{
		MUTATE

		CurWnd.Pos.y += 4.f;
		Vector2 CntLine = CenterLine(CurWnd.Pos); Vector2 TextSize = Render::String(CntLine, Name, true) / 2.f;
		Render::Line({ CntLine.x - TextSize.x - 10.f, CntLine.y }, { CurWnd.Pos.x + 4.f, CurWnd.Pos.y }, Render::Color(255, 255, 255));
		Render::Line({ CntLine.x + TextSize.x + 10.f, CntLine.y }, { CurWnd.Pos.x + CurWnd.Size.x - 4.f, CurWnd.Pos.y }, Render::Color(255, 255, 255));
		CurWnd.Pos.y += 8.f;

		MUTATE_END
	}

	__forceinline void Begin(const wchar_t* Name, Vector2& Pos, const const Vector2& Size, const Render::Color& Clr)
	{
		//MUTATE

		//base menu
		ProcessInput();
		if (!CurWnd.WndPos.Zero()) Pos = CurWnd.WndPos;
		Render::FillRoundedRectangle(Pos, Size, Clr, 8.f); 
		Render::String(CenterLine({ Pos.x, Pos.y + 10.f }), Name, true);
		CurWnd.WndPos = { Pos.x, Pos.y }; CurWnd.Size = Size;
		CurWnd.Pos = { Pos.x, Pos.y + 25.f };

		//MUTATE_END
	}
	
	void End()
	{
		MUTATE

		ProcessInput(true);

		//drag window
		if (InputMgr(E(L"##Drag")) || (IO.LKM == Pressed && InRect(CurWnd.WndPos, CurWnd.Size, IO.MousePos) && InputMgr(E(L"##Drag"), true)))
			CurWnd.WndPos += IO.MouseDelta;

		MUTATE_END
	}
}