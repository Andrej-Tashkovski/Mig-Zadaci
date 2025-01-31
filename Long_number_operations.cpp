#include <bits/stdc++.h>
using namespace std;

class Calculation {
protected:
    vector <int> result;
public:
    void Addition (vector<int> v1, vector<int> v2) {
        reverse(v1.begin(), v1.end());
        reverse(v2.begin(), v2.end());
        while (v1.size() != v2.size()) {
            if (v1.size() > v2.size()) {
                v2.push_back(0);
            } else {
                v1.push_back(0);
            }
        }

        int carry = 0;
        for (int i = 0; i < v1.size(); i++) {
            result.push_back((v1[i] + v2[i] + carry) % 10);
            carry = (v1[i] + v2[i] + carry) / 10;
        }

        if (carry) { result.push_back(carry); }
        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }

        reverse(result.begin(), result.end());
        for (int i = 0; i < result.size(); i++) {
            cout << result[i] << " ";
        }
    }

    void Subtraction (vector<int> v1, vector<int> v2) {
        if (v1 == v2) { cout << "0" << endl; return; }

        bool n = false;
        if (v1.size() < v2.size() || (v1.size() == v2.size() && v1 < v2)) {
            swap(v1, v2);
            n = true;
        }
        
        reverse(v1.begin(), v1.end());
        reverse(v2.begin(), v2.end());
        
        while (v1.size() != v2.size()) {
            v2.push_back(0);
        }
        
        int carry = 0;
        for (int i = 0; i < v1.size(); i++) {
            if ((v1[i] - v2[i] - carry) >= 0) {
                result.push_back(v1[i] - v2[i] - carry);
                carry = 0;
            } else {
                result.push_back(10 + (v1[i] - v2[i] - carry));
                carry = 1;
            }
        }

        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }

        reverse(result.begin(), result.end());
        if (n) { cout << "-"; }
        for (int i = 0; i  < result.size(); i++) {
            cout << result[i] << " ";
        }
    }

    void Multiplication (vector<int> v1, vector<int> v2) {
        reverse(v1.begin(), v1.end());
        reverse(v2.begin(), v2.end());
        vector<int> tempResult(v1.size() + v2.size(), 0);

        int sum = 0, product = 0;
        for (int i = 0; i < v1.size(); i++) {
            for (int j = 0; j < v2.size(); j++) {
                product = v1[i] * v2[j];
                sum = tempResult[i + j] + product;

                tempResult[i + j] = sum % 10;
                tempResult[i + j + 1] += sum / 10;
            }
        }

        while (tempResult.size() > 1 && tempResult.back() == 0) {
            tempResult.pop_back();
        }

        result = tempResult;
        reverse(result.begin(), result.end());
        for (int i = 0; i < result.size(); i++) {
            cout << result[i] << " ";
        }
    }

    /*
    void Division (vector<int> v1, vector<int> v2) {
        if (v1.size() < v2.size() || (v1.size() == v2.size() && v1 < v2)) {
            cout << "0" << endl;
            return;
        }

        vector <int> segment;
        vector <int> tempResult;
        int s = v2.size();
        copy(v1.begin(), v1.begin() + s, back_inserter(segment));
        if (v2[0] > segment[0]) {
            segment.push_back(v1[s + 1]);
        }
        for (int i = 0; i < v1.size(); i++) {
            tempResult.push_back()
        }
    }
    */
};

int main() {
    int n, m;
    char c;
    vector <int> v1;
    vector <int> v2;
    cin >> c;
    cin.ignore();
    cout << "Input a letter to stop the loop!" << endl;
    cout << "Enter the first digits: " << endl;
    while (cin >> n) {
        v1.push_back(n);
    }
    cin.clear();
    cin.ignore();
    cout << "Enter the second digits: " << endl;
    while (cin >> m) {
        v2.push_back(m);
    }

    Calculation number;
    if (c == '+') {
        number.Addition(v1, v2);
    } else if (c == '-') {
        number.Subtraction(v1, v2);
    } else if (c == '*') {
        number.Multiplication(v1, v2);
    } /*else if (c == '/') {
        number.Division(v1, v2);
    }*/
    
    return 0;
}