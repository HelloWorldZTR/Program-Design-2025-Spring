// 数据里面没有出现狼由于达到武器数目限制而无法全部抢走的情况
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iomanip>
const double EPS = 1e-6;
using namespace std;
enum EEventType
{
    EVENT_BORN,
    EVENT_LION_RUN,
    EVENT_PICKUP,
    EVENT_MARCH,
    EVENT_ARROW,
    EVENT_BOMB,
    EVENT_ATTACK,
    EVENT_FIGHT_BACK,
    EVENT_DEAD,
    EVENT_YELL,
    EVENT_EARNMONEY,
    EVENT_FLAG,
    EVENT_REACH,
    EVENT_CITYTAKEN,
    EVENT_PRINTMONEY,
    EVENT_WARRIOR_REPORT
};
enum
{
    WEAPON_SWORD,
    WEAPON_BOMB,
    WEAPON_ARROW
};
enum
{
    DRAGON,
    NINJA,
    ICEMAN,
    LION,
    WOLF
};

#define WEAPON_NUM 3
#define WARRIOR_NUM 5

#define COLOR_RED 0
#define COLOR_BLUE 1
#define COLOR_NONE 3

class CHeadquarter;
class CKingdom;
class CWarrior;
class CWeapon;

class CHeadquarter
{

    friend class CWarrior;
    friend class CWolf;
    friend class CDragon;
    friend class CIceman;
    friend class CLion;
    friend class CNinja;

private:
    int nBloodMoney; // addfor 2010 纪录打仗赢后得到的
    int nMoney;
    int nWarriorNo;
    list<CWarrior *> lstWarrior;
    int nColor;
    CKingdom *pKingdom;
    int nCityNo; // red: 0 ,blue nCitynum + 1
    CHeadquarter *pEnemyheadquarter;
    vector<CWarrior *> vAwardList;
    int nWarriorInEnemyHeadquarter = 0;

public:
    static int MakingSeq[2][WARRIOR_NUM];
    CHeadquarter(int nColor_, int nMoney_, int nCityNo_) : nColor(nColor_), nWarriorNo(1),
                                                           nMoney(nMoney_), nCityNo(nCityNo_), nBloodMoney(0)
    {
    }
    ~CHeadquarter()
    {
        list<CWarrior *>::iterator p;
        for (p = lstWarrior.begin(); p != lstWarrior.end(); p++)
            delete (*p);
        lstWarrior.clear();
    }
    void SetEnemy(CHeadquarter *p)
    {
        pEnemyheadquarter = p;
    }
    void SetKingdom(CKingdom *p)
    {
        pKingdom = p;
    }
    void AddEvent(EEventType eType, int nCityNo, int nColor, const string &sEventString);
    void PrintMoney();
    string GetColorStr()
    {
        if (nColor == COLOR_RED)
            return "red";
        else
            return "blue";
    }
    int GetColor()
    {
        return nColor;
    }
    void LionRunaway();
    void WarriorBorn();
    void WarriorsMarch(int nEnemyHeadquterCityNo);
    void WarriorsAttack();
    void PickUpLife();
    void HandleArrow();
    void HandleBomb();
    void CleanUpBattleField();
    void WarriorsReport();
    void EnemyReach();
    void DeleteDeadWarriors();
    CWarrior *QueryCityWarrior(int nCityNo);
    CWarrior *QueryNextCityWarrior(int nCityNo);
};
class CWarrior
{
public:
    static const int MAX_WPS = 3;

    friend class CHeadquarter;

protected:
    int nStrength;
    int nForce;
    int nCityNo;
    int nId;
    CHeadquarter *pHeadquarter;
    CWeapon *weapons[MAX_WPS];

public:
    static const char *Names[WARRIOR_NUM];
    static int InitialLifeValue[WARRIOR_NUM];
    static int InitalForce[WARRIOR_NUM];
    virtual bool Runaway() { return false; }
    virtual ~CWarrior();
    virtual int GetAttackDamage();
    virtual int GetFightBackDamage();
    virtual void Attack(CWarrior *pEnemy); 
    virtual void FightBack(CWarrior *pEnemy);
    virtual string GetName() = 0;
    virtual void March() {
        if (GetColor() == COLOR_RED)
            nCityNo++;
        else
            nCityNo--;
    }

