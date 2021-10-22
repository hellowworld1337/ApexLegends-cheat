struct PredictCtx
{
	//input data
	Vector3 StartPos;
	Vector3 TargetPos;
	Vector3 TargetVel;
	float BulletSpeed;
	float BulletGravity;

	//out data
	Vector2 AimAngles;
};

__forceinline Vector3 ExtrapolatePos(const PredictCtx& Ctx, float Time) {
	return Ctx.TargetPos + (Ctx.TargetVel * Time);
}

bool OptimalPitch(const PredictCtx& Ctx, const Vector2& Dir2D, float* OutPitch) 
{
	MUTATE
	float Vel = Ctx.BulletSpeed, Grav = Ctx.BulletGravity, DirX = Dir2D.x, DirY = Dir2D.y;
	float Root = Vel * Vel * Vel * Vel - Grav * (Grav * DirX * DirX + 2.f * DirY * Vel * Vel);
	if (Root >= 0.f) { 
		*OutPitch = atanf((Vel * Vel - sqrt(Root)) / (Grav * DirX)); 
		MUTATE_END
		return true;
	}
	
	return false;
}

bool SolveTrajectory(PredictCtx& Ctx, const Vector3& ExtrPos, float* TravelTime)
{
	MUTATE
	Vector3 Dir = ExtrPos - Ctx.StartPos;
	Vector2 Dir2D = { sqrtf(Dir.x * Dir.x + Dir.y * Dir.y), Dir.z };

	float CurPitch;
	if (!OptimalPitch(Ctx, Dir2D, &CurPitch)) {
		return false;
	}

	*TravelTime = Dir2D.x / (cosf(CurPitch) * Ctx.BulletSpeed);
	Ctx.AimAngles.y = atan2f(Dir.y, Dir.x);
	Ctx.AimAngles.x = CurPitch;

	MUTATE_END
	return true;
}

bool BulletPredict(PredictCtx& Ctx)
{
	const float MAX_TIME = 1.f, TIME_STEP = (1.f / 128.f/*256.f*/);
	for (float CurrentTime = 0.f; CurrentTime <= MAX_TIME; CurrentTime += TIME_STEP)
	{
		float TravelTime;
		Vector3 ExtrPos = ExtrapolatePos(Ctx, CurrentTime);
		if (!SolveTrajectory(Ctx, ExtrPos, &TravelTime)) {
			return false;
		}

		if (TravelTime < CurrentTime) {
			Ctx.AimAngles = { -RAD2DEG(Ctx.AimAngles.x), RAD2DEG(Ctx.AimAngles.y) };
			return true;
		}
	}
}