#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

class Player {
public:
    string name;
    string role;

    Player(string name, string role) {
        this->name = name;
        this->role = role;
    }

    string getName() {
        return name;
    }

    string getRole() {
        return role;
    }
};

class Batsman : public Player {
public:
    float battingAverage;
    float strikeRate;

    Batsman(string name, string role, float battingAverage, float strikeRate) : Player(name, role) {
        this->battingAverage = battingAverage;
        this->strikeRate = strikeRate;
    }

    float getBattingAverage() {
        return battingAverage;
    }

    float getStrikeRate() {
        return strikeRate;
    }
};

class Bowler : public Player {
public:
    float bowlingAverage;
    float economyRate;

    Bowler(string name, string role, float bowlingAverage, float economyRate) : Player(name, role) {
        this->bowlingAverage = bowlingAverage;
        this->economyRate = economyRate;
    }

    float getBowlingAverage() {
        return bowlingAverage;
    }

    float getEconomyRate() {
        return economyRate;
    }
};

class WicketKeeper : public Player {
public:
    int stumpings;
    int catches;

    WicketKeeper(string name, string role, int stumpings, int catches) : Player(name, role) {
        this->stumpings = stumpings;
        this->catches = catches;
    }

    int getStumpings() {
        return stumpings;
    }

    int getCatches() {
        return catches;
    }
};

class Team {
public:
    string name;
    vector<Player*> players;

    Team(string name) {
        this->name = name;
    }

    void addPlayer(Player* player) {
        players.push_back(player);
    }

    string getName() {
        return name;
    }

    vector<Player*> getPlayers() {
        return players;
    }

    void displayDetails() {
        cout << "Team: " << name << endl;
        cout << "Players:" << endl;
        for (Player* player : players) {
            cout << "Name: " << player->getName() << ", Role: " << player->getRole() << endl;
            
            if (player->getRole() == "Batsman") {
                Batsman* batsman = static_cast<Batsman*>(player);
                cout << "Batting Average: " << batsman->getBattingAverage() << ", Strike Rate: " << batsman->getStrikeRate() << endl;
            } else if (player->getRole() == "Bowler") {
                Bowler* bowler = static_cast<Bowler*>(player);
                cout << "Bowling Average: " << bowler->getBowlingAverage() << ", Economy Rate: " << bowler->getEconomyRate() << endl;
            } else if (player->getRole() == "WicketKeeper") {
                WicketKeeper* wicketKeeper = static_cast<WicketKeeper*>(player);
                cout << "Stumpings: " << wicketKeeper->getStumpings() << ", Catches: " << wicketKeeper->getCatches() << endl;
            }
        }
    }

    ~Team() {
        for (Player* player : players) {
            delete player;
        }
    }
};

class Scoreboard {
public:
    Team* battingTeam;
    Team* bowlingTeam;

    int battingTeamScore = 0;
    int bowlingTeamScore = 0;

    int battingTeamWicketsTaken = 0;
    int bowlingTeamWicketsTaken = 0;

    int currentOver = 0;

    float currentRunRate1 = 0.0f;
    float currentRunRate2 = 0.0f;

    float requiredRunRate2 = 0.0f;

    Scoreboard(Team* battingTeam, Team* bowlingTeam) {
        this->battingTeam = battingTeam;
        this->bowlingTeam = bowlingTeam;
    }

    void updateScoreboard(int runs, int wickets, Team* team) {
        if (team == battingTeam) {
            battingTeamScore += runs;
            battingTeamWicketsTaken += wickets;
            currentRunRate1 = static_cast<float>(battingTeamScore) / (currentOver + 1);
        } else if (team == bowlingTeam) {
            bowlingTeamScore += runs;
            bowlingTeamWicketsTaken += wickets;
            currentRunRate2 = static_cast<float>(bowlingTeamScore) / (currentOver + 1);
        }

        if (battingTeamWicketsTaken == 10) {
            requiredRunRate2 = static_cast<float>(battingTeamScore + 1) / (5 - currentOver);
        }
    }