    CWarrior(int nId_, int nStrength_, int nForce_, int nCityNo_, CHeadquarter *pHeadquarter_) : nId(nId_), nStrength(nStrength_), nForce(nForce_), nCityNo(nCityNo_), pHeadquarter(pHeadquarter_)
    {
        memset(weapons, 0, sizeof(weapons));
    }
    int GetStrength() { return nStrength; }
    int GetForce() { return nForce; }
    void SetStrength(int n) { nStrength = n; }
    void SetForce(int n) { nForce = n; }
    int GetPosition() { return nCityNo; }
    int GetColor() const { return pHeadquarter->GetColor(); }
    string GetColorStr() { return pHeadquarter->GetColorStr();}
    string ReportStatus();
};
class CEvent
{
private:
    EEventType eEventType;
    int nTime;
    int nCityNo;
    int nColor;
    string sDescribe;

public:
    CEvent(EEventType eEventType_, int nTime_, int nCityNo_, int nColor_, const string &s) : eEventType(eEventType_), nTime(nTime_), nCityNo(nCityNo_), nColor(nColor_), sDescribe(s)
    {
    }
    void Output()
    {
        char szTime[20];
        sprintf(szTime, "%03d:%02d", nTime / 60, nTime % 60);
        cout << szTime << " " << sDescribe << endl;
    }
    bool operator<(const CEvent &e2) const
    {
        if (nTime < e2.nTime)
            return true;
        else if (nTime > e2.nTime)
            return false;

        if( eEventType == e2.eEventType && eEventType == EVENT_WARRIOR_REPORT) { // Special case
            if( nColor < e2.nColor )
                return true;
            else if( nColor == e2.nColor)
                return nCityNo < e2.nCityNo ;
            else
                return false;
        }

        if (nCityNo < e2.nCityNo)
            return true;
        else if (nCityNo > e2.nCityNo)
            return false;

        if (eEventType < e2.eEventType)
            return true;
        else if (eEventType > e2.eEventType)
            return false;
        if (nColor < e2.nColor)
            return true;
        else
            return false;
    }
};
class CKingdom
{
    friend class CHeadquarter;

private:
    CHeadquarter Red, Blue;
    int nTimeInMinutes;
    vector<CEvent> vEvent;
    int nEndTime;
    int nCityNum;
    int lifeValue[25];
    int flags[25];
    int prevWinner[25];

public:
    void Run(int T)
    {
        for (int t = 0; t <= T; t++)
        { // modifor 2010 old: t < T
            if (TimePass(t) == 0)
                return;
        }
    }
    CKingdom(int nCityNum_, int nInitMoney) : nTimeInMinutes(0), Red(COLOR_RED, nInitMoney, 0), Blue(COLOR_BLUE, nInitMoney, nCityNum_ + 1),
                                              nCityNum(nCityNum_)
    {
        Red.SetKingdom(this);
        Red.SetEnemy(&Blue);
        Blue.SetKingdom(this);
        Blue.SetEnemy(&Red);
        nEndTime = 3000000;
        memset(lifeValue, 0, sizeof(lifeValue));
        memset(flags, -1, sizeof(flags));
        memset(prevWinner, -1, sizeof(prevWinner));
    }
    int TimePass(int nMinutes);
    string SysemTimeStr()
    {
        char szTime[20];
        sprintf(szTime, "%03d:%02d", nTimeInMinutes / 60, nTimeInMinutes % 60);
        return szTime;
    }
    int GetTime()
    {
        return nTimeInMinutes;
    }
    void WarEnd()
    {
        nEndTime = nTimeInMinutes;
        //FIXME: What does this mean????
        // if (nEndTime == 3000000)
        //     nEndTime = nTimeInMinutes;
    }
    void OutputResult()
    {
        sort(vEvent.begin(), vEvent.end());
        for (int i = 0; i < vEvent.size(); i++)
            vEvent[i].Output();
    }
    void AddEvent(EEventType eType, int nCityNo, int nColor, const string &sEventString)
    {
        CEvent tmp(eType, nTimeInMinutes, nCityNo, nColor, sEventString);
        vEvent.push_back(tmp);
        // addfor debug
        //	tmp.Output();
        // gwend
    }
};


class CWeapon
{
public:
    int nKindNo;
    friend class CWarrior;
    CWarrior *master;
    static const char *Names[WEAPON_NUM];

    virtual int GetForce() {};
    virtual void Use() {};
    static CWeapon *NewWeapon(int idx, CWarrior *master);
    CWeapon(CWarrior *m) : master(m) {}
};
class CSword : public CWeapon
{
public:
    int power;
public:
    virtual void Use()
    {
        power *= 0.8;
    }
    virtual int GetForce()
    {
        return power;
    }
    CSword(CWarrior *m) : CWeapon(m) { 
        nKindNo = 0; 
        power = m->GetForce() * 0.2; 
    }
};
class CArrow : public CWeapon
{
public:
    int usedTimes;
    int power;

public:
    static int initialPower;

public:
    CArrow(CWarrior *master) : CWeapon(master), usedTimes(0)
    {
        nKindNo = 2;
        power = initialPower;
    }
    virtual int GetForce(CWarrior *pEnemy)
    {
        return power;
    }
    void Use()
    {
        usedTimes++;
        if (usedTimes == 3) {
            power = 0;
        }
    }
};
class CBomb : public CWeapon
{
public:
    CBomb(CWarrior *master) : CWeapon(master)
    {
        nKindNo = 1;
    }
};
CWeapon *CWeapon::NewWeapon(int idx, CWarrior *master)
{
    switch (idx)
    {
    case WEAPON_SWORD:
        return new CSword(master);
    case WEAPON_BOMB:
        return new CBomb(master);
    case WEAPON_ARROW:
        return new CArrow(master);
    default:
        return NULL;
    }
}



typedef CWarrior *PWARRIOR;

string MyIntToStr(int n)
{
    char szTmp[300];
    sprintf(szTmp, "%d", n);
    return szTmp;
}

