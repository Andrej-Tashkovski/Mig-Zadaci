#include <bits/stdc++.h>
using namespace std;

class Calculation {
private:
    vector<int> digits;
    bool n;

    static Calculation Add(const Calculation& a, const Calculation& b) {
        Calculation result;
        result.digits.clear();
        int carry = 0;
        int i = a.digits.size() - 1;
        int j = b.digits.size() - 1;

        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) sum += a.digits[i--];
            if (j >= 0) sum += b.digits[j--];
            result.digits.push_back(sum % 10);
            carry = sum / 10;
        }

        reverse(result.digits.begin(), result.digits.end());
        return result;
    }

    static Calculation Sub(const Calculation& a, const Calculation& b) {
        Calculation result;
        result.digits.clear();
        int carry = 0;
        int i = a.digits.size() - 1;
        int j = b.digits.size() - 1;

        while (i >= 0 || j >= 0) {
            int diff = carry;
            if (i >= 0) diff += a.digits[i--];
            if (j >= 0) diff -= b.digits[j--];

            if (diff < 0) {
                diff += 10;
                carry = -1;
            } else {
                carry = 0;
            }

            result.digits.push_back(diff);
        }

        while (result.digits.size() > 1 && result.digits.back() == 0) { result.digits.pop_back(); }
        reverse(result.digits.begin(), result.digits.end());
        return result;
    }

    static Calculation Mult(const Calculation& a, const Calculation& b) {
        Calculation result;
        result.digits.resize(a.digits.size() + b.digits.size(), 0);

        for (int i = a.digits.size() - 1; i >= 0; i--) {
            for (int j = b.digits.size() - 1; j >= 0; j--) {
                int mul = a.digits[i] * b.digits[j];
                int sum = mul + result.digits[i + j + 1];
                result.digits[i + j + 1] = sum % 10;
                result.digits[i + j] += sum / 10;
            }
        }

        while (result.digits.size() > 1 && result.digits[0] == 0) {
            result.digits.erase(result.digits.begin());
        }

        return result;
    }

    static Calculation ModPow(const Calculation& a, const Calculation& b, const Calculation& mod) {
        Calculation result("1");
        Calculation base = a % mod;
        Calculation exponent = b;

        while (!exponent.isEqual(Calculation("0"))) {
            if ((exponent % Calculation("2")).isEqual(Calculation("1"))) { result = (result * base) % mod; }
            base = (base * base) % mod;
            exponent = exponent / Calculation("2");
        }

        return result;
    }

    static bool checkComposite(const Calculation& n, const Calculation& a, const Calculation& d, int s) {
        Calculation num = ModPow(a, d, n);
        if (num.isEqual(Calculation("1")) || num.isEqual(n - Calculation("1"))) {
            return false;
        }

        for (int r = 1; r < s; r++) {
            num = ModPow(num, Calculation("2"), n);
            if (num.isEqual(n - Calculation("1"))) {
                return false;
            }
        }

        return true;
    }

