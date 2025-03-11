#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm> // for std::begin and std::end

using namespace std;

class WType {
public:
    string name;
    int life;

    WType(string _name = "", int _life = 0) : name(_name), life(_life) {}

    operator string() const { return name; }

    bool operator<(const WType &other) const { return name < other.name; }
};

class Warrior {
public:
    int id;
    WType type;

    Warrior(const WType &type, int id) : type(type), id(id) {}
};

class Team {
private:
    string name;
    WType sequence[5];
    map<WType, vector<Warrior>> army;
    int idCounter = 0;
    int cursor = 0;
    int life;
    
public:
    bool stopped = false;

    Team(WType seq[5], const string &teamName, int teamLife)
        : name(teamName), life(teamLife) {
        copy(std::begin(seq), std::end(seq), std::begin(sequence));
    }

    void spawnNewWarrior(int time) {
        if (stopped) return;

        int cnt = 0;
        while (life < sequence[cursor].life) {
            cursor = (cursor + 1) % 5;
            cnt++;
            if (cnt == 5) {
                stopped = true;
                break;
            }
        }

        if (cnt == 5) {
            printf("%03d %s headquarter stops making warriors\n", time, name.c_str());
        } else {
            WType newType = sequence[cursor];
            auto p = make_unique<Warrior>(newType, ++idCounter);
            army[newType].push_back(*p);

            life -= newType.life;
            cursor = (cursor + 1) % 5;

            printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",
                   time, name.c_str(), p->type.name.c_str(), p->id, p->type.life,
                   army[newType].size(), newType.name.c_str(), name.c_str());
        }
    }
};

int main() {
    int testCasesCount;
    cin >> testCasesCount;

    for (int i = 1; i <= testCasesCount; i++) {
        int life;
        int lifes[5];
        cin >> life;
        for (int j = 0; j < 5; j++) cin >> lifes[j];

        WType dragon("dragon", lifes[0]);
        WType ninja("ninja", lifes[1]);
        WType iceman("iceman", lifes[2]);
        WType lion("lion", lifes[3]);
        WType wolf("wolf", lifes[4]);

        WType sequenceRed[] = {iceman, lion, wolf, ninja, dragon};
        WType sequenceBlue[] = {lion, dragon, ninja, iceman, wolf};

        Team red(sequenceRed, "red", life);
        Team blue(sequenceBlue, "blue", life);

        cout << "Case:" << i << endl;
        int time = 0;
        while (!(red.stopped && blue.stopped)) {
            red.spawnNewWarrior(time);
            blue.spawnNewWarrior(time);
            time++;
        }
    }
}
