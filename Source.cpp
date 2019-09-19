#include"Utill.hpp"
#include"Hook.h"




class Ni2
{
public:
	void Init();
	void ShutDown();


private:
	//listCount Nino2.exe+1280Ec8
	uintptr_t listCount = 0;

	//heal Nino2.exe+11D9E20
	//enitiyList heal
	//0x718 * enityListCOunt + 388
	uintptr_t entityListHeal = 0;

};

void Ni2::Init()
{
	
}

void Ni2::ShutDown()
{
}
