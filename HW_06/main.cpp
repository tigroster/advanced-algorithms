#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

struct number{
    bool is_prime = true;
};

vector<long long> prime_numbers;

int F, M_max;
long long x0, x1;
long long max_x0_x1;

vector<number> all_numbers;

int result = 0;
vector<int> prime_numbers_product{1, 2, 6, 30, 210, 2310, 30030, 510510};

void sieve_of_tigranothenes() {
    for (int i = 2; i <= M_max / prime_numbers_product[F-1]; i++) {
        if (all_numbers[i].is_prime) {
            prime_numbers.push_back(i);
            for (int j = i; j <= M_max / prime_numbers_product[F-1]; j += i) {
                all_numbers[j].is_prime = false;
            }
        }
    }
}

void generate_all_possible_M(int start, vector<long long>& current, const vector<long long>& nums,  int k, long long M, long long M_factors_product) {
    if (M > M_max) {
        return;
    }

    if (max_x0_x1 < M) {
        long long A = M_factors_product + 1;
        if (M % 4 == 0) {
            A = M_factors_product * 2 + 1;
        }
        if (A < M) {
            long long C = (x1 - A * x0) % M;
            if (__gcd(M, abs(C)) == 1) {
                result++;
            }
        }
    }


    for (int i = start; i < nums.size(); ++i) {
        current.push_back(nums[i]);
        generate_all_possible_M(i, current, nums, k - 1, M * nums[i], M_factors_product);
        current.pop_back();
    }
}


long long all_possible_factors_combinations(int start, vector<long long>& current, long long factors_product) {
    long long f = factors_product;

    if (current.size() == F) {

        if (factors_product * 2 <= M_max) {
            vector<long long> currentCombination;
            generate_all_possible_M(0, currentCombination, current, F, factors_product, factors_product);
        }
        return factors_product;
    }


    for (int i = start; i < prime_numbers.size() - F; ++i) {

        current.push_back(prime_numbers[i]);

        if (factors_product * (F - current.size()) * prime_numbers[i] * prime_numbers[i+1] * 2 <= M_max && f * 2 <= M_max) {
            f = all_possible_factors_combinations(i + 1, current, factors_product * prime_numbers[i]);
        }

        current.pop_back();
    }
    return factors_product;
}


int main() {

    cin >> F >> M_max >> x0 >> x1;

    max_x0_x1 = max(x0, x1);
    all_numbers.resize(M_max + 1);

    sieve_of_tigranothenes();

    vector<long long> current;
    all_possible_factors_combinations(0, current, 1);

    cout << result;

    return 0;
}
