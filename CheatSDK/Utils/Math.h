class Vector3
{
public:
	float x, y, z;

	__forceinline Vector3() {
		x = y = z = 0.0f;
	}

	__forceinline Vector3(float X, float Y, float Z) {
		x = X; y = Y; z = Z;
	}

	__forceinline float operator[](int i) const {
		return ((float*)this)[i];
	}

	__forceinline Vector3& operator+=(float v) {
		x += v; y += v; z += v; return *this;
	}

	__forceinline Vector3& operator-=(float v) {
		x -= v; y -= v; z -= v; return *this;
	}

	__forceinline Vector3& operator-=(const Vector3& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	__forceinline Vector3 operator*(float v) const {
		return Vector3(x * v, y * v, z * v);
	}

	__forceinline Vector3 operator/(float v) const {
		return Vector3(x / v, y / v, z / v);
	}

	__forceinline Vector3& operator+=(const Vector3& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	__forceinline Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	__forceinline Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	__forceinline Vector3& operator/=(float v) {
		x /= v; y /= v; z /= v; return *this;
	}

	__forceinline bool Zero() const {
		return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f && z > -0.1f && z < 0.1f);
	}
};

class Vector2 {
public:
	float x, y;

	__forceinline Vector2() {
		x = y = 0.0f;
	}

	__forceinline Vector2(float X, float Y) {
		x = X; y = Y;
	}

	__forceinline Vector2 operator+(float v) const {
		return Vector2(x + v, y + v);
	}

	__forceinline Vector2 operator-(float v) const {
		return Vector2(x - v, y - v);
	}

	__forceinline Vector2& operator+=(float v) {
		x += v; y += v; return *this;
	}

	__forceinline Vector2& operator*=(float v) {
		x *= v; y *= v; return *this;
	}

	__forceinline Vector2& operator/=(float v) {
		x /= v; y /= v; return *this;
	}

	__forceinline Vector2 operator-(const Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}

	__forceinline Vector2 operator+(const Vector2 & v) const {
		return Vector2(x + v.x, y + v.y);
	}

	__forceinline Vector2& operator+=(const Vector2& v) {
		x += v.x; y += v.y; return *this;
	}

	__forceinline Vector2& operator-=(const Vector2& v) {
		x -= v.x; y -= v.y; return *this;
	}

	__forceinline Vector2 operator/(float v) const {
		return Vector2(x / v, y / v);
	}

	__forceinline Vector2 operator*(float v) const {
		return Vector2(x * v, y * v);
	}

	__forceinline Vector2 operator/(const Vector2& v) const {
		return Vector2(x / v.x, y / v.y);
	}

	__forceinline bool Zero() const {
		return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f);
	}
};

class Matrix3x4
{
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
		};

		float m[3][4];
		float mm[12];
	};
};

class Matrix4x4
{
public:
	float m[4][4];
};

namespace Math
{
	#define M_PI 3.14159265358979323846f
	#define M_RADPI	57.295779513082f
	#define M_PI_F ((float)(M_PI))
	#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI_F))
	#define DEG2RAD(x) ((float)(x) * (float)(M_PI_F / 180.f))

	//#define sinf(a) ((float)FC(ntdll, sin, (double)(a)))
	#define cosf(a) ((float)FC(ucrtbase, cosf, (float)(a)))
	#define atanf(a) ((float)FC(ucrtbase, atanf, (float)(a)))
	#define atan2f(a, b) ((float)FC(ucrtbase, atan2f, (float)(a), (float)(b)))

	#define powf(a, b) ((a) * (a))
	//#define F(A) (A)

	/*float acosf(float x) {
		float negate = float(x < 0);
		x = abs(x);
		float ret = -0.0187293;
		ret = ret * x;
		ret = ret + 0.0742610;
		ret = ret * x;
		ret = ret - 0.2121144;
		ret = ret * x;
		ret = ret + 1.5707288;
		ret = ret * sqrt(1.0 - x);
		ret = ret - 2 * negate * ret;
		return negate * 3.14159265358979 + ret;
	}*/

	__forceinline float FastSQRT(float x) {
		union { int i; float x; } u;
		u.x = x; u.i = (u.i >> 1) + 0x1FC00000;
		u.x = u.x + x / u.x;
		return .25f * u.x + x / u.x;
	}

	__forceinline float FastDist2D(const Vector2& Src, const Vector2& Dst) {
		return FastSQRT(powf(Src.x - Dst.x, 2.f) + powf(Src.y - Dst.y, 2.f));
	}

	__forceinline float FastDist3D(const Vector3& Src, const Vector3& Dst) {
		return FastSQRT(powf(Src.x - Dst.x, 2.f) + powf(Src.y - Dst.y, 2.f) + powf(Src.z - Dst.z, 2.f));
	}

	__forceinline float GameDist(const Vector3& Src, const Vector3& Dst) {
		return FastDist3D(Src, Dst) * 0.01905f;
	}

	__forceinline float Dist3D(const Vector3& Src, const Vector3& Dst) {
		return sqrtf(powf(Src.x - Dst.x, 2.f) + powf(Src.y - Dst.y, 2.f) + powf(Src.z - Dst.z, 2.f));
	}

	__forceinline float Vec3Length(const Vector3& Src) {
		return sqrtf(powf(Src.x, 2.f) + powf(Src.y, 2.f) + powf(Src.z, 2.f));
	}