public:
    Calculation() { n = false; digits.push_back(0); }

    Calculation(const string& num) : n(false) {
        int start = 0;
        if (num[0] == '-') {
            n = true;
            start = 1;
        }

        for (int i = start; i < num.size(); i++) {
            if (isdigit(num[i])) {
                digits.push_back(num[i] - '0');
            }
        }
        
        if (digits.empty()) { digits.push_back(0); }
    }

    Calculation(long long num) : n(false) {
        if (num < 0) {
            n = true;
            num = -num;
        }
        
        if (num == 0) {
            digits.push_back(0);
        } else {
            while (num > 0) {
                digits.push_back(num % 10);
                num /= 10;
            }

            reverse(digits.begin(), digits.end());
        }
    }

    Calculation operator+(const Calculation& v) const {
        Calculation result;
        if (n && v.n) {
            result = Add(*this, v);
            result.n = true;
        } else if (n) {
            if (isGreaterOrEqual(v)) {
                result = Sub(*this, v);
                result.n = true;
            } else {
                result = Sub(v, *this);
            }
        } else if (v.n) {
            if (isGreaterOrEqual(v)) {
                result = Sub(*this, v);
            } else {
                result = Sub(v, *this);
                result.n = true;
            }
        } else {
            result = Add(*this, v);
        }

        return result;
    }

    Calculation operator-(const Calculation& v) const {
        Calculation result;
        if (n && v.n) {
            if (isGreaterOrEqual(v)) {
                result = Sub(*this, v);
                result.n = true;
            } else {
                result = Sub(v, *this);
            }
        } else if (n) {
            result = Add(*this, v);
            result.n = true;
        } else if (v.n) {
            result = Add(*this, v);
        } else {
            if (isGreaterOrEqual(v)) {
                result = Sub(*this, v);
            } else {
                result = Sub(v, *this);
                result.n = true;
            }
        }

        return result;
    }

    Calculation operator*(const Calculation& v) const {
        Calculation result = Mult(*this, v);
        result.n = (n != v.n);
        return result;
    }

    Calculation operator/(const Calculation& v) const {
        if (v.digits.size() == 1 && v.digits[0] == 0) {
            throw runtime_error("Division by zero");
        }

        if (digits.size() == 1 && digits[0] == 0) {
            return Calculation(0);
        }

        bool resultIsNegative = (n != v.n);
        Calculation dividend = *this;
        dividend.n = false;
        Calculation divisor = v;
        divisor.n = false;
        if (!dividend.isGreaterOrEqual(divisor)) {
            return Calculation(0);
        }

        Calculation result;
        result.digits.clear();
        Calculation currentDividend;
        currentDividend.digits.clear();

        for (int i = 0; i < dividend.digits.size(); i++) {
            currentDividend.digits.push_back(dividend.digits[i]);
            while (currentDividend.digits.size() > 1 && currentDividend.digits[0] == 0) {
                currentDividend.digits.erase(currentDividend.digits.begin());
            }

            if (!currentDividend.isGreaterOrEqual(divisor)) {
                result.digits.push_back(0);
                continue;
            }

            int quotientDigit = 0;
            while (currentDividend.isGreaterOrEqual(divisor)) {
                currentDividend = Sub(currentDividend, divisor);
                quotientDigit++;
            }

            result.digits.push_back(quotientDigit);
        }

        while (result.digits.size() > 1 && result.digits[0] == 0) {
            result.digits.erase(result.digits.begin());
        }

        result.n = resultIsNegative;
        return result;
    }

    Calculation operator%(const Calculation& v) const {
        if (v.digits.size() == 1 && v.digits[0] == 0) {
            throw runtime_error("Modulo by zero");
        }

        if (digits.size() == 1 && digits[0] == 0) {
            return Calculation(0);
        }

        Calculation dividend = *this;
        dividend.n = false;
        Calculation divisor = v;
        divisor.n = false;

        if (!dividend.isGreaterOrEqual(divisor)) {
            return dividend;
        }

        Calculation currentDividend = dividend;
        while (currentDividend.isGreaterOrEqual(divisor)) {
            currentDividend = Sub(currentDividend, divisor);
        }

        currentDividend.n = n;
        return currentDividend;
    }

    void print() const {
        if (n) { cout << "-"; }
        for (int digit : digits) {
            cout << digit;
        }
    }

    static Calculation genRand(int minDigits = 1, int maxDigits = 7) {
        Calculation result;
        result.digits.clear();
        int digitsSize = minDigits + rand() % (maxDigits - minDigits + 1);
        result.digits.push_back(1 + rand() % 9);

        for (int i = 1; i < digitsSize; i++) {
            result.digits.push_back(rand() % 10);
        }

        return result;
    }

    bool isGreaterOrEqual(const Calculation& other) const {
        if (digits.size() != other.digits.size()) {
            return digits.size() > other.digits.size();
        }

        for (int i = 0; i < digits.size(); i++) {
            if (digits[i] != other.digits[i]) {
                return digits[i] > other.digits[i];
            }
        }

        return true;
    }

    bool isEqual(const Calculation& other) const {
        if (digits.size() != other.digits.size() || n != other.n) {
            return false;
        }

        for (int i = 0; i < digits.size(); i++) {
            if (digits[i] != other.digits[i]) {
                return false;
            }
        }
        
        return true;
    }

    vector<pair<Calculation, int>> Factorization() const {
        vector<pair<Calculation, int>> result;
        Calculation num = *this;
        num.n = false;

        for (Calculation i = Calculation("2"); num.isGreaterOrEqual(i * i); i = i + Calculation("1")) {
            int k = 0;
            while ((num % i).isEqual(Calculation("0"))) {
                k++;
                num = num / i;
            }
            if (k) result.push_back(make_pair(i, k));
            if (num.isEqual(Calculation("1"))) break;
        }

        if (num.isGreaterOrEqual(Calculation("1"))) result.push_back(make_pair(num, 1));
        return result;
    }

    bool isPrime(int iterations = 5) {
        if (n || (digits.size() == 1 && digits[0] == 0)) { return false; }
        if (digits.size() == 1 && (digits[0] == 2 || digits[0] == 3 || digits[0] == 5)) { return true; }
        if (digits.size() == 1 && (digits[0] == 1 || digits[0] % 2 == 0)) { return false; }
    
        Calculation d = *this - Calculation("1");
        int s = 0;
        while ((d % Calculation("2")).isEqual(Calculation("0"))) {
            d = d / Calculation("2");
            s++;
        }
    
        for (int i = 0; i < iterations; i++) {
            Calculation a = Calculation("2") + (genRand() % (*this - Calculation("3")));
            if (checkComposite(*this, a, d, s)) {
                return false;
            }
        }
    
        return true;
    }

};

int main() {
    srand(time(0));
    string s1, s2, s3, s4;

    cout << "Enter the two numbers using the operations: +, -, *, /" << endl;
    cin >> s1 >> s2;
    Calculation num1(s1);
    Calculation num2(s2);

    Calculation sum = num1 + num2;
    Calculation diff = num1 - num2;
    Calculation prod = num1 * num2;
    Calculation quot = num1 / num2;

    cout << "Sum: ";
    sum.print();

    cout << "\nDifference: ";
    diff.print();

    cout << "\nProduct: ";
    prod.print();

    cout << "\nQuotient: ";
    quot.print();

    cout << "\nEnter a number to factorize:" << endl;
    cin >> s3;
    Calculation num3(s3);
    vector<pair<Calculation, int>> factors = num3.Factorization();
    for (const auto& factor : factors) {
        factor.first.print();                   // Print the factor
        cout << "^" << factor.second << endl;   // Print the exponent
    }
    
    cout << "Enter a number to test for primality: ";
    cin >> s4;
    Calculation num4(s4);
    if (num4.isPrime()) {
        cout << s4 << " is prime." << endl;
    } else {
        cout << s4 << " is composite." << endl;
    }

    return 0;
}