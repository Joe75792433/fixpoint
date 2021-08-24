#include <utility>
#include <map>
#include <string>
#include <iostream>
#include <queue>
using namespace std;

string get_string_from_address(unsigned int address) {
    string res = "";
    for (int i = 0; i < 3; ++i) {
        int seg = address & ((1 << 8) - 1);
        res = string(".") + to_string(seg) + res;
        address >>= 8;
    }
    res = to_string(address) + res;
    return res;
}

string get_output(unsigned int address, int prefix_length, long long start, long long end, bool trouble) {
    string res = trouble ? "trouble," : "busy,";
    res += get_string_from_address(address);
    res += '/';
    res += to_string(prefix_length);
    res += ',';
    res += to_string(start);
    res += ',';
    res += end == -1 ? "-" : to_string(end);
    return res;
}

int main(int argc, char *argv[]) {
    int N = 3, m = 3, t = 100;
    if (argc >= 2) {
        N = stoi(argv[1]);
    }
    if (argc >= 3) {
        m = stoi(argv[2]);
    }
    if (argc >= 4) {
        t = stoi(argv[3]);
    }
    
    map<pair<unsigned int, int>, long long> in_trouble, count_trouble;
    map<pair<unsigned int, int>, long long> in_busy;
    map<pair<unsigned int, int>, queue<int>> ping_log;
    map<pair<unsigned int, int>, int> sum_ping_log;
    while (true) {
        long long datetime;
        unsigned int address;
        int prefix_length;
        int ping;

        string input;

        getline(cin, input, ',');
        if (cin.eof()) break;
        datetime = stoll(input);

        address = 0;
        for (int i = 0; i < 3; ++i) {
            getline(cin, input, '.');
            address += stoi(input);
            address <<= 8;
        }
        getline(cin, input, '/');
        address += stoi(input);

        getline(cin, input, ',');
        prefix_length = stoi(input);

        getline(cin, input);
        ping = input == "-" ? -1 : stoi(input);

        pair<unsigned int, int> key = make_pair(address, prefix_length);
        if (ping == -1) {
            if (in_trouble.count(key) == 0) {
                in_trouble[key] = datetime;
                count_trouble[key] = 1;
            } else {
                ++count_trouble[key];
            }
        } else {
            if (in_trouble.count(key) == 1) {
                if (count_trouble[key] >= N) {
                    long long start = in_trouble[key];
                    string output = get_output(address, prefix_length, start, datetime, true);
                    cout << output << endl;
                }
                in_trouble.erase(key);
                count_trouble.erase(key);
            }

            if (ping_log[key].size() == m) {
                sum_ping_log[key] -= ping_log[key].front();
                ping_log[key].pop();
            }
            sum_ping_log[key] += ping;
            ping_log[key].push(ping);
            if (ping_log[key].size() == m) {
                if (sum_ping_log[key] >= m * t) {
                    if (in_busy.count(key) == 0) {
                        in_busy[key] = datetime;
                    }
                } else {
                    if (in_busy.count(key) == 1) {
                        long long start = in_busy[key];
                        string output = get_output(address, prefix_length, start, datetime, false);
                        cout << output << endl;
                        in_busy.erase(key);
                    }
                }
            }
        }
    }
    for (auto [key, start] : in_trouble) {
        if (count_trouble[key] >= N) {
            auto [address, prefix_length] = key;
            string output = get_output(address, prefix_length, start, -1, true);
            cout << output << endl;
        }
    }
    for (auto [key, start] : in_busy) {
        auto [address, prefix_length] = key;
        string output = get_output(address, prefix_length, start, -1, false);
        cout << output << endl;
    }
    return 0;
}