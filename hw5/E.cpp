/**
 * @author: hlwdztr
 * @date: 2025/03/11
 * @version: 3.0
 */

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

// #define DEBUG

const int MAX_CITY = 25;
const int MAX_ROBBED = 10;

using namespace std;

/**
 * @brief WarriorType class
 * @class WarriorType
 * 抽象出战士的类型，包括名字和生命值
 */
class WarriorType
{
public:
    string name;
    int life;
    int loyalty_decay;
    int force;
    WarriorType() {}
    WarriorType(string _name, int _life = 0, int _force = 0)
    {
        name = _name, life = _life, force = _force;
    }
    bool operator<(const WarriorType &_b) const
    {
        return name < _b.name;
    }
    bool operator==(const WarriorType &_b) const
    {
        return name == _b.name;
    }
    bool operator!=(const WarriorType &_b) const
    {
        return name != _b.name;
    }
    string getName()
    {
        return name;
    }
};

/**
 * @class WeaponType
 * 抽象出武器的类型，包括名字和攻击力(?)
 */
class WeaponType
{
private:
    string name;
    int attack;

public:
    WeaponType() {
        name = "none";
    }
    WeaponType(string _name)
    {
        name = _name;
    }
    operator string()
    {
        return name;
    }
    bool operator<(const WeaponType &_b) const
    {
        // 因为是sword, bomb, arrow可以用反字典序排
        return name < _b.name;
    }
    bool operator>(const WeaponType &_b) const
    {
        return name > _b.name;
    }
    bool operator==(const WeaponType &_b) const
    {
        return name == _b.name;
    }
    bool operator!=(const WeaponType &_b) const
    {
        return name != _b.name;
    }
    string getName()
    {
        return name;
    }
};

/*Global Vars*/
WarriorType dragon("dragon");
WarriorType ninja("ninja");
WarriorType iceman("iceman");
WarriorType lion("lion");
WarriorType wolf("wolf");

WeaponType sword("sword");
WeaponType bomb("bomb");
WeaponType arrow("arrow");
WeaponType none("none");
/*End Global vars*/

/**
 * @class Weapon
 * 武器的实例化
 */
class Weapon
{
public:
    WeaponType type;
    int usedTimes = 0;

public:
    Weapon()
    {
        type = none;
    }
    Weapon(const Weapon& other)
    {
        type = other.type;
        usedTimes = other.usedTimes;
    }
    Weapon(WeaponType _type)
    {
        type = _type;
    }
    operator string()
    {
        return type.getName();
    }
    string getName()
    {
        return type.getName();
    }
    bool operator==(const Weapon &_b) const
    {
        return type == _b.type && usedTimes == _b.usedTimes;
    }
    bool operator<(const Weapon &_b) const
    {
        if (type != _b.type)
            return type > _b.type;
        else
            return usedTimes < _b.usedTimes;
    }
    static bool compareBattle(const Weapon &a, const Weapon &b)
    {
        if (a.type != b.type)
            return a.type > b.type;
        else
            return a.usedTimes > b.usedTimes; // 用过的次数多的在前
    }
    static bool compareRob(const Weapon &a, const Weapon &b)
    {
        if (a.type != b.type)
            return a.type > b.type;
        else
            return a.usedTimes < b.usedTimes; // 用过的次数少的在前
    }
};

/**
 * @class Warrior
 * 战士的实例化
 */
class Warrior
{
public:
    int id;
    WarriorType type;
    int life;
    int force;
    int loyalty;
    float morale;
    vector<Weapon> weapons;

public:
    Warrior()
    {
        id = -1; // Not exist
    }
    ~Warrior()
    {
        weapons.clear();
    }
    Warrior(WarriorType &_type, int _id, int _loyalty = -1, float _morale = -1, vector<Weapon> _weapons = vector<Weapon>())
    {
        type = _type;
        id = _id;
        loyalty = _loyalty;
        morale = _morale;
        weapons = _weapons;
        weapons.reserve(10);
        life = type.life;
        force = type.force;
    }

    operator string()
    {
        return type.getName();
    }

    // Check if the warroior doesnt exist
    bool operator!()
    {
        return id == -1;
    }