class CNinja : public CWarrior
{
    friend class CHeadquarter;

public:
    // CNinja constructor
    CNinja(int nId_, int nStrength_, int nForce_, int nCityNo_, CHeadquarter *pHeadquarter_) : CWarrior(nId_, nStrength_, nForce_, nCityNo_, pHeadquarter_)
    {
        int wp1 = nId % 3;
        int wp2 = (nId + 1) % 3;
        weapons[wp1] = CWeapon::NewWeapon(wp1, this);
        weapons[wp2] = CWeapon::NewWeapon(wp2, this);
    }
    virtual string GetName()
    {
        return pHeadquarter->GetColorStr() + " ninja " + MyIntToStr(nId);
    }
    virtual int GetFightBackDamage()
    {
        return 0;
    }
    virtual void FightBack(CWarrior *pEnemy)
    {
    }
};
class CDragon : public CWarrior
{
    friend class CHeadquarter;

public:
    // CDragon Constructor
    double fMorale;
    CDragon(int nId_, int nStrength_, int nForce_, int nCityNo_, double fMorale, CHeadquarter *pHeadquarter_) : CWarrior(nId_, nStrength_, nForce_, nCityNo_, pHeadquarter_), fMorale(fMorale)
    {
        weapons[nId % 3] = CWeapon::NewWeapon(nId % 3, this);
    }
    virtual void Attack(CWarrior *p)
    {
        CWarrior::Attack(p);
    }
    virtual void FightBack(CWarrior *pEnemy)
    {
        CWarrior::FightBack(pEnemy);
    }
    virtual string GetName() {
        return pHeadquarter->GetColorStr() + " dragon " + MyIntToStr(nId);
    }
};
class CLion : public CWarrior
{
    friend class CHeadquarter;

private:
    int nLoyalty;

public:
    static int nLoyaltyDec;
    // CLion Constructor
    CLion(int nId_, int nStrength_, int nForce_, int nCityNo_, CHeadquarter *pHeadquarter_) : CWarrior(nId_, nStrength_, nForce_, nCityNo_, pHeadquarter_)
    {
        nLoyalty = pHeadquarter->nMoney;
    }
    virtual string GetName() {
        return pHeadquarter->GetColorStr() + " lion " + MyIntToStr(nId);
    }
    bool Runaway()
    {
        return nLoyalty <= 0;
    }
};
class CIceman : public CWarrior
{
    friend class CHeadquarter;
    int stepCnt = 0;
public:
    // CIceman constructor
    CIceman(int nId_, int nStrength_, int nForce_, int nCityNo_, CHeadquarter *pHeadquarter_) : CWarrior(nId_, nStrength_, nForce_, nCityNo_, pHeadquarter_)
    {
        weapons[nId % 3] = CWeapon::NewWeapon(nId % 3, this);
    }
    virtual void March() {
        int enemyCityNo = pHeadquarter->pEnemyheadquarter->nCityNo;
        if (nCityNo != enemyCityNo) {
                stepCnt++;
            if (stepCnt % 2 == 0) {
                nStrength -= 9;
                if (nStrength <= 0)
                    nStrength = 1;
                nForce += 20;
            }
        }
        CWarrior::March();
    }
    virtual string GetName() {
        return pHeadquarter->GetColorStr() + " iceman " + MyIntToStr(nId);
    }
};
class CWolf : public CWarrior
{
    friend class CHeadquarter;

public:
    // CWolf Constructor
    CWolf(int nId_, int nStrength_, int nForce_, int nCityNo_, CHeadquarter *pHeadquarter_) : CWarrior(nId_, nStrength_, nForce_, nCityNo_, pHeadquarter_)
    {
        //no weapons
    }
    virtual string GetName() {
        return pHeadquarter->GetColorStr() + " wolf " + MyIntToStr(nId);
    }
};

/**
 * Class CHeadquarter
 */
