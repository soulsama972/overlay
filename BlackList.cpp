#include "BlackList.hpp"
BlackList bList;


void __declspec(naked) GetViewMatrix()
{
	__asm
	{
		add edi, 0x138
		mov Overlay.viewMatrix, edi
		jmp bList.hViewMatrix.jumpBack
	}
}

void __declspec(naked) HAddOjbect()
{
	__asm
	{
		mov bList.object, eax
		mov eax, [eax + 0xFC]
		pushad
		pushf
	}
	bList.AddOjbect();
	__asm
	{
		popf
		popad
		jmp bList.hAddOjbect.jumpBack
	}
}

void __declspec(naked) GetAngleCemrea()
{
	__asm
	{
		mov bList.camera, esi;
		mov ecx, [esi + 0x000000E8]
		jmp bList.hcamera.jumpBack
	}

}

void BlackList::Init()
{
	base = Utill::getBaseAddr(L"Blacklist_DX11_game.exe");
	if (!base)
	{
		MessageBoxA(NULL, "error getting base address", "", MB_OK);
		exit(0);
	}
	Hook::midHook(hViewMatrix, base + offsetViewMatrix, GetViewMatrix, 6);
	Hook::insertHook(hViewMatrix);

	Hook::midHook(hAddOjbect, base + offsetObject, HAddOjbect, 6);
	Hook::insertHook(hAddOjbect);

	Hook::midHook(hcamera, base + offsetViewAngle, GetAngleCemrea, 6);
	Hook::insertHook(hcamera);

	pCam = (globalCamera*)(base + offsetGlobalCamera);
}

void BlackList::Clean()
{
	std::lock_guard<std::mutex> lck(Overlay.mu);

	Hook::unHooked(hViewMatrix);
	Hook::freeHook(hViewMatrix);


	Hook::unHooked(hAddOjbect);
	Hook::freeHook(hAddOjbect);

	Hook::unHooked(hcamera);
	Hook::freeHook(hcamera);
}



