#include <bits/stdc++.h>
#include <fstream>
#include <windows.h>
using namespace std;

wstring IndexToLetter(int i) {
    const wstring str [31] = {
        L"\u00d0\u00b0", L"\u00d0\u00b1", L"\u00d0\u00b2", L"\u00d0\u00b3", 
        L"\u00d0\u00b4", L"\u00d1\u0093", L"\u00d0\u00b5", L"\u00d0\u00b6", 
        L"\u00d0\u00b7", L"\u00d1\u0095", L"\u00d0\u00b8", L"\u00d1\u0098", 
        L"\u00d0\u00ba", L"\u00d0\u00bb", L"\u00d1\u0099", L"\u00d0\u00bc", 
        L"\u00d0\u00bd", L"\u00d1\u009a", L"\u00d0\u00be", L"\u00d0\u00bf", 
        L"\u00d1\u0080", L"\u00d1\u0081", L"\u00d1\u0082", L"\u00d1\u009c", 
        L"\u00d1\u0083", L"\u00d1\u0084", L"\u00d1\u0085", L"\u00d1\u0086", 
        L"\u00d1\u0087", L"\u00d1\u009f", L"\u00d1\u0088"
    };
    return str[i];
}
    
int LetterToIndex(wchar_t second_half) {
    switch (second_half) {
        case  L'\u00b0': { return 0; }
        case  L'\u00b1': { return 1; }
        case  L'\u00b2': { return 2; }
        case  L'\u00b3': { return 3; }
        case  L'\u00b4': { return 4; }
        case  L'\u0093': { return 5; }
        case  L'\u00b5': { return 6; }
        case  L'\u00b6': { return 7; }
        case  L'\u00b7': { return 8; }
        case  L'\u0095': { return 9; }
        case  L'\u00b8': { return 10; }
        case  L'\u0098': { return 11; }
        case  L'\u00ba': { return 12; }
        case  L'\u00bb': { return 13; }
        case  L'\u0099': { return 14; }
        case  L'\u00bc': { return 15; }
        case  L'\u00bd': { return 16; }
        case  L'\u009a': { return 17; }
        case  L'\u00be': { return 18; }
        case  L'\u00bf': { return 19; }
        case  L'\u0080': { return 20; }
        case  L'\u0081': { return 21; }
        case  L'\u0082': { return 22; }
        case  L'\u009c': { return 23; }
        case  L'\u0083': { return 24; }
        case  L'\u0084': { return 25; }
        case  L'\u0085': { return 26; }
        case  L'\u0086': { return 27; }
        case  L'\u0087': { return 28; }
        case  L'\u009f': { return 29; }
        case  L'\u0088': { return 30; }
        }
    }
    wstring TransformString(wstring word) {
        wstring rez;
        for(int i = 1; i < word.size(); i+=2) {
            rez += word[i];
        }
        return rez;
    }

struct TrieNode {
    TrieNode *child[31] = { nullptr };
    bool isWord = false;
};

class Trie {
    TrieNode* root;
    int AlphabetSize = 31;
    void preorder(TrieNode *node, wstring str, wostream &out) {
        if (node->isWord) {
            out << str << endl;
        }
        for (int i = 0; i < AlphabetSize; i++) {
            if (node->child[i] != nullptr) {
                preorder(node->child[i], str + IndexToLetter(i), out);
            }
        }
    }
public:
    Trie() {
        root = new TrieNode();
    }
    ~Trie() {
        if (root != nullptr)
            delete root;
    }
    void insert(wstring s) {
        s = TransformString(s);
        TrieNode *cur = root;
        for (int i = 0; i < s.length(); i++) {
            int ind = LetterToIndex(s[i]);
            if (cur->child[ind] == nullptr) {
                cur->child[ind] = new TrieNode();
            }
            cur = cur->child[ind];
            if (i + 1 == s.length()) {
                cur->isWord = true;
            }
        }
    }
    bool search(wstring s) {
        s = TransformString(s);
        TrieNode *cur = root;
        for (int i = 0; i < s.length(); i++) {
            int ind = LetterToIndex(s[i]);
            if (cur->child[ind] == nullptr) {
                return false;
            }
            cur = cur->child[ind];
        }
        return true;
    }
    void print(wostream &out) {
        preorder(root, wstring(), out);
    }
};

int main() {
    wifstream in("Dictionary.txt");
    wofstream out("Output.txt");
    wstring word;
    Trie trie;
    while (in >> word) {
        out << word;
        trie.insert(word);
        out << " " << trie.search(word) << endl;
    }
    out << endl;

    trie.print(out);
    in.close();
    out.close();
}
/*
Run in cmd: g++ Mkd_Trie.cpp, a.exe
Result in 'Output.txt'
The output is going to write which word is searched and a number
(1 or 0, true if the word is found or false if the word isn't found)
*/