/*
1 - KMP Algorithm and reverse KMP Algorithm
2 - Rabin-Karp Algorithm
3 - Patricia Trie
*/
#include <bits/stdc++.h>
using namespace std;

class PatriciaTrie {
private:
    struct Node {
        unordered_map<char, Node*> children;
        bool isEndOfWord;
        Node() : isEndOfWord(false) {}
    };
    Node* root;
public:
    PatriciaTrie() {
        root = new Node();
    }
    ~PatriciaTrie() {
        clear(root);
    }

    void insert(const string& word) {
        Node* current = root;
        for (char ch : word) {
            if (!current->children.count(ch)) {
                current->children[ch] = new Node();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
    }

    bool search(const string& word) {
        Node* current = root;
        for (char ch : word) {
            if (!current->children.count(ch)) {
                return false;
            }
            current = current->children[ch];
        }
        return current->isEndOfWord;
    }
    bool startsWith(const string& prefix) {
        Node* current = root;
        for (char ch : prefix) {
            if (!current->children.count(ch)) {
                return false;
            }
            current = current->children[ch];
        }
        return true;
    }
    void getWordsWithPrefix(Node* current, string& prefix, vector<string>& result) {
        if (current->isEndOfWord) {
            result.push_back(prefix);
        }

        for (auto& child : current->children) {
            prefix.push_back(child.first);
            getWordsWithPrefix(child.second, prefix, result);
            prefix.pop_back();
        }
    }
    vector<string> getWordsWithPrefix(const string& prefix) {
        Node* current = root;
        vector<string> result;
        string currentPrefix = prefix;

        for (char ch : prefix) {
            if (!current->children.count(ch)) {
                return result;
            }
            current = current->children[ch];
        }

        getWordsWithPrefix(current, currentPrefix, result);
        return result;
    }

private:
    void clear(Node* node) {
        for (auto it = node->children.begin(); it != node->children.end(); ++it) {
            clear(it->second);
        }
        delete node;
    }
};

class String {
protected:
    char *chars;
    int length;
public:
    String() {
        this->chars = new char[2];
        strcpy(this->chars, " ");
        this->length = 0;
    }
    String(const String &s) {
        this -> chars = new char[strlen(s.chars) + 1];
        this -> length = s.length;
    }
    String &operator=(const String &other) {
        if (this != &other) {
            delete[] chars;
            length = other.length;
            chars = new char[length + 1];
            strcpy(chars, other.chars);
        }
        return *this;
    }
    String(const char *str) {
        length = strlen(str);
        chars = new char[length + 1];
        strcpy(chars, str);
    }
    ~String() {
        delete [] chars;
    }
    int getLength() {
        return length;
    }
    int *buildKMPTable(const char *pattern, int len) {
        int *table = new int[len];
        int j = 0;
        table[0] = 0;
        for (int i = 1; i < len; i++) {
            while (j > 0 && pattern[i] != pattern[j]) {
                j = table[j - 1];
            }
            if (pattern[i] == pattern[j]) {
                j++;
            }
            table[i] = j;
        }
        return table;
    }
    vector <int> findAllSubStrings(const String &subStr, int &count) {
        if (subStr.length == 0) {
            count = 0;
            return {};
        }
        int *table = buildKMPTable(subStr.chars, subStr.length);
        vector <int> matches;
        int j = 0;
        for (int i = 0; i < length; i++) {
            while (j > 0 && chars[i] != subStr.chars[j]) {
                j = table[j - 1];
            }
            if (chars[i] == subStr.chars[j]) {
                if (j == subStr.length - 1) {
                    matches.push_back(i - j);
                    j = table[j];
                }
                else {
                    j++;
                }
            }
        }
        delete [] table;
        count = matches.size();
        return matches;
    }
    int findFirstSubString(const String &subStr) {
        if (subStr.length == 0) {
            return -1;
        }
        int *table = buildKMPTable(subStr.chars, subStr.length);
        int j = 0;

        for (int i = 0; i < length; i++) {
            while (j > 0 && chars[i] != subStr.chars[j]) {
                j = table[j - 1];
            }
            if (chars[i] == subStr.chars[j]) {
                if (j == subStr.length - 1) {
                    delete [] table;
                    return i - j;
                }
                j++;
            }
        }
        delete [] table;
        return -1;
    }
    vector <int> findAllSubStringsReverse(const String &subStr, int &count) {
        String reversedString = reverseString(*this);
        String reversedSubString = reverseString(subStr);
        return reversedString.findAllSubStrings(reversedSubString, count);
    }
    int findFirstSubStringReverse(const String &subStr) {
        if (subStr.length == 0) {
            return -1;
        }
        int *table = buildKMPTable(subStr.chars, subStr.length);
        int j = subStr.length - 1;
        for (int i = length - 1; i >= 0; i--) {
            while (j < subStr.length - 1 && chars[i] != subStr.chars[j]) {
                j = table[j] - 1;
                if (j < 0) {
                    j = subStr.length - 1;
                }
            }
            if (chars[i] == subStr.chars[j]) {
                if (j == 0) {
                    delete [] table;
                    return i;
                }
                j--;
            }
        }
        delete [] table;
        return -1; 
    }
    String reverseString(const String &str) {
        String result;
        result.length = str.length;
        result.chars = new char[result.length + 1];
        for (int i = 0; i < result.length; i++) {
            result.chars[i] = str.chars[result.length - i - 1];
        }
        result.chars[result.length] = '\0';
        return result;
    }

    vector<int> rabinKarp(const String &subStr) {
        const int base = 26; 
        const int mod = 1e9 + 9;
        int S = getLength(), SS = subStr.length;
        
        vector<long long> posPow(max(S, SS));
        posPow[0] = 1;
        for (int i = 1; i < (int)posPow.size(); i++) {
            posPow[i] = (posPow[i - 1] * base) % mod;
        }
        
        vector<long long> hashes(S + 1, 0);
        for (int i = 0; i < S; i++) {
            hashes[i + 1] = (hashes[i] + (chars[i] - 'a' + 1) * posPow[i]) % mod;
        }
        
        long long hash = 0;
        for (int i = 0; i < SS; i++) 
            hash = (hash + (subStr.chars[i] - 'a' + 1) * posPow[i]) % mod; 

        vector<int> occurrences;
        int killCounter = SS;
        for (int i = 0; i + SS - 1 < S; i++) {
            long long currentHash = (hashes[i + SS] + mod - hashes[i]) % mod;
            if (currentHash == hash * posPow[i] % mod) {
                for (int j = 0; j < SS; j++) {
                    if (subStr.chars[j] == chars[i + j]) {
                        killCounter = killCounter - 1;
                        if (killCounter == 0) {
                            occurrences.push_back(i);
                        }
                    }
                }
                killCounter = SS;
            }
        }
        return occurrences;
    }
};

int main() {
    string s, ss, sss, ssss, prefix;
    int count = 0, reverseCount = 0;
    
    cout << "Enter the string: ";
    getline(cin, s);
    String string(s.c_str());
    
    cout << "\nEnter the substring: ";
    getline(cin, ss);
    String subString(ss.c_str());
    
    cout << "\nALL SUBSTRINGS IN ORDER\n";
    vector<int> allMatches = string.findAllSubStrings(subString, count);
    cout << "All occurrences of " << ss << " are on position: ";
    for (int i = 0; i < count; i++) {
        cout << allMatches[i] << " ";
    }
    cout << "\n----------------------------------------------------------\n";

    cout << "FIRST SUBSTRING IN ORDER\n";
    int firstPosition = string.findFirstSubString(subString);
    cout << "First occurrence of " << ss << " is on position: " << firstPosition;
    cout << "\n----------------------------------------------------------\n";

    cout << "ALL SUBSTRINGS IN REVERSE ORDER\n";
    vector <int> allReverseMatches = string.findAllSubStringsReverse(subString, reverseCount);
    cout << "All occurrences of " << ss << " in reverse order are on position: ";
    for (int i = 0; i < reverseCount; i++) {
        cout << (string.getLength() - allReverseMatches[i] - subString.getLength()) << " ";
    }
    cout << "\n----------------------------------------------------------\n";

    cout << "FIRST SUBSTRING IN REVERSE ORDER\n";
    int firstReversePosition = string.findFirstSubStringReverse(subString);
    cout << "First occurrence of " << ss << " in reverse order is on position: " << firstReversePosition;
    cout << "\n----------------------------------------------------------\n";
    
    cout << "ALL SUBSTRINGS WITH RABIN-KARP\n";
    vector <int> rabinKarpMatches = string.rabinKarp(subString);
    cout << "All occurrences of " << ss << " in order using Rabin-Karp are on position: ";
    for (int i = 0; i < rabinKarpMatches.size(); i++) {
        cout << rabinKarpMatches[i] << " ";
    }
    cout << "\n----------------------------------------------------------\n";
    
    PatriciaTrie Trie;
    cout << "\nEnter the english dictionary (write '!' when you finish inputing the words): ";
    while (cin >> sss) {
        if (sss == "!") {
            break;
        } else {
            Trie.insert(sss);
        }
    }
    cout << "\nEnter a word to search in the english dictionary (using Patricia Trie, write '!' when you finish inputing the words): ";
    while (cin >> ssss) {
        if (ssss == "!") {
            break;
        } else {
            if (Trie.search(ssss)) {
                cout << "The word " << ssss << " is found in the english dictionary." << endl;
            } else {
                cout << "The word " << ssss << " isn't found in the english dictionary." << endl;
            }
        }
    }
    cout << "Enter a prefix to search in the Patricia Trie: ";
    cin >> prefix;
    vector<std::string> wordsWithPrefix = Trie.getWordsWithPrefix(prefix);

    if (wordsWithPrefix.empty()) {
        cout << "No words start with the prefix '" << prefix << "' in the Trie." << endl;
    } else {
        cout << "Words that start with the prefix '" << prefix << "': ";
        reverse(wordsWithPrefix.begin(), wordsWithPrefix.end());
        for (const std::string& word : wordsWithPrefix) {
            cout << "[" << word << "] ";
        }
    }

    return 0;
}
