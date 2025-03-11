/**
 * @author: hlwdztr
 * @date: 2025/03/06
 * @version: 2.0
 */

#include <iostream>
#include <vector>
#include <map>

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
    WarriorType() {}
    WarriorType(string _name, int _life)
    {
        name = _name, life = _life;
    }
    bool operator<(const WarriorType &_b) const
    {
        return name < _b.name;
    }
    bool operator==(const WarriorType &_b) const
    {
        return name == _b.name;
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
    WeaponType(string _name, int _attack)
    {
        name = _name, attack = _attack;
    }
    operator string()
    {
        return name;
    }
    bool operator<(const WeaponType &_b) const
    {
        return name < _b.name;
    }
    bool operator==(const WeaponType &_b) const
    {
        return name == _b.name;
    }
    string getName()
    {
        return name;
    }
};

/*Global Vars*/
WarriorType dragon("dragon", 0);
WarriorType ninja("ninja", 0);
WarriorType iceman("iceman", 0);
WarriorType lion("lion", 0);
WarriorType wolf("wolf", 0);

WeaponType sword("sword", 0);
WeaponType bomb("bomb", 0);
WeaponType arrow("arrow", 0);
WeaponType none("none", 0);
/*End Global vars*/


/**
 * @class Weapon
 * 武器的实例化
 */
class Weapon
{
    WeaponType type;

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
    int loyalty;
    float morale;
    vector<Weapon> weapons;

public:
    Warrior(WarriorType &_type, int _id, int _loyalty = -1, float _morale = -1, vector<Weapon> _weapons = vector<Weapon>())
    {
        type = _type;
        id = _id;
        loyalty = _loyalty;
        morale = _morale;
        weapons = _weapons;
    }
    operator string()
    {
        return type.getName();
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
    WarriorType sequence[5];
    WeaponType weaponsSequence[3];
    map<WarriorType, vector<Warrior>> army;
    int idCounter;
    int cursor = 0;
    int life;

public:
    bool stopped = false;

public:
    Team(WarriorType _sequence[5], WeaponType _weaponSequence[3], string _name, int _life)
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
            printf("%03d %s headquarter stops making warriors\n",
                   time, name.c_str());
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

            army[newType].push_back(*p);

            // Print the message
            printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",
                   time, name.c_str(), p->type.name.c_str(), p->id, p->type.life, army[newType].size(), newType.name.c_str(), name.c_str());
            if (newType == dragon)
                printf("It has a %s,and it's morale is %.2f\n",
                    p->weapons[0].getName().c_str(), p->morale);
            else if (newType == ninja)
                printf("It has a %s and a %s\n",
                    p->weapons[0].getName().c_str(), p->weapons[1].getName().c_str());
            else if (newType == iceman)
                printf("It has a %s\n",
                    p->weapons[0].getName().c_str());
            else if (newType == lion)
                printf("It's loyalty is %d\n",
                    p->loyalty);
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
        int life;
        int lifes[5];
        scanf("%d", &life);
        for (int j = 0; j < 5; j++)
        {
            scanf("%d", &lifes[j]);
        }

        dragon.life = lifes[0];
        ninja.life = lifes[1];
        iceman.life = lifes[2];
        lion.life = lifes[3];
        wolf.life = lifes[4];

        // Initialize Teams
        WarriorType sequenceRed[] = {iceman, lion, wolf, ninja, dragon};
        WarriorType sequenceBlue[] = {lion, dragon, ninja, iceman, wolf};
        WeaponType weaponsSequence[] = {sword, bomb, arrow};

        Team red(sequenceRed, weaponsSequence, "red", life);
        Team blue(sequenceBlue, weaponsSequence, "blue", life);

        // Start Simulations
        cout << "Case:" << i << endl;
        int time = 0;
        while (true)
        {
            if (red.stopped && blue.stopped)
                break;
            red.spawnNewWarrior(time);
            blue.spawnNewWarrior(time);
            time++;
        }
    }
}