    // Return weapon count
    map<WeaponType, int> weaponCount()
    {
        map<WeaponType, int> res;
        res[sword] = 0;
        res[bomb] = 0;
        res[arrow] = 0;
        for (auto &w : weapons)
        {
            res[w.type]++;
        }
        return res;
    }

    // Return Classified weapons
    map<WeaponType, vector<Weapon>> classifiedWeapons()
    {
        map<WeaponType, vector<Weapon>> res;
        res[sword] = vector<Weapon>();
        res[bomb] = vector<Weapon>();
        res[arrow] = vector<Weapon>();
        for (auto &w : weapons)
        {
            res[w.type].push_back(w);
        }
        return res;
    }

    // Remove a weapon
    // Have to make sure it exists yourself
    void removeWeapon(Weapon &weapon)
    {
        #ifdef DEBUG
        cout<<"Remove "<<weapon.getName()<<" from "<<type.getName()<<" "<<id<<endl;
        #endif
        for (auto it = weapons.begin(); it != weapons.end(); it++)
        {
            if (*it == weapon)
            {
                weapons.erase(it);
                break;
            }
        }
    }
};

/**
 * @class Team
 * 红蓝两队的实例化
 */
class Team
{
public:
    string name;
    WarriorType sequence[5];                     // The sequence of warriors to spawn
    WeaponType weaponsSequence[3];               // Too lazy
    array<vector<Warrior>, MAX_CITY + 1> cities; // 0 ~ n+1 cities, store the warriors in each city
    int idCounter;
    int cursor = 0;
    int life;
    static int N;
    int baseLocation;

public:
    bool stopped = false;

public:
    Team(WarriorType _sequence[5], WeaponType _weaponSequence[3], string _name, int _life, int _N, int _baseLocation)
    {
        for (int i = 0; i < 5; i++)
        {
            sequence[i] = _sequence[i];
            idCounter = 0;
        }
        for (int i = 0; i < 3; i++)
        {
            weaponsSequence[i] = _weaponSequence[i];
        }
        name = _name;
        life = _life;
        N = _N;
        baseLocation = _baseLocation;
    }
    ~Team()
    {
        for (int i = 0; i <= N + 1; i++)
        {
            cities[i].clear();
        }
    }
    void spawnNewWarrior(int time)
    {
        if (stopped)
            return;
        int cnt = 0;
        while (life < sequence[cursor].life)
        {
            cursor++;
            cursor %= 5;
            cnt++;
            if (cnt == 5)
            {
                stopped = true;
                break;
            }
        }
        if (cnt == 5)
        {
#ifdef DEBUG
            printf("%03d:00 %s headquarter stops making warriors\n",
                   time, name.c_str());
#endif
        }
        else
        {
            // Prepare to spawn new warrior
            WarriorType newType = sequence[cursor];
            int n = ++idCounter; // id
            life -= newType.life;
            cursor++;
            cursor %= 5;

            // Spawn new warrior
            Warrior *p;
            if (newType == dragon)
            {
                Weapon weapon = Weapon(weaponsSequence[n % 3]);
                vector<Weapon> weapons = {weapon};
                float morale = (float)life / newType.life;
                p = new Warrior(newType, n, -1, morale, weapons);
            }
            else if (newType == ninja)
            {
                Weapon weapon1 = Weapon(weaponsSequence[n % 3]);
                Weapon weapon2 = Weapon(weaponsSequence[(n + 1) % 3]);
                vector<Weapon> weapons = {weapon1, weapon2};
                p = new Warrior(newType, n, -1, -1, weapons);
            }
            else if (newType == iceman)
            {
                Weapon weapon = Weapon(weaponsSequence[n % 3]);
                p = new Warrior(newType, n, -1, -1, {weapon});
            }
            else if (newType == lion)
            {
                Weapon weapon = Weapon(weaponsSequence[n % 3]);
                int loaylty = life;
                p = new Warrior(newType, n, loaylty, -1, {weapon});
            }
            else if (newType == wolf)
            {
                p = new Warrior(newType, n);
            }
            else
            {
                throw "Invalid Warrior Type"; // Nope, impossible
            }

            cities[baseLocation].push_back(*p);

            // Print the message
            printf("%03d:00 %s %s %d born\n",
                   time, name.c_str(), p->type.name.c_str(), p->id);
            if (newType == lion)
                printf("Its loyalty is %d\n", p->loyalty);
#ifdef DEBUG
            // printf("###");
            // printf("%03d:00 %s %s %d born with strength %d,%d %s in %s headquarter\n",
            //        time, name.c_str(), p->type.name.c_str(), p->id, p->type.life, cities[newType].size(), newType.name.c_str(), name.c_str());
            // printf("###");
            // if (newType == dragon)
            //     printf("It has a %s,and it's morale is %.2f\n",
            //            p->weapons[0].getName().c_str(), p->morale);
            // else if (newType == ninja)
            //     printf("It has a %s and a %s\n",
            //            p->weapons[0].getName().c_str(), p->weapons[1].getName().c_str());
            // else if (newType == iceman)
            //     printf("It has a %s\n",
            //            p->weapons[0].getName().c_str());
            // else if (newType == lion)
            //     printf("It's loyalty is %d\n",
            //            p->loyalty);
#endif
            delete p;
        }
    }
    void marchForward()
    {
        // Move warriors to the next city
        if (baseLocation == 0)
        {
            for (int i = N; i >= 0; i--)
            {
                if (cities[i].size() > 0)
                {
                    if (cities[i].size() > 1)
                        throw "Too many warriors in one city";
                    Warrior w = cities[i].back();
                    cities[i].pop_back();
                    cities[i + 1].push_back(w);
                }
            }
        }
        else
        {
            for (int i = 1; i <= N + 1; i++)
            {
                if (cities[i].size() > 0)
                {
                    if (cities[i].size() > 1)
                        throw "Too many warriors in one city";
                    Warrior w = cities[i].back();
                    cities[i].pop_back();
                    cities[i - 1].push_back(w);
                }
            }
        }
        // Handle iceman life decrease
        for (int i = 1; i <= N; i++)
        {
            if (cities[i].size() > 0)
            {
                Warrior &w = cities[i].back();
                if (w.type == iceman)
                {
                    // cout<<"-----"<<endl;
                    // cout<<w.life<<endl;
                    w.life -= (int)(float)w.life / 10; // Not dead yet
                }
            }
        }
        // Handle lion loyalty
        for (int i = 0; i <= N; i++)
        {
            if (i == N + 1 - baseLocation)
                continue; // ignore warroirs in enemy base
            if (cities[i].size() > 0)
            {
                Warrior &w = cities[i].back();
                if (w.type == lion)
                {
                    w.loyalty -= w.type.loyalty_decay; // Dont ran away at this time
                }
            }
        }
    }
    static void handleMarchInfo(int time, Team &red, Team &blue)
    {
        for (int i = 0; i <= N + 1; i++)
        {
            if (red.cities[i].size() > 0)
            {
                Warrior &w = red.cities[i].back();
                printMarchInfo(time, w, red, blue, i);
            }
            if (blue.cities[i].size() > 0)
            {
                Warrior &w = blue.cities[i].back();
                printMarchInfo(time, w, blue, red, i);
            }
        }
    }
    static void printMarchInfo(int time, Warrior &w, Team &warriorTeam, Team &enemyTeam, int city)
    {
        if (city == N + 1 - warriorTeam.baseLocation)
        { // Reach enemy base
            printf("%03d:10 %s %s %d reached %s headquarter with %d elements and force %d\n",
                   time, warriorTeam.name.c_str(), w.type.name.c_str(), w.id, enemyTeam.name.c_str(), w.life, w.force);
        }
        else
        {
            printf("%03d:10 %s %s %d marched to city %d with %d elements and force %d\n",
                   time, warriorTeam.name.c_str(), w.type.name.c_str(), w.id, city, w.life, w.force);
        }
    }
    void checkLionLoyalty(int time)
    {
        for (int i = 0; i <= N + 1; i++)
        {
            if (i == N + 1 - baseLocation)
                continue; // ignore warroirs in enemy base
            if (cities[i].size() > 0)
            {
                Warrior &w = cities[i].back();
                if (w.type == lion)
                {
                    if (w.loyalty <= 0)
                    {
                        printf("%03d:05 %s lion %d ran away\n",
                               time, name.c_str(), w.id);
                        cities[i].pop_back();
                    }
                }
            }
        }
    }
    static void handleWolfRob(int time, Team &red, Team &blue)
    {
        for (int i = 1; i <= N + 1; i++)
        {
            if (red.cities[i].size() > 0 && blue.cities[i].size() > 0)
            {
                Warrior &w1 = red.cities[i].back();
                Warrior &w2 = blue.cities[i].back();
                Warrior *robber, *robbed;
                string robber_team, robbed_team;
                if(w1.type == wolf && w2.type != wolf)
                {
                    robber = &w1;robber_team = red.name;
                    robbed = &w2;robbed_team = blue.name;
                }
                else if(w2.type == wolf && w1.type != wolf)
                {
                    robber = &w2;robber_team = blue.name;
                    robbed = &w1;robbed_team = red.name;
                }
                else
                {
                    continue;
                }
                // Rob weapons
                map<WeaponType, vector<Weapon>> weapons = robbed->classifiedWeapons();
                for(auto type : {sword, bomb, arrow})
                {
                    if(weapons[type].size() > 0)
                    {
                        size_t maxRobCount = (size_t)MAX_ROBBED - robber->weapons.size();
                        for(int j = 0; j < min(maxRobCount, weapons[type].size()); j++)
                        {
                            Weapon w = weapons[type][j];
                            robber->weapons.push_back(w);
                            robbed->removeWeapon(w);
                        }
                        printRobInfo(time, robber_team, robber->id, min(maxRobCount, weapons[type].size()), type.getName(), robbed_team, robbed->type.name, robbed->id, i);
                        break;
                    }
                }
            }
        }
    }