	__forceinline void ClampAngle(Vector3& Ang) {
		if (Ang.x < -89.f) Ang.x = -89.f;
		if (Ang.x > 89.f) Ang.x = 89.f;
		while (Ang.y < -180.f) Ang.y += 360.f;
		while (Ang.y > 180.f) Ang.y -= 360.f;
		Ang.z = 0.f;
	}

	/*__forceinline float atan2(float y, float x)
	{
		float t0, t1, t3, t4;

		t3 = abs(x);
		t1 = abs(y);
		t0 = max(t3, t1);
		t1 = min(t3, t1);
		t3 = F(1.f) / t0;
		t3 = t1 * t3;

		t4 = t3 * t3;
		t0 = F(-0.013480470f);
		t0 = t0 * t4 + F(0.057477314f);
		t0 = t0 * t4 - F(0.121239071f);
		t0 = t0 * t4 + F(0.195635925f);
		t0 = t0 * t4 - F(0.332994597f);
		t0 = t0 * t4 + F(0.999995630f);
		t3 = t0 * t3;

		t3 = (abs(y) > abs(x)) ? F(1.570796327f) - t3 : t3;
		t3 = (x < 0) ? M_PI - t3 : t3;
		t3 = (y < 0) ? -t3 : t3;

		return t3;
	}

	float atan(float x) {
		return atan2(x, float(1));
	}*/

	Vector3 CalcAngle(const Vector3& Src, const Vector3& Dst)
	{
		MUTATE

		Vector3 Delta = Src - Dst, AimAngles;
		float Hyp = sqrtf(powf(Delta.x, 2.f) + powf(Delta.y, 2.f));
		AimAngles.y = atanf(Delta.y / Delta.x) * M_RADPI;
		AimAngles.x = atanf(Delta.z / Hyp) * M_RADPI;
		if (Delta.x >= 0.f) AimAngles.y += 180.f;
		AimAngles.z = 0.f; 
		
		MUTATE_END
		return AimAngles;
	}
	
	__forceinline void VectorNormalize(Vector3& Src)
	{
		float l = Vec3Length(Src);
		if (l != 0.0f)
		{
			Src /= l;
		}
		else
		{
			Src.x = Src.y = 0.0f; Src.z = 1.0f;
		}
	}

	/*float acos(float x) {
		float negate = float(x < 0);
		x = abs(x);
		float ret = -0.0187293;
		ret = ret * x;
		ret = ret + 0.0742610;
		ret = ret * x;
		ret = ret - 0.2121144;
		ret = ret * x;
		ret = ret + 1.5707288;
		ret = ret * sqrt(1.0 - x);
		ret = ret - 2 * negate * ret;
		return negate * 3.14159265358979 + ret;
	}
	
	float sin(float x)
	{
		const float a0 = +1.91059300966915117e-31f;
		const float a1 = +1.00086760103908896f;
		const float a2 = -1.21276126894734565e-2f;
		const float a3 = -1.38078780785773762e-1f;
		const float a4 = -2.67353392911981221e-2f;
		const float a5 = +2.08026600266304389e-2f;
		const float a6 = -3.03996055049204407e-3f;
		const float a7 = +1.38235642404333740e-4f;
		return a0 + x * (a1 + x * (a2 + x * (a3 + x * (a4 + x * (a5 + x * (a6 + x * a7))))));
	}*/

	/*__forceinline float CosAdd(float x) {
		float x2 = x * x;
		const float c1 = 0.99940307f;
		const float c2 = -0.49558072f;
		const float c3 = 0.03679168f;
		return (c1 + x2 * (c2 + c3 * x2));
	}

	float cos(float angle) {
		angle = angle - floorf(angle * 0.15f) * 6.28f;
		angle = angle > 0.f ? angle : -angle;
		if (angle < 1.57f) return CosAdd(angle);
		if (angle < 3.14f) return -CosAdd(3.14f - angle);
		if (angle < 4.71f) return -CosAdd(angle - 3.14f);
		return CosAdd(6.28f - angle);
	}*/

	void MakeVector(Vector3 angle, Vector3& vector) 
	{
		MUTATE

		float pitch = float(angle[0] * M_PI / 180);
		float yaw = float(angle[1] * M_PI / 180);
		float tmp = float(cosf(pitch));
		vector.x = float(-tmp * -cosf(yaw));
		vector.y = float(sinf(yaw) * tmp);
		vector.z = float(-sinf(pitch));

		MUTATE_END
	}

	__forceinline float Dot(const Vector3& v1, const Vector3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float GetFov(const Vector2& ScrCenter, const Vector2& ScreenPos/*const Vector3& viewAngle, const Vector3& aimAngle*/)
	{
		return Math::FastDist2D(ScrCenter, ScreenPos);

		//MUTATE

		//Vector3 ang, aim; MakeVector(viewAngle, aim); MakeVector(aimAngle, ang);
		//float mag = (float)FastSQRT(powf(aim.x, 2) + powf(aim.y, 2) + powf(aim.z, 2));
		//float u_dot_v = Dot(aim, ang);
		//auto fov = RAD2DEG(acosf(u_dot_v / (pow(mag, 2))));

		//MUTATE_END
		//return ((fov < 0) ? 0.f : fov); //fix pls
	}
}