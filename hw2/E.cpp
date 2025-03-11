#include <iostream>
#include <vector>
#include <map>

using namespace std;

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
    operator string()
    {
        return name;
    }
    bool operator<(const WarriorType &_b) const
    {
        return name < _b.name;
    }
};

class Warrior
{
public:
    int id;
    WarriorType type;

public:
    Warrior(WarriorType &_type, int _id)
    {
        type = _type, id = _id;
    }
};

class Team
{
private:
    string name;
    WarriorType sequence[5];
    map<WarriorType, vector<Warrior>> army;
    int idCounter;
    int cursor = 0;
    int life;

public:
    bool stopped = false;

public:
    Team(WarriorType _sequence[5], string _name, int _life)
    {
        for (int i = 0; i < 5; i++)
        {
            sequence[i] = _sequence[i];
            idCounter = 0;
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
            WarriorType newType = sequence[cursor];
            Warrior *p = new Warrior(newType, ++idCounter);
            army[newType].push_back(*p);

            life -= newType.life;
            cursor++;
            cursor %= 5;

            printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",
                   time, name.c_str(), p->type.name.c_str(), p->id, p->type.life, army[newType].size(),newType.name.c_str(), name.c_str());
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
        WarriorType dragon("dragon", lifes[0]);
        WarriorType ninja("ninja", lifes[1]);
        WarriorType iceman("iceman", lifes[2]);
        WarriorType lion("lion", lifes[3]);
        WarriorType wolf("wolf", lifes[4]);

        WarriorType sequenceRed[] = {iceman, lion, wolf, ninja, dragon};
        WarriorType sequenceBlue[] = {lion, dragon, ninja, iceman, wolf};

        Team red(sequenceRed, "red", life);
        Team blue(sequenceBlue, "blue", life);

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