#include <bits/stdc++.h>
using namespace std;

class String {
protected:
    char * chars;
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
        int * table = buildKMPTable(subStr.chars, subStr.length);
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
};

int main() {
    string s, ss;
    cout << "Enter the string: ";
    getline(cin, s);
    String string(s.c_str());
    cout << "\nEnter the substring: ";
    getline(cin, ss);
    String subString(ss.c_str());
    int count = 0, reverseCount = 0;
    
    cout << "\nALL SUBSTRINGS IN ORDER\n";
    vector<int> allMatches = string.findAllSubStrings(subString, count);
    cout << "All occurrences of " << ss << ": ";
    for (int i = 0; i < count; i++) {
        cout << allMatches[i] + 1 << " ";
    }
    cout << "\n----------------------------------------\n";

    cout << "FIRST SUBSTRING IN ORDER\n";
    int firstPosition = string.findFirstSubString(subString);
    cout << "First occurrence of " << ss << ": " << firstPosition + 1 << endl;
    cout << "----------------------------------------\n";

    cout << "ALL SUBSTRINGS IN REVERSE ORDER\n";
    vector <int> allReverseMatches = string.findAllSubStringsReverse(subString, reverseCount);
    cout << "All occurrences of " << ss << "in reverse order: ";
    for (int i = 0; i < reverseCount; i++) {
        cout << (string.getLength() - allReverseMatches[i] - subString.getLength() + 1) << " ";
    }
    cout << "\n----------------------------------------\n";

    cout << "FIRST SUBSTRING IN REVERSE ORDER\n";
    int firstReversePosition = string.findFirstSubStringReverse(subString);
    cout << "First occurrence of '" << ss << "' in reverse order: " << firstReversePosition + 1 << endl;
}