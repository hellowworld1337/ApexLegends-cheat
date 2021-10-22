class GlobalVars
{
public:
	float realtime; //0x0000
	int32_t framecount; //0x0004
	float absoluteframetime; //0x0008
	float curtime; //0x000C
	float frametime; //0x0010
	int32_t tickcount; //0x0014
	float Unk1; //0x0018
	float interval_per_tick; //0x001C
	float interpolation_amount; //0x0020
	int32_t simTicksThisFrame; //0x0024
	int32_t network_protocol; //0x0028
}; //Size: 0x002C

class UserCmd
{
public:
	int command_number;
	int tick_count;
	float command_time;
	Vector3 viewangles;
	BYTE pad1[0xC];
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	BYTE pad2[0x24C];
};

struct kbutton_t {
	int down[2];
	int state;
};

class Ray
{
public:
	BYTE RayRaw[0x68];
	/*__forceinline Ray(const Vector3& Start, const Vector3& End) {
		SpoofCall(Fn::RayInit, RayRaw, &Start, &End);
	}*/
};

struct surface_t
{
	const char*		name;				//0x0000
	int				surfaceProp;		//0x0008
	unsigned short	flags;				//0x000C
};

struct plane_t
{
	Vector3	surfaceNormal;	//0x0000
	float	dist;			//0x000C
};

struct TraceData
{
	Vector3 startpos;
	int unk0;
	Vector3 endpos;
	int unk01;
	plane_t plane;
	float fraction;
	int contents;
	unsigned short dispFlags;
	bool allsolid;
	bool startsolid;
	int unk02;
	float fractionLeftSolid;
	int unk03;
	surface_t surface;
	int hitgroup;
	short physicsBone;
	unsigned short m_nWorldSurfaceIndex;
	void* m_pEnt;
	int hitbox;
	char pad[0x114];
};

bool LKM_State() {
	return bool(*(int*)(GetModuleBase(nullptr) + Off::lkmState) & 3);
}

bool RKM_State() {
	return bool(*(int*)(GetModuleBase(nullptr) + Off::rkmState) & 3);
}

bool World2Screen(const Vector3& Position, Vector2& ScreenPosition)
{
	MUTATE

	DWORD64 ViewRender = *(DWORD64*)Off::ViewRender; 
	if (!ViewRender) return false;

	Matrix4x4* m_Matrix = *(Matrix4x4**)(ViewRender + Off::ViewMatrix); 
	if (!m_Matrix) return false;

	float w = 0.f;
	ScreenPosition.x = (m_Matrix->m[0][0] * Position.x) + (m_Matrix->m[0][1] * Position.y) + (m_Matrix->m[0][2] * Position.z + m_Matrix->m[0][3]);
	ScreenPosition.y = (m_Matrix->m[1][0] * Position.x) + (m_Matrix->m[1][1] * Position.y) + (m_Matrix->m[1][2] * Position.z + m_Matrix->m[1][3]);
	w = (m_Matrix->m[3][0] * Position.x) + (m_Matrix->m[3][1] * Position.y) + (m_Matrix->m[3][2] * Position.z + m_Matrix->m[3][3]);

	if (w < 0.01f)
		return false;

	float invw = 1.f / w;
	ScreenPosition.x *= invw;
	ScreenPosition.y *= invw;

	float x = Global::WndSize.x / 2.f;
	float y = Global::WndSize.y / 2.f;
	x += .5f * ScreenPosition.x * Global::WndSize.x + .5f;
	y -= .5f * ScreenPosition.y * Global::WndSize.y + .5f;
	ScreenPosition.x = x; ScreenPosition.y = y; 
	
	MUTATE_END
	return true;
}

__int64 __fastcall InitRay(__int64 a1, float* a2, float* a3)
{
	float v3; // xmm1_4
	float v4; // xmm2_4
	float v5; // xmm0_4
	__int64 result; // rax

	v3 = *a3 - *a2;
	*(float*)(a1 + 16) = v3;
	v4 = a3[1] - a2[1];
	*(float*)(a1 + 20) = v4;
	v5 = a3[2] - a2[2];
	*(float*)(a1 + 24) = v5;
	*(_BYTE*)(a1 + 93) = (float)((float)((float)(v4 * v4) + (float)(v3 * v3)) + (float)(v5 * v5)) != 0.0;
	*(_QWORD*)(a1 + 52) = 0i64;
	*(_DWORD*)(a1 + 48) = 0;
	*(_QWORD*)(a1 + 60) = 0i64;
	*(_DWORD*)(a1 + 68) = 0;
	*(_QWORD*)(a1 + 72) = 1065353216i64;
	*(_QWORD*)(a1 + 80) = 0i64;
	*(_BYTE*)(a1 + 92) = 1;
	*(_DWORD*)(a1 + 88) = 0;
	*(_QWORD*)(a1 + 96) = 0i64;
	*(_QWORD*)(a1 + 36) = 0i64;
	*(_DWORD*)(a1 + 32) = 0;
	*(float*)a1 = *a2;
	*(float*)(a1 + 4) = a2[1];
	result = *((unsigned int*)a2 + 2);
	*(_QWORD*)(a1 + 8) = (unsigned int)result;
	*(_DWORD*)(a1 + 28) = 0;
	*(_DWORD*)(a1 + 44) = 0;
	return result;
}