void CHeadquarter::LionRunaway()
{

    string sEventString;
    list<CWarrior *>::iterator i = lstWarrior.begin();
    while (i != lstWarrior.end())
    {
        if ((*i)->Runaway())
        {
            // 输出样例： 000:05 blue lion 1 ran away
            int nCityNo = (*i)->GetPosition();
            if (nColor == COLOR_RED && nCityNo == pKingdom->nCityNum + 1 ||
                nColor == COLOR_BLUE && nCityNo == 0)
                continue;
            sEventString = (*i)->GetName() + " ran away";
            AddEvent(EVENT_LION_RUN, (*i)->nCityNo, nColor, sEventString);
            i = lstWarrior.erase(i); // 指向被erased的元素的后一个
            continue;
        }
        i++;
    }
}
int CKingdom::TimePass(int nMinutes)
{
    int i;
    nTimeInMinutes = nMinutes;
    if (nTimeInMinutes > nEndTime)
        return 0;
    int nRemain = nTimeInMinutes % 60;
    switch (nRemain)
    {
    case 0: // 生产怪物
        Red.WarriorBorn();
        Blue.WarriorBorn();
        break;
    case 5: // lion可能逃跑
        Red.LionRunaway();
        Blue.LionRunaway();
        break;
    case 10: // 前进
        Red.WarriorsMarch(nCityNum + 1);
        Blue.WarriorsMarch(0);
        break;
    case 20: // Generate life
        for (int i = 1; i <= nCityNum; i++)
        {
            lifeValue[i] += 10;
        }
        break;
    case 30:
        Red.PickUpLife();
        Blue.PickUpLife();
        break;
    case 35:
        Red.HandleArrow();
        Blue.HandleArrow();
        break;
    case 38:
        Red.HandleBomb();
        Blue.HandleBomb();
        break;
    case 40: // 发生战斗
        Red.WarriorsAttack();
        Blue.WarriorsAttack();
        Red.CleanUpBattleField();
        Blue.CleanUpBattleField();
        break;
    case 50:
        Red.PrintMoney();  // addfor 2010
        Blue.PrintMoney(); // addfor 2010
        break;
    case 55:
        Red.WarriorsReport();
        Blue.WarriorsReport();
        break;
    case 59:
        Red.DeleteDeadWarriors();
        Blue.DeleteDeadWarriors();
        break;
    }
    return 1;
}
void CHeadquarter::EnemyReach()
{
    if (nColor == COLOR_RED)
        AddEvent(EVENT_CITYTAKEN, nCityNo, nColor, string("red headquarter was taken"));
    else
        AddEvent(EVENT_CITYTAKEN, nCityNo, nColor, string("blue headquarter was taken"));
    pKingdom->WarEnd();
}
CWarrior *CHeadquarter::QueryCityWarrior(int nCityNo)
{
    list<CWarrior *>::iterator i;
    for (i = lstWarrior.begin(); i != lstWarrior.end(); i++)
    {
        if ((*i)->GetPosition() == nCityNo)
            return (*i);
    }
    return NULL;
}
CWarrior *CHeadquarter::QueryNextCityWarrior(int nCityNo)
{
    int direction = nColor == COLOR_RED ? -1 : 1; // This n color is enemy's color
    list<CWarrior *>::iterator i;
    for (i = lstWarrior.begin(); i != lstWarrior.end(); i++)
    {
        if ((*i)->GetPosition() == nCityNo+direction)
            return (*i);
    }
    return NULL;
}
void CHeadquarter::WarriorsMarch(int nEnemyHeadquterCityNo)
{
    list<CWarrior *>::iterator i;
    string sEventString;
    for (i = lstWarrior.begin(); i != lstWarrior.end(); i++)
    {
        int nOldPos = (*i)->nCityNo;
        if (nColor == COLOR_RED)
        {
            if ((*i)->nCityNo < nEnemyHeadquterCityNo)
                (*i)->March();
        }
        else
        {
            if ((*i)->nCityNo > nEnemyHeadquterCityNo)
                (*i)->March();
        }
        char szTmp[100];
        sprintf(szTmp, " with %d elements and force %d", (*i)->nStrength, (*i)->nForce);

        if (nOldPos != nEnemyHeadquterCityNo)
        {
            if ((*i)->nCityNo == nEnemyHeadquterCityNo)
            {
                sEventString = (*i)->GetName() + " reached " + pEnemyheadquarter->GetColorStr() + " headquarter" + szTmp;
                AddEvent(EVENT_REACH, (*i)->nCityNo, nColor, sEventString);
                nWarriorInEnemyHeadquarter++;
                if(nWarriorInEnemyHeadquarter >= 2)
                    pEnemyheadquarter->EnemyReach();
            }
            else
            {
                sEventString = (*i)->GetName() + " marched to city " + MyIntToStr((*i)->GetPosition()) + szTmp;
                AddEvent(EVENT_MARCH, (*i)->GetPosition(), nColor, sEventString);
                // addfor debug
                //				if( sEventString.find("blue lion 6 marched to city 10 with 10 elements and force 50")
                //					!= string::npos)
                //					cout << "UUUU" <<endl;
                // gwend
            }
        }
    }
}
void CHeadquarter::WarriorsReport()
{
    list<CWarrior *>::iterator i = lstWarrior.begin();
    string sEventString;
    while (i != lstWarrior.end())
    {
        if ((*i)->nStrength <= 0)
        { // 在35分，或刚才的战斗中已经被杀了
            i = lstWarrior.erase(i);
            continue;
        }
        string sEventString = (*i)->GetName();
        string sWeaponStatus = (*i)->ReportStatus();
        sEventString += sWeaponStatus;
        AddEvent(EVENT_WARRIOR_REPORT, (*i)->nCityNo, nColor, sEventString);
        i++;
    }
}
void CHeadquarter::WarriorsAttack()
{
    list<CWarrior *>::iterator j = lstWarrior.begin();
    for (j; j != lstWarrior.end(); j++)
    {
        CWarrior *pAttacker = (*j);
        if (pAttacker->nStrength <= 0) {
            continue;
        }
        
        int nCityNo = pAttacker->GetPosition();
        CWarrior *pEnemy = pEnemyheadquarter->QueryCityWarrior(nCityNo);

        bool attackFirst;
        if(pKingdom->flags[nCityNo] == -1) {
            // COLOR_BLUE = 1
            // COLOR_RED = 0
            attackFirst = (nCityNo % 2 != nColor); //奇偶
        }
        else {
            attackFirst = pKingdom->flags[nCityNo] == nColor;
        }

        if (pEnemy && attackFirst)
        { 
            if(pEnemy->GetStrength() <= 0) {// Shot by arrow
                if(pAttacker->GetName().find("dragon") != string::npos) {
                    ((CDragon *)pAttacker)->fMorale += 0.2;

                    if(pAttacker->GetStrength()>0 && ((CDragon *)pAttacker)->fMorale > 0.8) {
                        string sResult = pAttacker->GetName() + " yelled in city " + MyIntToStr(nCityNo);
                        AddEvent(EVENT_YELL, nCityNo, nColor, sResult);
                    }
                }
                continue; 
            }
            // Cache the life before attack
            int attackerLife = pAttacker->GetStrength();
            int enemyLife = pEnemy->GetStrength();

            pAttacker->Attack(pEnemy);
            string sResult = pAttacker->GetName() + " attacked " +
             pEnemy->GetName() + " in city " + MyIntToStr(nCityNo) 
             + " with " + MyIntToStr(pAttacker->GetStrength()) 
             + " elements and force " + MyIntToStr(pAttacker->GetForce());
            AddEvent(EVENT_ATTACK, nCityNo, nColor, sResult);
            
            if(pEnemy->GetStrength() <= 0) //attacker alive enemy dead
            {
                sResult = pEnemy->GetName() + " was killed in city " + MyIntToStr(nCityNo);
                AddEvent(EVENT_DEAD, nCityNo, nColor, sResult); 
                
                if(pAttacker->GetName().find("dragon") != string::npos) {
                    ((CDragon *)pAttacker)->fMorale += 0.2;
                }

                if(pEnemy->GetName().find("lion") != string::npos) {
                    pAttacker->SetStrength(pAttacker->GetStrength() + enemyLife);
                }
            }
            else
            {
                pEnemy->FightBack(pAttacker);
                if(pEnemy->GetName().find("ninja")==string::npos) { // Not ninja
                    sResult = pEnemy->GetName() + " fought back against " 
                    + pAttacker->GetName() + " in city " + MyIntToStr(nCityNo);
                    AddEvent(EVENT_FIGHT_BACK, nCityNo, nColor, sResult);
                }

                if(pAttacker->GetStrength() <= 0) // attacker dead enemy alive
                {
                    sResult = pAttacker->GetName() + " was killed in city " + MyIntToStr(nCityNo);
                    AddEvent(EVENT_DEAD, nCityNo, nColor, sResult);
                    if(pAttacker->GetName().find("lion")!=string::npos) {
                        pEnemy->SetStrength(pEnemy->GetStrength() + attackerLife);
                    }
                }
                else // draw
                {
                    if(pEnemy->GetName().find("dragon") != string::npos) {
                        ((CDragon *)pEnemy)->fMorale -= 0.2;
                    }
                }
            }

            if(pAttacker->GetName().find("dragon")!=string::npos && pAttacker->GetStrength()>0 && ((CDragon *)pAttacker)->fMorale > 0.8) {
                string sResult = pAttacker->GetName() + " yelled in city " + MyIntToStr(nCityNo);
                AddEvent(EVENT_YELL, nCityNo, nColor, sResult);
            }
        }
    }
}
void CHeadquarter::HandleArrow()
{
    for(auto it = lstWarrior.begin(); it != lstWarrior.end(); it++)
    {
        CWarrior *pAttacker = (*it);
        auto hasArrow = [](CWarrior *p){return p->weapons[WEAPON_ARROW] != NULL && ((CArrow*)p->weapons[WEAPON_ARROW])->usedTimes !=3;};
        if(!hasArrow(pAttacker))
            continue;
        int nCityNo = pAttacker->GetPosition();
        CWarrior *pEnemy = pEnemyheadquarter->QueryNextCityWarrior(nCityNo);
        if(pEnemy) {
            int damage = ((CArrow *)pAttacker->weapons[WEAPON_ARROW])->GetForce(pEnemy);
            int enemyLife = pEnemy->GetStrength();
            pAttacker->weapons[WEAPON_ARROW]->Use();
            pEnemy->SetStrength(pEnemy->GetStrength() - damage);

            if(pEnemy->GetStrength() <= 0) {
                string sResult = pAttacker->GetName() + " shot and killed " + pEnemy->GetName();
                this->AddEvent(EVENT_ARROW, nCityNo, this->nColor, sResult);
                // Lion Life
                if(pEnemy->GetName().find("lion") != string::npos) {
                    // pAttacker->SetStrength(pAttacker->GetStrength() + enemyLife);
                    // int mask = nColor == COLOR_RED ? 1 : -1;
                    // int nextCityNo = pAttacker->GetPosition() + mask;
                    // CWarrior *pNext = QueryCityWarrior(nextCityNo);
                    // if(pNext) {
                    //     pNext->SetStrength(pNext->GetStrength() + enemyLife);
                    // }
                }
            }
            else {
                string sResult = pAttacker->GetName() + " shot";
                this->AddEvent(EVENT_ARROW, nCityNo, this->nColor, sResult);
            }
        }
    }
}
void CHeadquarter::HandleBomb()
{
    for (auto it = lstWarrior.begin(); it != lstWarrior.end(); it++)
    {
        CWarrior *pAttacker = (*it);
        if(pAttacker -> weapons[WEAPON_BOMB] == NULL || pAttacker->GetStrength() <= 0)
            continue;
        
        int nCityNo = pAttacker->GetPosition();
        CWarrior *pEnemy = pEnemyheadquarter->QueryCityWarrior(nCityNo);
        if(pEnemy == NULL || pEnemy->GetStrength() <= 0)
            continue;

        bool attackFirst;
        if(pKingdom->flags[nCityNo] == -1) {
            // COLOR_BLUE = 1
            // COLOR_RED = 0
            attackFirst = (nCityNo % 2 != nColor); //奇偶
        }
        else {
            attackFirst = pKingdom->flags[nCityNo] == nColor;
        }

        if(
            (!attackFirst && pAttacker->GetStrength() <= pEnemy->GetAttackDamage()) ||
            (attackFirst && pEnemy->GetStrength() > pAttacker->GetAttackDamage() && pAttacker->GetStrength() <= pEnemy->GetFightBackDamage())
        ){
            string sResult = pAttacker->GetName() + " used a bomb and killed " + pEnemy->GetName();
            this->AddEvent(EVENT_BOMB, nCityNo, this->nColor, sResult);
            pAttacker->SetStrength(-1); //Dead AF
            pEnemy->SetStrength(-1);
        }
    }
}
void CHeadquarter::PickUpLife()
{
    for(auto it = lstWarrior.begin(); it != lstWarrior.end(); it++)
    {
        int nCityNo = (*it)->GetPosition();
        if(pEnemyheadquarter->QueryCityWarrior(nCityNo) == NULL)
        {
            if(pKingdom->lifeValue[nCityNo] > 0) {
                nMoney += pKingdom->lifeValue[nCityNo];
                string sResult = (*it)->GetName() + " earned " + MyIntToStr(pKingdom->lifeValue[nCityNo]) + " elements for his headquarter";
                AddEvent(EVENT_PICKUP, nCityNo, this->nColor, sResult);
                pKingdom->lifeValue[nCityNo] = 0;
            }
        }
    }
}
void CHeadquarter::CleanUpBattleField()
{
    //Rewarding 
    for(auto it = lstWarrior.begin(); it != lstWarrior.end(); it++)
    {
        CWarrior *pAttacker = (*it);
        int nCityNo = pAttacker->GetPosition();
        CWarrior *pEnemy = pEnemyheadquarter->QueryCityWarrior(nCityNo);
        // Our: Alive
        // Enemy: Dead
        if(pAttacker->GetStrength() > 0 && pEnemy != NULL 
        && pEnemy->GetStrength() <= 0)
        {
            if(nMoney >= 8) {
                pAttacker->nStrength += 8;
                nMoney -= 8;
            }
        }
    }
    //Pickup
    for(auto it = lstWarrior.begin(); it != lstWarrior.end(); it++)
    {
        CWarrior *pAttacker = (*it);
        int nCityNo = pAttacker->GetPosition();
        CWarrior *pEnemy = pEnemyheadquarter->QueryCityWarrior(nCityNo);
        // Our: Alive
        // Enemy: Dead
        if(pAttacker->GetStrength()>0 && pEnemy != NULL 
        && pEnemy->GetStrength() <= 0)
        {
            nMoney += pKingdom->lifeValue[nCityNo];
            string sResult = pAttacker->GetName() +
            " earned " + MyIntToStr(pKingdom->lifeValue[nCityNo]) + " elements for his headquarter";
            this->AddEvent(EVENT_EARNMONEY, nCityNo, this->nColor, sResult);
            pKingdom->lifeValue[nCityNo] = 0;
        }
    }
    //Wolf Rob
    for(auto it = lstWarrior.begin(); it != lstWarrior.end(); it++)
    {
        CWarrior *pAttacker = (*it);
        int nCityNo = pAttacker->GetPosition();
        CWarrior *pEnemy = pEnemyheadquarter->QueryCityWarrior(nCityNo);
        // Our: Alive
        // Enemy: Dead
        auto hasSword = [](CWarrior *p){return p->weapons[WEAPON_SWORD] != NULL && ((CSword*)p->weapons[WEAPON_SWORD])->power > 0;};
        auto hasArrow = [](CWarrior *p){return p->weapons[WEAPON_ARROW] != NULL && ((CArrow*)p->weapons[WEAPON_ARROW])->usedTimes !=3;};
        auto hasBomb = [](CWarrior *p){return p->weapons[WEAPON_BOMB] != NULL;};

        if(pAttacker->GetStrength() > 0 && pEnemy != NULL
        && pEnemy->GetStrength() <= 0)
        {
            if(pAttacker->GetName().find("wolf") != string::npos) {
                if(hasSword(pEnemy) && !hasSword(pAttacker)) {
                    pAttacker->weapons[WEAPON_SWORD] = pEnemy->weapons[WEAPON_SWORD];
                    pEnemy->weapons[WEAPON_SWORD] = NULL;
                }
                if(hasArrow(pEnemy) && !hasArrow(pAttacker)) {
                    pAttacker->weapons[WEAPON_ARROW] = pEnemy->weapons[WEAPON_ARROW];
                    pEnemy->weapons[WEAPON_ARROW] = NULL;
                }
                if(hasBomb(pEnemy) && !hasBomb(pAttacker)) {
                    pAttacker->weapons[WEAPON_BOMB] = pEnemy->weapons[WEAPON_BOMB];
                    pEnemy->weapons[WEAPON_BOMB] = NULL;
                }
            }
        }
    }
    //Flag check
    for(auto it = lstWarrior.begin(); it != lstWarrior.end(); it++)
    {
        CWarrior *pAttacker = (*it);
        int nCityNo = pAttacker->GetPosition();
        CWarrior *pEnemy = pEnemyheadquarter->QueryCityWarrior(nCityNo);
        // Our: Alive
        // Enemy: Dead
        if(pAttacker->GetStrength() > 0 && pEnemy != NULL
        && pEnemy->GetStrength() <= 0)
        {
            if(pKingdom->prevWinner[nCityNo] == nColor
            && pKingdom->flags[nCityNo] != nColor) {
                pKingdom->flags[nCityNo] = nColor;
                string sResult = GetColorStr() + " flag raised in city "+MyIntToStr(nCityNo);
                this->AddEvent(EVENT_FLAG, nCityNo, this->nColor, sResult);
            }
            pKingdom->prevWinner[nCityNo] = nColor;
        }
        // Our:Alive
        // Enemy: Alive
        else if(pAttacker->GetStrength() > 0 && pEnemy != NULL && pEnemy->GetStrength() > 0)
        {
            pKingdom->prevWinner[nCityNo] = -1;
        }
    }
    // Moved up
    // Dragon Morale 
    // for(auto it = lstWarrior.begin(); it != lstWarrior.end(); it++)
    // {
    //     CWarrior *pAttacker = (*it);
    //     int nCityNo = pAttacker->GetPosition();
    //     CWarrior *pEnemy = pEnemyheadquarter->QueryCityWarrior(nCityNo);
        
    //     if(pAttacker->GetName().find("dragon") != string::npos) {
    //         // Our: Alive
    //         // Enemy: Dead
    //         if(pAttacker->GetStrength() > 0 && pEnemy != NULL
    //         && pEnemy->GetStrength() <= 0) {
    //             ((CDragon *)pAttacker)->fMorale += 0.2;
    //         }
    //         // Our: Alive
    //         // Enemy: Alive
    //         else if(pAttacker->GetStrength() > 0 && pEnemy != NULL
    //         && pEnemy->GetStrength() > 0) {
    //             ((CDragon *)pAttacker)->fMorale -= 0.2;
    //         }
    //     }
    // }
    // Lion Loyalty
    for(auto it = lstWarrior.begin(); it != lstWarrior.end(); it++)
    {
        CWarrior *pAttacker = (*it);
        int nCityNo = pAttacker->GetPosition();
        CWarrior *pEnemy = pEnemyheadquarter->QueryCityWarrior(nCityNo);
        
        if(pAttacker->GetName().find("lion") != string::npos) {
            // Our: Alive
            // Enemy: Alive
            if(pAttacker->GetStrength() > 0 && pEnemy != NULL
            && pEnemy->GetStrength() > 0) {
                ((CLion *)pAttacker)->nLoyalty -= CLion::nLoyaltyDec;
            }
        }
    }
}
void CHeadquarter::DeleteDeadWarriors()
{
    list<CWarrior *>::iterator i = lstWarrior.begin();
    while (i != lstWarrior.end())
    {
        if ((*i)->GetStrength() <= 0)
        {
            i = lstWarrior.erase(i);
            continue;
        }
        i++;
    }
}
void CHeadquarter::AddEvent(EEventType eType, int nCityNo, int nColor, const string &sEventString)
{
    pKingdom->AddEvent(eType, nCityNo, nColor, sEventString);
}
void CHeadquarter::PrintMoney() // addfor 2010
{
    char szTmp[100];

    sprintf(szTmp, "%d", nMoney);
    string sEventString = string(szTmp) + " elements in " + GetColorStr() + " headquarter";
    if (nColor == COLOR_RED)
        pKingdom->AddEvent(EVENT_PRINTMONEY, 0, nColor, sEventString);
    else
        pKingdom->AddEvent(EVENT_PRINTMONEY, pKingdom->nCityNum + 1, nColor, sEventString);
}
void CHeadquarter::WarriorBorn()
{
    CWarrior *p = NULL;
    int nSeqIdx = (nWarriorNo - 1) % 5;
    if (nMoney < CWarrior::InitialLifeValue[MakingSeq[nColor][nSeqIdx]])
        return;
    nMoney -= CWarrior::InitialLifeValue[MakingSeq[nColor][nSeqIdx]];
    int nKindNo = MakingSeq[nColor][nSeqIdx];
    double fMorale = (double)nMoney / (double)CWarrior::InitialLifeValue[nKindNo];
    
    switch (nKindNo)
    {
        case DRAGON:
            p = new CDragon(nWarriorNo, CWarrior::InitialLifeValue[nKindNo], CWarrior::InitalForce[nKindNo], nCityNo, fMorale, this);
            break;
        case NINJA:
            p = new CNinja(nWarriorNo, CWarrior::InitialLifeValue[nKindNo], CWarrior::InitalForce[nKindNo], nCityNo, this);
            break;
        case ICEMAN:
            p = new CIceman(nWarriorNo, CWarrior::InitialLifeValue[nKindNo], CWarrior::InitalForce[nKindNo], nCityNo, this);
            break;
        case LION:
            p = new CLion(nWarriorNo, CWarrior::InitialLifeValue[nKindNo], CWarrior::InitalForce[nKindNo], nCityNo, this);
            break;
        case WOLF:
            p = new CWolf(nWarriorNo, CWarrior::InitialLifeValue[nKindNo], CWarrior::InitalForce[nKindNo], nCityNo, this);
            break;
    }

    lstWarrior.push_back(p);
    string sEventString = p->GetName() + " born";
    if (nKindNo == LION)
        sEventString += "\nIts loyalty is " + MyIntToStr(((CLion *)p)->nLoyalty);
    if (nKindNo == DRAGON) {
        char sTmp[20];
        sprintf(sTmp, "%.2f", ((CDragon *)p)->fMorale);
        sEventString += "\nIts morale is " + string(sTmp);
    }
    pKingdom->AddEvent(EVENT_BORN, nCityNo, nColor, sEventString);
    nWarriorNo++;
}
/**
 * Class CWarrior
 */
