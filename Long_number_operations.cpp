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

    void Division (vector<int> v1, vector<int> v2) {
        if (v1[0] == 0 || v2[0] == 0) {
            cout << "0" << endl;
            return;
        } else if (v1.size() == v2.size() && v1 == v2) {
            cout << "1" << endl;
            return;
        }

        vector <int> segment;
        int s = v2.size();
        for (int i = 0; i < s; i++) {
            segment.push_back(v1[i]);
        }

        if (v2[0] > segment[0]) {
            segment.push_back(v1[s]);
            result.push_back(100);
        } else if (v2[0] == segment[0]) {
            for (int i = 0; i < segment.size(); i++) {
                if (segment[i] > v2[i]) {
                    continue;
                } else {
                    segment.push_back(v1[s + 1]);
                    result.push_back(100);
                }
            }
        }
        
        Sub(v1, v2, segment);
        
        int k = 0;
        if (result[0] == 100) {
            cout << "0 , ";
            k = 1;
        }
        for (int i = k; i < result.size(); i++) {
            cout << result[i] << " ";
        }
    }

    vector<int> Sub (vector<int> v1, vector<int> v2, vector<int> segment) {
        reverse(segment.begin(), segment.end());
        reverse(v2.begin(), v2.end());
        int s = v2.size();

        while (segment.size() != v2.size()) {
            v2.push_back(0);
        }


        int carry = 0, count = 0;
        bool n = false;
        for (int i = 0; i + s < v1.size(); i++) {

            while (!n) {
            n = false;

                for (int j = 0; j < segment.size(); j++) {
                    if ((segment[j] - v2[j] - carry) >= 0) {
                        result.push_back(segment[j] - v2[j] - carry);
                        carry = 0;
                    } else {
                        result.push_back(10 + (segment[j] - v2[j] - carry));
                        carry = 1;
                    }
                    if (v2[0] > segment[0] || v2.size() > segment.size()) {
                        n = true;
                    }
                    count++;
                }

                result.push_back(count);
            }
            
        }
        
        return result;
    }

    void Factorization(vector<int> number) {
        vector<pair<int, int>> factors = factorize(number);

        cout << "Prime factorization of the number is: ";
        for (size_t i = 0; i < factors.size(); i++) {
            cout << factors[i].first << "^" << factors[i].second;
            if (i < factors.size() - 1) {
                cout << " * ";
            }
        }
        cout << endl;
    }

    bool isDivisible(vector<int>& number, int divisor) {
        long long remainder = 0;
        for (int digit : number) {
            remainder = (remainder * 10 + digit) % divisor;
        }
        return remainder == 0;
    }

    void divideNumber(vector<int>& number, int divisor) {
        long long remainder = 0;
        for (size_t i = 0; i < number.size(); i++) {
            long long temp = remainder * 10 + number[i];
            number[i] = temp / divisor;
            remainder = temp % divisor;
        }
        while (!number.empty() && number[0] == 0) {
            number.erase(number.begin());
        }
    }

    vector<pair<int, int>> factorize(vector<int> number) {
        vector<pair<int, int>> factors;

        if (isDivisible(number, 2)) {
            int exponent = 0;
            while (isDivisible(number, 2)) {
                exponent++;
                divideNumber(number, 2);
            }
            factors.push_back({2, exponent});
        }

        for (int i = 3; i <= 1e6; i += 2) {
            if (isDivisible(number, i)) {
                int exponent = 0;
                while (isDivisible(number, i)) {
                    exponent++;
                    divideNumber(number, i);
                }
                factors.push_back({i, exponent});
            }
        }

        if (!number.empty() && number.size() > 0) {
            string remainingNumber;
            for (int digit : number) {
                remainingNumber += to_string(digit);
            }
            cout << "Remaining prime factor: " << remainingNumber << endl;
        }

        return factors;
    }

};

int main() {
    int n, m;
    char c;
    vector <int> v1;
    vector <int> v2;
    vector <int> v3 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
                        1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
                        1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };     // strictly for factorization
    cout << "Input a sign: +, -, *, /, | (factorization)" << endl;
    cin >> c;
    cin.ignore();
    Calculation number;
    if (c == '|') {
        number.Factorization(v3);
    } else {
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
    }
    
    
    if (c == '+') {
        number.Addition(v1, v2);
    } else if (c == '-') {
        number.Subtraction(v1, v2);
    } else if (c == '*') {
        number.Multiplication(v1, v2);
    } else if (c == '/') {
        number.Division(v1, v2);
    }    
    
    return 0;
}