    static void printRobInfo(int time, string robber_team, int robber_id, int weapon_count, string weapon_name, string robbed_team, string robbed_name, int robbed_id, int city_id)
    {
        if (city_id == 0)
        {
            printf("%03d:35 %s wolf %d took %d %s from %s %s %d in red headquarter\n",
                   time, robber_team.c_str(), robber_id, weapon_count, weapon_name.c_str(), robbed_team.c_str(), robbed_name.c_str(), robbed_id);
        }
        else if (city_id == N + 1)
        {
            printf("%03d:35 %s wolf %d took %d %s from %s %s %d in blue headquarter\n",
                   time, robber_team.c_str(), robber_id, weapon_count, weapon_name.c_str(), robbed_team.c_str(), robbed_name.c_str(), robbed_id);
        }
        else
        {
            printf("%03d:35 %s wolf %d took %d %s from %s %s %d in city %d\n",
                   time, robber_team.c_str(), robber_id, weapon_count, weapon_name.c_str(), robbed_team.c_str(), robbed_name.c_str(), robbed_id, city_id);
        }
    }

    static void handleCombat(int time, Team &red, Team &blue)
    {
        for (int i = 0; i <= N + 1; i++)
        {
            if (red.cities[i].size() > 0 && blue.cities[i].size() > 0)
            {
                // Combat will occur
                Warrior &wr = red.cities[i].back();
                Warrior &wb = blue.cities[i].back();
                sort(wr.weapons.begin(), wr.weapons.end(), Weapon::compareBattle);
                sort(wb.weapons.begin(), wb.weapons.end(), Weapon::compareBattle);
                // Combat
                Warrior *attacker, *defender;
                if (i % 2 == 0) // Blue first
                {
                    attacker = &wb;
                    defender = &wr;
                }
                else // Red first
                {
                    attacker = &wr;
                    defender = &wb;
                }
                bool combatEnded = false;
                map<Warrior *, int> weaponIndex;
                weaponIndex[attacker] = 0;
                weaponIndex[defender] = 0;

                while (!combatEnded)
                {
                    handleAttack(attacker, defender, weaponIndex[attacker]);
                    weaponIndex[attacker]++;
                    if (weaponIndex[attacker] >= attacker->weapons.size())
                    {
                        weaponIndex[attacker] = 0;
                    }
                    swap(attacker, defender);
                    if (attacker->life <= 0 || defender->life <= 0)
                    {
                        combatEnded = true;
                    }
                    if (attacker->weapons.size() == 0 && defender->weapons.size() == 0)
                    {
                        combatEnded = true;
                    }
                    if (checkDeadLoop(attacker) && checkDeadLoop(defender))
                    {
                        combatEnded = true;
                    }
                }

                // Loot weapons
                if (wr.life > 0 && wb.life <= 0)
                {
                    size_t maxLootCount = (size_t)MAX_ROBBED - wr.weapons.size();
                    sort(wb.weapons.begin(), wb.weapons.end(), Weapon::compareRob);
                    for (int j = 0; j < min(maxLootCount, wb.weapons.size()); j++)
                    {
                        #ifdef DEBUG
                        cout << wr.type.name << " " << wr.id << " looted " << wb.weapons[j].getName() << " from " << wb.type.name << " " << wb.id << endl;
                        #endif
                        wr.weapons.push_back(wb.weapons[j]);
                    }
                }
                else if (wb.life > 0 && wr.life <= 0)
                {
                    size_t maxLootCount = (size_t)MAX_ROBBED - wb.weapons.size();
                    sort(wr.weapons.begin(), wr.weapons.end(), Weapon::compareRob);
                    for (int j = 0; j < min(maxLootCount, wr.weapons.size()); j++)
                    {
                        #ifdef DEBUG
                        cout << wb.type.name << " " << wb.id << " looted " << wr.weapons[j].getName() << " from " << wr.type.name << " " << wr.id << endl;
                        #endif
                        wb.weapons.push_back(wr.weapons[j]);
                    }
                }

                // Print combat result
                // Delete dead warriors(this has to be done after looting)
                if (wr.life <= 0 && wb.life <= 0)
                {
                    printf("%03d:40 both red %s %d and blue %s %d died in city %d\n",
                           time, wr.type.name.c_str(), wr.id, wb.type.name.c_str(), wb.id, i);
                    red.cities[i].pop_back();
                    blue.cities[i].pop_back();
                }
                else if (wb.life <= 0)
                {
                    printf("%03d:40 red %s %d killed blue %s %d in city %d remaining %d elements\n",
                           time, wr.type.name.c_str(), wr.id, wb.type.name.c_str(), wb.id, i, wr.life);
                    blue.cities[i].pop_back();
                }
                else if (wr.life <= 0)
                {
                    printf("%03d:40 blue %s %d killed red %s %d in city %d remaining %d elements\n",
                           time, wb.type.name.c_str(), wb.id, wr.type.name.c_str(), wr.id, i, wb.life);
                    red.cities[i].pop_back();
                }
                else
                {
                    printf("%03d:40 both red %s %d and blue %s %d were alive in city %d\n",
                           time, wr.type.name.c_str(), wr.id, wb.type.name.c_str(), wb.id, i);
                }

                // Handle Dragon
                if (wr.life > 0 && wr.type == dragon)
                {
                    printf("%03d:40 red dragon %d yelled in city %d\n", time, wr.id, i);
                }
                if (wb.life > 0 && wb.type == dragon)
                {
                    printf("%03d:40 blue dragon %d yelled in city %d\n", time, wb.id, i);
                }
            }
        }
    }

