/**
 * @author: hlwdztr
 * @date: 2025/03/11
 * @version: 3.0
 */

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#define DEBUG

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
    WeaponType() {}
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
        //因为是sword, bomb, arrow可以用反字典序排
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
        if(type != _b.type)
            return type < _b.type;
        else
            return usedTimes < _b.usedTimes;
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

    Warrior(WarriorType &_type, int _id, int _loyalty = -1, float _morale = -1, vector<Weapon> _weapons = vector<Weapon>())
    {
        type = _type;
        id = _id;
        loyalty = _loyalty;
        morale = _morale;
        weapons = _weapons;
        life = type.life;
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
    map<WeaponType, vector<Weapon &>> classifiedWeapons()
    {
        map<WeaponType, vector<Weapon &>> res;
        res[sword] = vector<Weapon &>();
        res[bomb] = vector<Weapon &>();
        res[arrow] = vector<Weapon &>();
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
private:
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
                vector<Weapon> weapons = {weapon};
                p = new Warrior(newType, n, -1, -1, weapons);
            }
            else if (newType == lion)
            {
                int loaylty = life;
                p = new Warrior(newType, n, loaylty);
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
        for (int i = 0; i <= N; i++)
        {
            if (cities[i].size() > 0)
            {
                Warrior &w = cities[i].back();
                if (w.type == iceman)
                {
                    w.life -= w.type.life * 0.1; // Not dead yet
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
        for(int i=0; i<=N+1; i++) {
            if(red.cities[i].size() > 0) {
                Warrior &w = red.cities[i].back();
                printMarchInfo(time, w, red, blue, i);
            }
            if(blue.cities[i].size() > 0) {
                Warrior &w = blue.cities[i].back();
                printMarchInfo(time, w, blue, red, i);
            }
        }
    }
    static void printMarchInfo(int time, Warrior &w, Team& warriorTeam, Team& enemyTeam, int city) {
        if(city == N + 1 - warriorTeam.baseLocation) {// Reach enemy base
            printf("%03d:10 %s %s %d reached %s headquarter with %d elements and force %d\n",
                   time, warriorTeam.name.c_str(), w.type.name.c_str(), w.id, enemyTeam.name.c_str(), w.life, w.force);
        }
        else {
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
                if (w1.type == wolf && w2.type != wolf)
                {
                    auto weapons = w2.classifiedWeapons();
                    for (WeaponType w : {sword, bomb, arrow})
                    {
                        int number = weapons[w].size();
                        if (number > 0 && number <= MAX_ROBBED)
                        {
                            for (int j = 0; j < number; j++)
                            {
                                w1.weapons.push_back(weapons[w][j]);
                                w2.removeWeapon(weapons[w][j]);
                            }
                            printRobInfo(time, red.name, w1.id, number, w.getName(), blue.name, w2.type.getName(), w2.id, i);
                            return;
                        }
                        else if (number > MAX_ROBBED)
                        {
                            sort(weapons[w].begin(), weapons[w].end());
                            for (int j = 0; j < MAX_ROBBED; j++)
                            {
                                w1.weapons.push_back(weapons[w][j]);
                                w2.removeWeapon(weapons[w][j]);
                            }
                            printRobInfo(time, red.name, w1.id, MAX_ROBBED, w.getName(), blue.name, w2.type.getName(), w2.id, i);
                            return;
                        }
                    }
                }
                else if (w2.type == wolf && w1.type != wolf)
                {
                    auto weapons = w1.classifiedWeapons();
                    for (WeaponType w : {sword, bomb, arrow})
                    {
                        int number = weapons[w].size();
                        if (number > 0 && number <= MAX_ROBBED)
                        {
                            for (int j = 0; j < number; j++)
                            {
                                w2.weapons.push_back(weapons[w][j]);
                                w1.removeWeapon(weapons[w][j]);
                            }
                            printRobInfo(time, blue.name, w2.id, number, w.getName(), red.name, w1.type.getName(), w1.id, i);
                            return;
                        }
                        else if (number > MAX_ROBBED)
                        {
                            sort(weapons[w].begin(), weapons[w].end());
                            for (int j = 0; j < MAX_ROBBED; j++)
                            {
                                w2.weapons.push_back(weapons[w][j]);
                                w1.removeWeapon(weapons[w][j]);
                            }
                            printRobInfo(time, blue.name, w2.id, MAX_ROBBED, w.getName(), red.name, w1.type.getName(), w1.id, i);
                            return;
                        }
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
                // TODO: Combat
            }
        }
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
                printf("%03d:55 %s %s %d has %d sword %d bomb %d arrow and %d elements",
                       time, red.name.c_str(), w.type.name.c_str(), w.id, weaponInfo[sword], weaponInfo[bomb], weaponInfo[arrow], w.life);
            }
            if (blue.cities[i].size() > 0)
            {
                Warrior &w = blue.cities[i].back();
                auto weaponInfo = w.weaponCount();
                printf("%03d:55 %s %s %d has %d sword %d bomb %d arrow and %d elements",
                       time, blue.name.c_str(), w.type.name.c_str(), w.id, weaponInfo[sword], weaponInfo[bomb], weaponInfo[arrow], w.life);
            }
        }
    }
};

int main()
{
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
        wolf.life = lifes[4];
        wolf.force = forces[4];

        // Initialize Teams
        WarriorType sequenceRed[] = {iceman, lion, wolf, ninja, dragon};
        WarriorType sequenceBlue[] = {lion, dragon, ninja, iceman, wolf};
        WeaponType weaponsSequence[] = {sword, bomb, arrow};

        Team red(sequenceRed, weaponsSequence, "red", m, n, 0);
        Team blue(sequenceBlue, weaponsSequence, "blue", m, n, n + 1);

        // Start Simulations
        cout << "Case:" << i << endl;
        for (int time = 0; time <= t; time++)
        {
            if (red.stopped && blue.stopped)
                break;
            //: 00
            red.spawnNewWarrior(time);
            blue.spawnNewWarrior(time);
            //: 05
            red.checkLionLoyalty(time);
            blue.checkLionLoyalty(time);
            //: 10
            red.marchForward();
            blue.marchForward();
            Team::handleMarchInfo(time, red, blue);
            //: 35
            Team::handleWolfRob(time, red, blue);
            //: 40
            Team::handleCombat(time, red, blue);
            //: 50
            red.reportElementsInfo(time);
            blue.reportElementsInfo(time);
            //: 55
            Team::reportWarriorInfo(time, red, blue);

            time++;
        }
    }
}