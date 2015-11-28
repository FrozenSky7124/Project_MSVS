/*
¶àÌ¬°¸Àý
*/
#include <iostream>
using namespace std;

class HeroFighter
{
public:
	virtual int AttackPower()
	{
		return 10;
	}
};

class EnemyFighter
{
public:
	int DestoryPower()
	{
		return 15;
	}
};

class HeroFighter_Adv : public HeroFighter
{
public:
	virtual int AttackPower()
	{
		return 20;
	}
};

void openFire(HeroFighter *pHero, EnemyFighter *pEnemy)
{
	if (pHero->AttackPower() > pEnemy->DestoryPower())
	{
		cout << "EnemyFighter Down!" << endl;
	}
	else
	{
		cout << "HeroFighter Down!" << endl;
	}
}

void main3()
{
	HeroFighter h1;
	EnemyFighter e1;
	HeroFighter_Adv h2;
	/*
	if (h1.AttackPower() > e1.DestoryPower())
	{
		cout << "EnemyFighter Down!" << endl;
	}
	else
	{
		cout << "HeroFighter Down!" << endl;
	}
	*/
	openFire(&h1, &e1);
	openFire(&h2, &e1);
	system("pause");
}