    static bool checkDeadLoop(Warrior *w)
    {
        if (w->weapons.size() == 0)
            return true;
        int swordDamage = (float)w->force * 2 / 10;
        for (auto &weapon : w->weapons)
        {
            if (weapon.type == bomb)
                return false;
            if (weapon.type == arrow)
                return false;
        }
        return swordDamage == 0;
    }
    // Handle attack
    // Remove the weapon if it's used
    // returns true if any damage is commited
    static bool handleAttack(Warrior *attacker, Warrior *defender, int weaponIndex = 0)
    {
        if (attacker->weapons.size() == 0)
            return false;
        Weapon &w = attacker->weapons[weaponIndex];
#ifdef DEBUG
        cout<< attacker->type.name <<" "<<attacker->id<<" used "<< w.getName() <<" on "<<defender->type.name<<" "<<defender->id<<endl;
#endif        
        int damage = 0;
        if (w.type == sword)
        {
            damage = (float)attacker->force * 2 / 10;
            defender->life -= damage;
            // Dont break
        }
        else if (w.type == bomb)
        {
            damage = (float)attacker->force * 4 / 10;
            defender->life -= damage;
            if (attacker->type != ninja)
                attacker->life -= damage / 2;
            attacker->removeWeapon(w);
        }
        else if (w.type == arrow)
        {
            damage = (float)attacker->force * 3 / 10;
            defender->life -= damage;
            w.usedTimes++;
            if (w.usedTimes == 2)
                attacker->removeWeapon(w);
        }
        return damage > 0;
    }

