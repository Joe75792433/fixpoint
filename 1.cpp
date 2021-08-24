#include <utility>
#include <map>
#include <string>
#include <iostream>
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

string get_output(unsigned int address, int prefix_length, long long start, long long end) {
    string res = get_string_from_address(address);
    res += '/';
    res += to_string(prefix_length);
    res += ',';
    res += to_string(start);
    res += ',';
    res += end == -1 ? "-" : to_string(end);
    return res;
}

int main() {
    map<pair<unsigned int, int>, long long> in_trouble;
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

        if (ping == -1) {
            if (in_trouble.count(make_pair(address, prefix_length)) == 0) {
                in_trouble[make_pair(address, prefix_length)] = datetime;
            }
        } else {
            if (in_trouble.count(make_pair(address, prefix_length)) == 1) {
                long long start = in_trouble[make_pair(address, prefix_length)];
                string output = get_output(address, prefix_length, start, datetime);
                cout << output << endl;
                in_trouble.erase(make_pair(address, prefix_length));
            }
        }
    }
    for (auto [key, start] : in_trouble) {
        auto [address, prefix_length] = key;
        string output = get_output(address, prefix_length, start, -1);
        cout << output << endl;
    }
    return 0;
}