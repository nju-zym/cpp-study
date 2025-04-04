#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

long long climbStairs(int n) {
    if (n == 0) {
        return 1;
    }
    if (n == 1) {
        return 1;
    }
    if (n == 2) {
        return 2;
    }

    // The maximum i that can affect future steps is floor(n / 2)
    int max_i = n / 2;

    // dp[k][mask] represents the number of ways to reach step k with the given mask
    unordered_map<int, unordered_map<unsigned long long, long long>> dp;
    dp[0][0] = 1;

    for (int k = 0; k <= n; ++k) {
        for (auto const& entry : dp[k]) {
            unsigned long long mask = entry.first;
            long long count         = entry.second;

            // Try to step 1 or 2
            for (int step = 1; step <= 2; ++step) {
                int next_k = k + step;
                if (next_k > n) {
                    continue;
                }

                bool is_blocked = false;
                if (next_k % 2 == 0) {
                    int i = next_k / 2;
                    if (i <= max_i && (mask & (1ULL << (i - 1)))) {
                        is_blocked = true;
                    }
                }

                if (!is_blocked) {
                    unsigned long long new_mask = mask;
                    if (next_k <= max_i) {
                        new_mask |= (1ULL << (next_k - 1));
                    }
                    dp[next_k][new_mask] += count;
                }
            }
        }
    }

    long long ans = 0;
    for (auto const& entry : dp[n]) {
        ans += entry.second;
    }

    return ans;
}

int main() {
    int n;
    cin >> n;
    cout << climbStairs(n) << endl;
    return 0;
}