    void displayScoreboard() {
        cout << "Scoreboard:" << endl;
        cout << "Batting team: " << battingTeam->getName() << endl;
        cout << "Batting team score: " << battingTeamScore << endl;
        cout << "Batting team wickets taken: " << battingTeamWicketsTaken << endl;
        cout << "Current run rate: " << currentRunRate1 << endl;

        cout << "Bowling team: " << bowlingTeam->getName() << endl;
        cout << "Bowling team score: " << bowlingTeamScore << endl;
        cout << "Bowling team wickets taken: " << bowlingTeamWicketsTaken << endl;
        cout << "Current run rate: " << currentRunRate2 << endl;

        if (battingTeamWicketsTaken == 10) {
            cout << "Required run rate for the bowling team: " << requiredRunRate2 << endl;
        }
    }
};

class Match {
public:
    vector<Team*> teams;
    Scoreboard* scoreboard;

    Match(vector<Team*> teams) {
        this->teams = teams;
        scoreboard = new Scoreboard(teams[0], teams[1]);
    }

    void playMatch() {
        for (int innings = 0; innings < 2; innings++) {
            scoreboard->currentOver = 0;
            scoreboard->battingTeamWicketsTaken = 0; // Reset for new innings

            while (scoreboard->currentOver < 5 && scoreboard->battingTeamWicketsTaken < 10) {
                int totalRuns = rand() % 31; // Random runs between 0 and 30
                int wickets = rand() % 3; // Random wickets between 0 and 2

                // Make sure to not exceed 10 wickets
                if (scoreboard->battingTeamWicketsTaken + wickets > 10) {
                    wickets = 10 - scoreboard->battingTeamWicketsTaken;
                }

                scoreboard->updateScoreboard(totalRuns, wickets, teams[innings]);
                scoreboard->currentOver++;
            }

            if (innings == 0) {
                swap(scoreboard->battingTeam, scoreboard->bowlingTeam);
            }
        }

        cout << "Match simulation completed." << endl;
    }

    Scoreboard* getScoreboard() {
        return scoreboard;
    }

    ~Match() {
        delete scoreboard;
    }
};

int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    vector<Team*> teams;

    for (int i = 0; i < 2; i++) {
        string teamName;
        cout << "Enter the name of Team " << (i + 1) << ": ";
        cin >> teamName;
        Team* team = new Team(teamName);

        int numPlayers;
        cout << "Enter the number of players for " << teamName << ": ";
        cin >> numPlayers;

        for (int j = 0; j < numPlayers; j++) {
            string playerName, role;
            float battingAverage, strikeRate, bowlingAverage, economyRate;
            int stumpings, catches;

            cout << "Enter details for Player " << (j + 1) << " of " << teamName << ":" << endl;
            cout << "Name: ";
            cin >> playerName;
            cout << "Role (Batsman/Bowler/WicketKeeper): ";
            cin >> role;

            if (role == "Batsman") {
                cout << "Batting Average: ";
                cin >> battingAverage;
                cout << "Strike Rate: ";
                cin >> strikeRate;
                team->addPlayer(new Batsman(playerName, role, battingAverage, strikeRate));
            } else if (role == "Bowler") {
                cout << "Bowling Average: ";
                cin >> bowlingAverage;
                cout << "Economy Rate: ";
                cin >> economyRate;
                team->addPlayer(new Bowler(playerName, role, bowlingAverage, economyRate));
            } else if (role == "WicketKeeper") {
                cout << "Stumpings: ";
                cin >> stumpings;
                cout << "Catches: ";
                cin >> catches;
                team->addPlayer(new WicketKeeper(playerName, role, stumpings, catches));
            } else {
                cout << "Invalid role. Please enter 'Batsman', 'Bowler', or 'WicketKeeper'." << endl;
                j--;
            }
        }

        teams.push_back(team);
    }

    Match match(teams);
    match.playMatch();
    match.getScoreboard()->displayScoreboard();

    // Clean up teams
    for (Team* team : teams) {
        delete team;
    }

    return 0;
}
