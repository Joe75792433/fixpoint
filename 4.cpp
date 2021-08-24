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

int main(int argc, char *argv[]) {
    int N = 3;
    if (argc >= 2) {
        N = stoi(argv[1]);
    }
    
    map<pair<unsigned int, int>, long long> in_trouble, count_trouble;
    while (true) {
        long long datetime;
        unsigned int address, network_address;
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

        network_address = address & ((1LL << 32) - (1LL << (32 - prefix_length)));

        pair<unsigned int, int> key = make_pair(address, prefix_length);
        pair<unsigned int, int> key_network = make_pair(network_address, prefix_length);
        if (ping == -1) {
            if (in_trouble.count(key) == 0) {
                in_trouble[key] = datetime;
                count_trouble[key] = 1;
            } else {
                ++count_trouble[key];
            }
            if (in_trouble.count(key_network) == 0) {
                in_trouble[key_network] = datetime;
                count_trouble[key_network] = 1;
            } else {
                ++count_trouble[key_network];
            }
        } else {
            if (in_trouble.count(key) == 1) {
                if (count_trouble[key] >= N) {
                    long long start = in_trouble[key];
                    string output = get_output(address, prefix_length, start, datetime);
                    cout << output << endl;
                }
                in_trouble.erase(key);
                count_trouble.erase(key);
            }
            if (in_trouble.count(key_network) == 1) {
                if (count_trouble[key_network] >= N) {
                    long long start = in_trouble[key_network];
                    string output = get_output(network_address, prefix_length, start, datetime);
                    cout << output << endl;
                }
                in_trouble.erase(key_network);
                count_trouble.erase(key_network);
            }
        }
    }
    for (auto [key, start] : in_trouble) {
        if (count_trouble[key] >= N) {
            auto [address, prefix_length] = key;
            string output = get_output(address, prefix_length, start, -1);
            cout << output << endl;
        }
    }
    return 0;
}