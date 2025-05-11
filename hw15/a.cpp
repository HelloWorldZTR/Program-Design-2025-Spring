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
    EVENT_MARCH,
    EVENT_WOLFSROB,
    EVENT_ARROW,
    EVENT_BOMB,
    EVENT_FIGHT_RESULT,
    EVENT_YELL,
    EVENT_EARNMONEY,
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

class CHeadquarter;
class CKingdom;
class CWarrior;
class CWeapon;

class CWeapon
{
public:
    int nKindNo;
    friend class CWarrior;
    CWarrior *master;
    static const char *Names[WEAPON_NUM];
    friend int WpCompare(const void *wp1, const void *wp2);
    friend int WpCompare2(const void *wp1, const void *wp2);

    virtual int GetForce() {};
    virtual void Use() {};
    static CWeapon *NewWeapon(int idx, CWarrior *master)
    {
        switch (idx)
        {
        case 0:
            return new CSword(master);
        case 1:
            return new CBomb(master);
        case 2:
            return new CArrow(master);
        }
        return NULL;
    }
    CWeapon(CWarrior *m) : master(m) {}
};

#define COLOR_RED 0
#define COLOR_BLUE 1
#define COLOR_NONE 3

typedef CWarrior *PWARRIOR;
string MyIntToStr(int n)
{
    char szTmp[300];
    sprintf(szTmp, "%d", n);
    return szTmp;
}
// cls CWarrior
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
    virtual string Yell() { return ""; }
    virtual string TakeEnemyWeapons(CWarrior *pEnemy, bool beforeFight = false);
    virtual ~CWarrior()
    {
        for (int i = 0; i < MAX_WPS; ++i)
            if (weapons[i])
                delete weapons[i];
    }
    virtual int GetAttackDamage() {
        int atk = 0;
        atk += nForce;
        if (weapons[WEAPON_SWORD]) {
            atk += weapons[WEAPON_SWORD]->GetForce();
        }
        return atk;
    }
    virtual int GetFightBackDamage() {
        int dmg = 0;
        if (weapons[WEAPON_SWORD]) {
            dmg += weapons[WEAPON_SWORD]->GetForce();
        }
        return dmg;
    }
    virtual void Attack(CWarrior *pEnemy) {
        int atk = 0;
        atk += nForce;
        if (weapons[WEAPON_SWORD]) {
            atk += weapons[WEAPON_SWORD]->GetForce();
            weapons[WEAPON_SWORD]->Use();
        }
        pEnemy->nStrength -= atk;
    }
    virtual void FightBack(CWarrior *pEnemy) {
        if(nStrength <= 0)  return; //Dead already
        int atk = 0;
        atk += nForce  / 2;
        if (weapons[WEAPON_SWORD]) {
            atk += weapons[WEAPON_SWORD]->GetForce();
            weapons[WEAPON_SWORD]->Use();
        }
        pEnemy->nStrength -= atk;
    }
    virtual string GetName() = 0;
    virtual void March() {
        if (GetColor() == COLOR_RED)
            nCityNo++;
        else
            nCityNo--;
    }

    void SortWeapons(bool forTaken)
    {
        if (forTaken)
            qsort(weapons, MAX_WPS, sizeof(CWeapon *), WpCompare2);
        else
            qsort(weapons, MAX_WPS, sizeof(CWeapon *), WpCompare);
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
    string ReportStatus()
    {
        char tmp[200];
        sprintf(tmp, " has ");
        if(!weapons[WEAPON_ARROW] && !weapons[WEAPON_SWORD] && !weapons[WEAPON_BOMB])
            sprintf(tmp, "no weapons");
        else {
            if(weapons[WEAPON_ARROW]) {
                sprintf(tmp, "arrow(%d),", 3 - ((CArrow *)weapons[WEAPON_ARROW])->usedTimes);
            }
            if(weapons[WEAPON_BOMB]) {
                sprintf(tmp, "bomb,");
            }
            if(weapons[WEAPON_SWORD]) {
                sprintf(tmp, "sword(%d),", weapons[WEAPON_SWORD]->GetForce());
            }
        }
        int len = strlen(tmp);
        if(tmp[len - 1] == ',')
            tmp[len - 1] = '\0';
        return string(tmp);
    }
};

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
    float fMorale;
    CDragon(int nId_, int nStrength_, int nForce_, int nCityNo_, float fMorale, CHeadquarter *pHeadquarter_) : CWarrior(nId_, nStrength_, nForce_, nCityNo_, pHeadquarter_), fMorale(fMorale)
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
    virtual string Yell()
    {
        if (nStrength > 0 && fMorale > 0.8)
        { // 没有战死
            return GetName() + " yelled in city " + MyIntToStr(nCityNo);
        }
        else
            return "";
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
        weapons[nId % 3] = CWeapon::NewWeapon(nId % 3, this);
    }
    virtual string GetName() {
        return pHeadquarter->GetColorStr() + " lion " + MyIntToStr(nId);
    }
    bool Runaway()
    {
        return nLoyalty <= 0;
    }
    virtual void March() {
        CWarrior::March();
        nLoyalty -= nLoyaltyDec;
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
                if (nStrength < 0)
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

class CSword : public CWeapon
{
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
    int usedTimes;
    int power;

public:
    static int initalPower;

public:
    CArrow(CWarrior *master) : CWeapon(master), usedTimes(0)
    {
        nKindNo = 2;
        power = initalPower;
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
    friend int WpCompare(const void *wp1, const void *wp2);
    friend int WpCompare2(const void *wp1, const void *wp2);
};
class CBomb : public CWeapon
{
public:
    CBomb(CWarrior *master) : CWeapon(master)
    {
        nKindNo = 1;
    }
};

int WpCompare(const void *wp1, const void *wp2)
{
    CWeapon **p1 = (CWeapon **)wp1;
    CWeapon **p2 = (CWeapon **)wp2;
    if (*p1 == NULL)
        return 1;
    if (*p2 == NULL)
        return -1;
    if ((*p1)->nKindNo != (*p2)->nKindNo)
        return (*p1)->nKindNo - (*p2)->nKindNo;
    else
    {
        if ((*p1)->nKindNo == WEAPON_ARROW) // arrow
            return ((CArrow *)(*p2))->usedTimes - ((CArrow *)(*p1))->usedTimes;
        else
            return 0;
    }
}
int WpCompare2(const void *wp1, const void *wp2)
{
    CWeapon **p1 = (CWeapon **)wp1;
    CWeapon **p2 = (CWeapon **)wp2;
    if (*p1 == NULL)
        return 1;
    if (*p2 == NULL)
        return -1;
    if ((*p1)->nKindNo != (*p2)->nKindNo)
        return (*p1)->nKindNo - (*p2)->nKindNo;
    else
    {
        if ((*p1)->nKindNo == WEAPON_ARROW) // arrow
            return ((CArrow *)(*p1))->usedTimes - ((CArrow *)(*p2))->usedTimes;
        else
            return 0;
    }
}

// cls CHeadQuarter
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
    CWarrior *QueryCityWarrior(int nCityNo);
    CWarrior *QueryNextCityWarrior(int nCityNo);
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
        /*
                if( eEventType == e2.eEventType && eEventType == EVENT_WARRIOR_REPORT) {
                    if( nColor < e2.nColor )
                        return true;
                    else if( nColor == e2.nColor)
                        return nCityNo < e2.nCityNo ;
                    else
                        return false;
                }
        */
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
    int flags[25] = {-1};
    int prevWinner[25] = {-1};

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
        if (nEndTime == 3000000)
            nEndTime = nTimeInMinutes;
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

// CHeadquarter functions
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
        for (int i = 0; i <= nCityNum + 1; i++)
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
    case 40: // 发生战斗
        Red.WarriorsAttack();
        Blue.WarriorsAttack();
        break;
    case 50:
        Red.PrintMoney();  // addfor 2010
        Blue.PrintMoney(); // addfor 2010
        break;
    case 55:
        Red.WarriorsReport();
        Blue.WarriorsReport();
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
    int direction = nColor == COLOR_RED ? 1 : -1;
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
        if (pAttacker->nStrength <= 0)
            continue;
        int nCityNo = pAttacker->GetPosition();
        CWarrior *pEnemy = pEnemyheadquarter->QueryCityWarrior(nCityNo);
        char szTmp[200];
        if (pEnemy)
        { 
            if(pEnemy->GetStrength() <= 0)
                continue; // Shot by arrow
            pAttacker->Attack(pEnemy);
            string sResult = pAttacker->GetName() + " attacked " +
             pEnemy->GetName() + " in city " + MyIntToStr(nCityNo) 
             + " with " + MyIntToStr(pAttacker->GetStrength()) 
             + " elements and " + MyIntToStr(pAttacker->GetForce())
             + " force";
            AddEvent(EVENT_FIGHT_RESULT, nCityNo, nColor, sResult);
            if(pEnemy->GetStrength() <= 0)
            {
                sResult = pEnemy->GetName() + " was killed in city " + MyIntToStr(nCityNo);
                AddEvent(EVENT_FIGHT_RESULT, nCityNo, nColor, sResult);  
            }
            else
            {
                pEnemy->FightBack(pAttacker);
                sResult = pEnemy->GetName() + " fought back against " 
                + pAttacker->GetName() + " in city " + MyIntToStr(nCityNo) + " with " + MyIntToStr(pEnemy->GetStrength()) + " elements and " + MyIntToStr(pEnemy->GetForce()) + " force";
                AddEvent(EVENT_FIGHT_RESULT, nCityNo, nColor, sResult);
                if(pAttacker->GetStrength() <= 0)
                {
                    sResult = pAttacker->GetName() + " was killed in city " + MyIntToStr(nCityNo);
                    AddEvent(EVENT_FIGHT_RESULT, nCityNo, nColor, sResult);  
                }
            }
        }
    }
}
void CHeadquarter::HandleArrow()
{
    for(auto it = lstWarrior.begin(); it != lstWarrior.end(); it++)
    {
        CWarrior *pAttacker = (*it);
        if(pAttacker -> weapons[WEAPON_ARROW] == NULL)
            continue;
        int nCityNo = pAttacker->GetPosition();
        CWarrior *pEnemy = pEnemyheadquarter->QueryNextCityWarrior(nCityNo);
        if(pEnemy) {
            int damage = ((CArrow *)pAttacker->weapons[WEAPON_ARROW])->GetForce(pEnemy);
            pAttacker->weapons[WEAPON_ARROW]->Use();
            pEnemy->SetStrength(pEnemy->GetStrength() - damage);

            if(pEnemy->GetStrength() <= 0) {
                string sResult = pAttacker->GetName() + " shot and killed " + pEnemy->GetName();
                this->AddEvent(EVENT_ARROW, nCityNo, this->nColor, sResult);
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
        if(
            pAttacker->GetStrength() <= pEnemy->GetAttackDamage() &&
            pAttacker->GetStrength() <= pEnemy->GetFightBackDamage()
        ){
            string sResult = pAttacker->GetName() + " used bomb and killed " + pEnemy->GetName();
            this->AddEvent(EVENT_BOMB, nCityNo, this->nColor, sResult);
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
        if(pEnemy != NULL && pEnemy->GetStrength() <= 0)
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
        if(pEnemy != NULL && pEnemy->GetStrength() <= 0)
        {
            nMoney += pKingdom->lifeValue[nCityNo];
            string sResult = pAttacker->GetName() +
            " earned " + MyIntToStr(pKingdom->lifeValue[nCityNo]) + " elements for his headquarter";
            this->AddEvent(EVENT_EARNMONEY, nCityNo, this->nColor, sResult);
            pKingdom->lifeValue[nCityNo] = 0;
        }
    }
    //Wolf Rob
    //TODO:
    //Flag check
    //TODO:
    //Delete Dead
    //TODO:
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

    switch (nKindNo)
    {
    case DRAGON:
        float fMorale = (float)nMoney / CWarrior::InitialLifeValue[nKindNo];
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
    pKingdom->AddEvent(EVENT_BORN, nCityNo, nColor, sEventString);
    nWarriorNo++;
}

const char *CWeapon::Names[WEAPON_NUM] = {"sword", "bomb", "arrow"};
const char *CWarrior::Names[WARRIOR_NUM] = {"dragon", "ninja", "iceman", "lion", "wolf"};
int CWarrior::InitialLifeValue[WARRIOR_NUM];
int CWarrior::InitalForce[WARRIOR_NUM];
int CLion::nLoyaltyDec;
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
        CArrow::initalPower = R;
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