int CWarrior::GetAttackDamage()
{
    int atk = nForce;
    if (weapons[WEAPON_SWORD]) {
        atk += weapons[WEAPON_SWORD]->GetForce();
    }
    return atk;
}
int CWarrior::GetFightBackDamage()
{
    int dmg = nForce * 0.5;
    if (weapons[WEAPON_SWORD]) {
        dmg += weapons[WEAPON_SWORD]->GetForce();
    }
    return dmg;
}
void CWarrior::Attack(CWarrior *pEnemy){
    int atk = 0;
    atk += nForce;
    if (weapons[WEAPON_SWORD]) {
        atk += weapons[WEAPON_SWORD]->GetForce();
        weapons[WEAPON_SWORD]->Use();
    }
    pEnemy->nStrength -= atk;
}
void CWarrior::FightBack(CWarrior *pEnemy) {
    if(nStrength <= 0)  return; //Dead already
    int atk = 0;
    atk += nForce  / 2;
    if (weapons[WEAPON_SWORD]) {
        atk += weapons[WEAPON_SWORD]->GetForce();
        weapons[WEAPON_SWORD]->Use();
    }
    pEnemy->nStrength -= atk;
}
string CWarrior::ReportStatus()
{
    string sRes = " has ";
    auto hasSword = [](CWarrior *p){return p->weapons[WEAPON_SWORD] != NULL && ((CSword*)p->weapons[WEAPON_SWORD])->power > 0;};
    auto hasArrow = [](CWarrior *p){return p->weapons[WEAPON_ARROW] != NULL && ((CArrow*)p->weapons[WEAPON_ARROW])->usedTimes !=3;};
    auto hasBomb = [](CWarrior *p){return p->weapons[WEAPON_BOMB] != NULL;};
    if(!hasSword(this) && !hasArrow(this) && !hasBomb(this))
        sRes += "no weapon";
    else {
        if(hasArrow(this)) {
            sRes += "arrow(" + to_string(3 - ((CArrow *)weapons[WEAPON_ARROW])->usedTimes) + "),";
        }
        if(hasBomb(this)) {
            sRes += "bomb,";
        }
        if(hasSword(this)) {
            sRes += "sword(" + to_string(weapons[WEAPON_SWORD]->GetForce()) + "),";
        }
    }
    if(sRes[sRes.length()-1] == ',')
        sRes = sRes.substr(0, sRes.length()-1);
    return sRes;
}
CWarrior::~CWarrior()
{
    for (int i = 0; i < MAX_WPS; ++i)
        if (weapons[i])
            delete weapons[i];
}

