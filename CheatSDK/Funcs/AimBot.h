//thx ApexBot for good prediction
#include "BulletPrediction.h"

//Aim Code
__forceinline void SmoothAngle(Vector3 src, Vector3& dst, float factor) {
	Vector3 delta = dst - src;
	Math::ClampAngle(delta);
	dst = src + delta / factor;
}

CBasePlayer* FindTarget(int& AimEntIndex, int& BestHitBox)
{
	MUTATE

	//set global data
	LPlayer* LP_Ent = LP; CBasePlayer* FOV_Target = nullptr;
	float f_Best = AimBot::FOV, d_Best = AimBot::Distance;
	if (!LP_Ent->Alive()) { AimEntIndex = -1; return nullptr; }

	//aim cache
	if (AimEntIndex != -1)
	{
		//check player
		CBasePlayer* CurEnt = Ent(AimEntIndex);
		if (CurEnt->Type(Player)) 
		{
			//check no knocked & on screen & visible
			auto HBoxPos = CurEnt->HitBoxPos(BestHitBox, false); Vector2 ScreenPos;
			if (!CurEnt->Knocked() && World2Screen(HBoxPos, ScreenPos) && CurEnt->VisiblePos(LP_Ent, HBoxPos)) {
				if (Math::GetFov(Global::WndSize / 2.f, ScreenPos
				/*LP->DynamicAngles(), Math::CalcAngle(LP->CameraPos(), HBoxPos)*/) < f_Best)
					return CurEnt;
			}
		}
	}

	//set invalid cache
	AimEntIndex = -1;

	//new find target
	for (int i = 0; i < 80; i++)
	{
		//get entity & check is player
		CBasePlayer* CurEnt = Ent(i);
		if (CurEnt->Type(Player))
		{
			//skip knocked
			if (AimBot::NoKnocked && CurEnt->Knocked())
				continue;

			//cut distance
			Vector3 BodyBonePos = CurEnt->HitBoxPos(3, false);
			float CurDist = Math::GameDist(BodyBonePos, LP->CameraPos());
			if (CurDist > AimBot::Distance) continue;

			//check head
			Vector3 HeadBonePos = CurEnt->HitBoxPos(0, false); Vector2 ScreenPos;
			if (World2Screen(HeadBonePos, ScreenPos) && CurEnt->VisiblePos(LP_Ent, HeadBonePos))
			{
				//calc fov
				float CurFOV = Math::GetFov(
					Global::WndSize / 2.f, ScreenPos
					/*LP->DynamicAngles(),
					Math::CalcAngle(LP->CameraPos(), HeadBonePos)*/);

				//set fov target
				if (CurFOV < f_Best) {
					FOV_Target = CurEnt;
					f_Best = CurFOV;
					AimEntIndex = i;
					BestHitBox = 0;
				}
			}

			//check body
			if (AimBot::Nearest && World2Screen(BodyBonePos, ScreenPos) && CurEnt->VisiblePos(LP_Ent, BodyBonePos))
			{
				//calc fov
				float CurFOV = Math::GetFov(
					Global::WndSize / 2.f, ScreenPos
					/*LP->DynamicAngles(),
					Math::CalcAngle(LP->CameraPos(), BodyBonePos)*/);

				//set fov target
				if (CurFOV < f_Best) {
					FOV_Target = CurEnt;
					f_Best = CurFOV;
					AimEntIndex = i;
					BestHitBox = 3;
				}
			}
		}
	}

	//set best target
	MUTATE_END
	return FOV_Target;
}

bool Aim(UserCmd* Cmd)
{
	MUTATE

	static int AimHitBox, AimID = -1;
	if (AimBot::Active && ((Cmd->buttons & 1) || (Cmd->buttons & 0x20000)))
	{
		//get weapon speed & gravity
		float BulletSpeed = LP->ActiveWeapon()->BulletSpeed(); //bullet speed (units)
		float BulletGrav = LP->ActiveWeapon()->BulletGravity(); //(gravity scale * 750.f)

		//is weapon check
		if ((BulletSpeed > 1.f) || (BulletGrav > 1.f))
		{
			//find target
			Vector3 AimAngles;
			CBasePlayer* AimTarget;
			if ((AimTarget = FindTarget(AimID, AimHitBox)))
			{
				//get target & muzzle positions
				Vector3 MuzzlePos = LP->CameraPos();//->ActiveWeapon()->MuzzlePos();
				Vector3 TargetPos = AimTarget->HitBoxPos(AimHitBox, true);

				//check need prediction
				if (BulletSpeed > 1.f)
				{
					//simple prediction
					if (AimBot::PredictType)
					{
						//compensate bullet drop 
						float VerticalTime = Math::Dist3D(TargetPos, MuzzlePos) / BulletSpeed;
						TargetPos.z += (750.f * LP->ActiveWeapon()->BulletGravity() * 0.5f) * (VerticalTime * VerticalTime);

						//compensate velocity
						float HorizontalTime = Math::Dist3D(TargetPos, MuzzlePos) / BulletSpeed;
						TargetPos += (AimTarget->AbsVelocity(true) * HorizontalTime);

						//calculate angle
						AimAngles = Math::CalcAngle(MuzzlePos, TargetPos);
					}

					else 
					{
						//set pred ctx
						PredictCtx Ctx; 
						Ctx.StartPos = MuzzlePos;
						Ctx.TargetPos = TargetPos; 
						Ctx.BulletSpeed = BulletSpeed;
						Ctx.BulletGravity = (BulletGrav * 750.f);
						Ctx.TargetVel = AimTarget->AbsVelocity();

						//target not hittable?!
						if (!BulletPredict(Ctx)) { AimID = -1; return false; }
						AimAngles = Vector3{ Ctx.AimAngles.x, Ctx.AimAngles.y, 0.f };
					}
				}

				else {
					//not needed bullet prediction
					AimAngles = Math::CalcAngle(MuzzlePos, TargetPos);
				}

				//remove sway & recoil
				if (AimBot::WeaponFix) {
					AimAngles -= LP->PunchAngle();
					AimAngles -= ((LP->DynamicAngles() - LP->PunchAngle()) - Cmd->viewangles);
				}

				Math::ClampAngle(AimAngles);

				//apply smooth (if need)
				if (!AimBot::Silent) {
					SmoothAngle(Cmd->viewangles, AimAngles, AimBot::Smooth);
					Math::ClampAngle(AimAngles);
				}

				Cmd->viewangles = AimAngles; 
				
				MUTATE_END
				return true;
			}
		}
	}

	//invalid
	AimID = -1;
	return false;
}