    void reportElementsInfo(int time)
    {
        printf("%03d:50 %d elements in %s headquarter\n",
               time, life, name.c_str());
    }

    static void reportWarriorInfo(int time, Team &red, Team &blue)
    {
        for (int i = 0; i <= N + 1; i++)
        {
            if (red.cities[i].size() > 0)
            {
                Warrior &w = red.cities[i].back();
                auto weaponInfo = w.weaponCount();
                printf("%03d:55 %s %s %d has %d sword %d bomb %d arrow and %d elements\n",
                       time, red.name.c_str(), w.type.name.c_str(), w.id, weaponInfo[sword], weaponInfo[bomb], weaponInfo[arrow], w.life);
            }
            if (blue.cities[i].size() > 0)
            {
                Warrior &w = blue.cities[i].back();
                auto weaponInfo = w.weaponCount();
                printf("%03d:55 %s %s %d has %d sword %d bomb %d arrow and %d elements\n",
                       time, blue.name.c_str(), w.type.name.c_str(), w.id, weaponInfo[sword], weaponInfo[bomb], weaponInfo[arrow], w.life);
            }
        }
    }
};

int Team::N = 0;

int main()
{
#ifdef DEBUG
    freopen("data2.in", "r", stdin);
#endif
    int testCasesCount;
    scanf("%d", &testCasesCount);
    for (int i = 1; i <= testCasesCount; i++)
    {
        // Handle Case input
        int m, n, k, t;
        int lifes[5];
        int forces[5];

        scanf("%d %d %d %d", &m, &n, &k, &t);
        for (int j = 0; j < 5; j++)
        {
            scanf("%d", &lifes[j]);
        }
        for (int j = 0; j < 5; j++)
        {
            scanf("%d", &forces[j]);
        }

        dragon.life = lifes[0];
        dragon.force = forces[0];
        ninja.life = lifes[1];
        ninja.force = forces[1];
        iceman.life = lifes[2];
        iceman.force = forces[2];
        lion.life = lifes[3];
        lion.loyalty_decay = k;
        lion.force = forces[3];
        wolf.life = lifes[4];
        wolf.force = forces[4];

        // Initialize Teams
        WarriorType sequenceRed[] = {iceman, lion, wolf, ninja, dragon};
        WarriorType sequenceBlue[] = {lion, dragon, ninja, iceman, wolf};
        WeaponType weaponsSequence[] = {sword, bomb, arrow};

        Team::N = n;

        Team red(sequenceRed, weaponsSequence, "red", m, n, 0);
        Team blue(sequenceBlue, weaponsSequence, "blue", m, n, n + 1);

        // Start Simulations
        printf("Case %d:\n", i);
        for (int time = 0;; time++)
        {
            //: 00
            if (time * 60 + 0 > t)
                break;
            red.spawnNewWarrior(time);
            blue.spawnNewWarrior(time);
            //: 05
            if (time * 60 + 5 > t)
                break;
            red.checkLionLoyalty(time);
            blue.checkLionLoyalty(time);
            //: 10
            if (time * 60 + 10 > t)
                break;
            red.marchForward();
            blue.marchForward();
            Team::handleMarchInfo(time, red, blue);
            //: 35
            if (time * 60 + 35 > t)
                break;
            Team::handleWolfRob(time, red, blue);
            //: 40
            if (time * 60 + 40 > t)
                break;
            Team::handleCombat(time, red, blue);
            //: 50
            if (time * 60 + 50 > t)
                break;
            red.reportElementsInfo(time);
            blue.reportElementsInfo(time);
            //: 55
            if (time * 60 + 55 > t)
                break;
            Team::reportWarriorInfo(time, red, blue);
        }
    }
    return 0;
}