const char *CWeapon::Names[WEAPON_NUM] = {"sword", "bomb", "arrow"};
const char *CWarrior::Names[WARRIOR_NUM] = {"dragon", "ninja", "iceman", "lion", "wolf"};
int CWarrior::InitialLifeValue[WARRIOR_NUM];
int CWarrior::InitalForce[WARRIOR_NUM];
int CLion::nLoyaltyDec;
int CArrow::initialPower;
int CHeadquarter::MakingSeq[2][WARRIOR_NUM] = {{2, 3, 4, 1, 0}, {3, 0, 1, 2, 4}};
int main()
{
    int nCases;
    int M, N, R, K, T;
    cin >> nCases;
    int nCaseNo = 1;
    while (nCases--)
    {
        cin >> M >> N >> R >> K >> T;
        CLion::nLoyaltyDec = K;
        CArrow::initialPower = R;
        // 第二行：五个整数，依次是 dragon 、NINJA、iceman、lion、wolf 的初始生命值。它们都大于0小于等于100
        int i;
        for (i = 0; i < WARRIOR_NUM; i++)
            cin >> CWarrior::InitialLifeValue[i];
        for (i = 0; i < WARRIOR_NUM; i++)
            cin >> CWarrior::InitalForce[i];
        CKingdom MyKingdom(N, M);
        MyKingdom.Run(T);
        cout << "Case " << nCaseNo++ << ":" << endl;
        MyKingdom.OutputResult();
    }
    return 0;
}