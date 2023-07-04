
#include <bits/stdc++.h>

using namespace std;


class Event {
public:
    string day;
    int start_time;
    int end_time;
    vector<string>users;
    Event(string day, int start_time, int duration, vector<string>users) {
        this->day = day;
        this->start_time = start_time;
        this->end_time = start_time + duration;
        this->users = users;
    }
};

class Calendar {
private:
    string user;
    vector<Event*>events;
public:
    Calendar(string user) {
        this->user = user;
    }
    void addEvent(Event* event) {
        events.push_back(event);
    }
    void showEvents(string day) {
        if (events.size() == 0)cout << "none" << endl;
        for (auto u : events) {
            if (u->day == day) {
                cout << u->start_time << "-" << u->end_time << " ";
                for (auto k : u->users)cout << k << " ";
                cout << endl;
            }
        }
    }
};

map<string, Calendar*> user_map;
map<string, vector<Event*>>event_map;

// 1. addUser function.
void addUser(string user) {
    if (user_map[user] == nullptr) {
        Calendar *calendar = new Calendar(user);
        user_map[user] = calendar;
        cout << "success" << endl;
    }
    else cout << "failure" << endl;
}
void createEvent(string day, int start_time, int duration, int n, vector<string> users) {
    map<string, int>vis;
    for (int j = 0; j < n; ++j) {
        vis[users[j]]++;
    }
    vector<Event*> eve = event_map[day];
    bool flag = false;
    // checking if event is colliding or not.
    for (auto u : eve) {
        if (u->start_time >= start_time + duration || u->end_time <= start_time) {}
        else {
            for (auto k : u->users) {
                if (vis.find(k) != vis.end()) {
                    cout << "failure" << endl;
                    flag = true;
                    break;
                }
            }
        }
        if (flag)break;
    }

    if (flag)return;

    // checking if all users are registered or not.
    for (auto u : users) {
        if (user_map.find(u) == user_map.end()) {
            cout << "failure" << endl;
            flag = true;
            break;
        }
    }
    if (flag)return;

    Event* n_event = new Event(day, start_time, duration, users);
    event_map[day].push_back(n_event);
    for (auto u : users) {
        if (user_map[u] != nullptr)user_map[u]->addEvent(n_event);
    }

    cout << "success" << endl;

}
void suggestSlots(string day, int start_range, int end_range, int duration, int n, vector<string> users) {
    map<string, int>vis;
    bool f = 0;
    for (int j = 0; j < n; ++j) {
        vis[users[j]]++;
    }
    vector<Event*> eve = event_map[day];
    vector<int>count(1440, 0);
    for (auto u : eve) {
        int q = 0;
        for (auto k : u->users) {
            if (vis[k]) {
                q = 1;
                break;
            }
        }
        if (q) { // partial sum
            count[u->start_time]++;
            count[u->end_time]--;
        }
    }
    for (int j = start_range + 1; j < end_range; ++j) {
        count[j] += count[j - 1];
    }
    // prefix sum
    for (int j = start_range + 1; j < end_range; ++j) {
        count[j] += count[j - 1];
    }
    bool flag = false;
    for (int j = start_range; j + duration <= end_range; ++j) {
        if (start_range == 0) {
            if (count[j + duration - 1] == 0) {
                cout << j << endl;
                flag = true;
                break;
            }
        }
        else {
            if (count[j + duration - 1] - count[j - 1] == 0) {
                cout << j << endl;
                flag = true;
                break;
            }
        }
    }
    if (!flag)cout << "none" << endl;
}

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        if (s == "add-user") {
            string user;
            cin >> user;
            addUser(user);
        }
        else if (s == "create-event") { //O(n*events)
            string day;
            int start_time, duration, no;
            cin >> day >> start_time >> duration >> no;
            vector<string>users(no);
            for (int j = 0; j < no; ++j) {
                cin >> users[j];
            }
            createEvent(day, start_time, duration, no, users);
        }
        else if (s == "show-events") { //O(events*n)
            string day, user;
            cin >> day >> user;
            if (user_map[user] == nullptr) {
                cout << "failure" << endl;
                continue;
            }
            user_map[user]->showEvents(day);
        }
        else if (s == "suggest-slot") { // O(max(1440,n*events)).
            string day;
            int start_range, end_range, duration, no;
            cin >> day >> start_range >> end_range >> duration >> no;
            vector<string>users(no);
            for (int j = 0; j < no; ++j) cin >> users[j];
            suggestSlots(day, start_range, end_range, duration, no, users);
        }
    }
    return 0;
}