void BlackList::DrawESPBone()
{
	for (size_t i = 0; i < len; i++)
	{
		__try
		{
			char name[5] = { 0 };
			memcpy(name, oList[i]->skeleton->meshInstance->model->NameModel, 4);
			if (!strcmp("ESam", name))
			{
				if (oList[i]->playerPawn->basicWeapon)
					oList[i]->playerPawn->basicWeapon->weaponComponent->holdWeapon->ammoLeft = 99;
				oList[i]->playerPawn->pHealthSystem->currentHealth = oList[i]->playerPawn->pHealthSystem->maxHealth;
			/*	for (int j = 0; j < oList[i]->skeleton->meshLen; j++)
				{
					fVec2 p;
					wchar_t buffer[40];
					ZeroMemory(buffer, 40);
					swprintf(buffer, 40, L"%d",j);
					Overlay.WorldToScreen(oList[i]->skeleton->meshInstance->model->pMesh[j].pos, p, false);
					Overlay.pFontWrapper->DrawString(Overlay.devcon, buffer, 8, p.x,p.y, 0xff00ffff, FW1_TEXT_FLAG::FW1_RESTORESTATE);
				}*/
				continue;
			}
			if (oList[i]->playerPawn->pHealthSystem && oList[i]->playerPawn->pHealthSystem->currentHealth > 0)
			{
				if (oList[i]->skeleton->meshLen == 34)
					DrawDog(oList[i]->skeleton->meshInstance->model->pMesh);
				else if (oList[i]->skeleton->meshLen >=51)
					DrawMan(oList[i]->skeleton->meshInstance->model->pMesh);
				else if (oList[i]->skeleton->meshLen == 10)
					DrawDrone(oList[i]->skeleton->meshInstance->model->pMesh);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			continue;
		}
	}
	Overlay.DrawShapes();
	Overlay.rect.ClearInstance();
	Overlay.Line.ClearInstance();
}


void BlackList::AimBot()
{
	float temp = 0.0f,smallst = 9999999.0f;
	AUObject* player = 0;
	AUObject* enemy = 0;

	for (size_t i = 0; i < len; i++)
	{
		__try
		{
			char name[5] = { 0 };
			memcpy(name, oList[i]->skeleton->meshInstance->model->NameModel, 4);
			if (!strcmp("ESam", name))
			{
				player = oList[i];
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			continue;
		}
	}

	if (!player)
		return;

	for (size_t i = 0; i < len; i++)
	{
		__try
		{
			if (oList[i]->playerPawn->pHealthSystem->currentHealth > 0 && oList[i] != player)
			{
				temp = fVec3( oList[i]->playerPawn->oriPos - player->playerPawn->oriPos).GetLength();
				if (smallst > temp)
				{
					smallst = temp;
					enemy = oList[i];
				}
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			continue;
		}
	}
	if (!enemy)
		return;


	__try
	{
		fVec3 EnemyHead = fVec3(enemy->skeleton->meshInstance->model->pMesh[6].pos.x,
			enemy->skeleton->meshInstance->model->pMesh[6].pos.y,
			enemy->skeleton->meshInstance->model->pMesh[50].pos.z );
		fVec3 diff = EnemyHead - pCam->pAVAECamera->eyePos;
		float m = diff.GetLength();

		//ZeroMemory(buffer, 40);
		//swprintf(buffer, 20, L"angle: %f", angle);
		//Overlay.pFontWrapper->DrawString(Overlay.devcon, buffer, 15, 0, 400, 0xffff0000, FW1_TEXT_FLAG::FW1_RESTORESTATE);
		

		float pitch = asinf(diff.z / m)  * 180 / PI;
		float yaw = atan2f(diff.y, diff.x) * 180 / PI;
		
		camera->angle.x = ((int)(pitch / 360 * 65535));//pich 
		camera->angle.y = (int)(yaw / 360 * 65535);//yaw
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return;
	}
}

void BlackList::AddOjbect()
{
	for (size_t i = 0; i < len; i++)
	{
		if (oList[i] == object)
			return;
	}
	oList[len++] = object;
}

void BlackList::DrawDog(Mesh* mesh)
{
	fVec4 c = fVec4(0, 1, 1, 1);
	int legs[] = { 23, 22 ,21 ,20, 28, 24, 25, 26,27 };
	int hands[] = { 14, 13 ,12 ,11, 4, 17, 16 ,18, 19 };
	int oriId[] = {28,2,4,8 };

	fVec2 fLegs[9];
	fVec2 fHands[9];
	fVec2 fOri[4];
	for (int i = 0; i < 4; i++)
		if (!Overlay.WorldToScreen(mesh[oriId[i]].pos, fOri[i], false))
			return;
	for (int i = 0; i < 9; i++)
	{
		if (!Overlay.WorldToScreen(mesh[legs[i]].pos, fLegs[i], false))
			return;
		if (!Overlay.WorldToScreen(mesh[hands[i]].pos, fHands[i], false))
			return;
	}
	for (int i = 0; i < 8; i++)
	{
		Overlay.InsertLine(fLegs[i].x, fLegs[i].y, fLegs[i + 1].x, fLegs[i + 1].y, c);
		Overlay.InsertLine(fHands[i].x, fHands[i].y, fHands[i + 1].x, fHands[i + 1].y, c);
	}
	for (int i = 0; i < 3; i++)
		Overlay.InsertLine(fOri[i].x, fOri[i].y, fOri[i + 1].x, fOri[i + 1].y, c);
}

void BlackList::DrawMan(Mesh* mesh)
{
	fVec4 c = fVec4(0, 1, 1, 1);
	int legs[] = { 52, 51 ,50 ,49, 45, 46, 47, 48 };
	int hands[] = { 10 ,9 ,8 ,7 ,26 ,27 ,28 ,29 };
	int oriId[] = { 6,0 };

	fVec2 fLegs[8];
	fVec2 fHands[8];
	fVec2 fOri[2];
	for (int i = 0; i < 2; i++)
		if (!Overlay.WorldToScreen(mesh[oriId[i]].pos, fOri[i], false))
			return;
	for (int i = 0; i < 9; i++)
	{
		if (!Overlay.WorldToScreen(mesh[legs[i]].pos, fLegs[i], false))
			return;
		if (!Overlay.WorldToScreen(mesh[hands[i]].pos, fHands[i], false))
			return;
	}
	for (int i = 0; i < 7; i++)
	{
		Overlay.InsertLine(fLegs[i].x, fLegs[i].y, fLegs[i + 1].x, fLegs[i + 1].y, c);
		Overlay.InsertLine(fHands[i].x, fHands[i].y, fHands[i + 1].x, fHands[i + 1].y, c);
	}
	Overlay.InsertLine(fOri[0].x, fOri[0].y, fOri[1].x, fOri[1].y, c);
}

void BlackList::DrawDrone(Mesh* mesh)
{
	fVec4 c = fVec4(0, 1, 1, 1);
	int left[] = { 1,2,3,4 };
	int right[] = { 5,6,7,8 };
	int ori[] = { 9,0 };
	fVec2 fLeft[4];
	fVec2 fRight[4];
	fVec2 fOri[2];

	for (int i = 0; i < 2; i++)
		if (!Overlay.WorldToScreen(mesh[ori[i]].pos, fOri[i], false))
			return;

	for (int i = 0; i < 4; i++)
		if (!Overlay.WorldToScreen(mesh[left[i]].pos, fLeft[i], false))
			return;

	for (int i = 0; i < 4; i++)
		if (!Overlay.WorldToScreen(mesh[right[i]].pos, fRight[i], false))
			return;

	for (int i = 0; i < 3; i++)
	{
		Overlay.InsertLine(fLeft[i].x, fLeft[i].y, fLeft[i + 1].x, fLeft[i + 1].y, c);
		Overlay.InsertLine(fRight[i].x, fRight[i].y, fRight[i + 1].x, fRight[i + 1].y, c);
	}
	Overlay.InsertLine(fLeft[0].x, fLeft[0].y, fRight[0].x, fRight[0].y, c);
	Overlay.InsertLine(fLeft[3].x, fLeft[3].y, fRight[3].x, fRight[3].y, c);
		
}






