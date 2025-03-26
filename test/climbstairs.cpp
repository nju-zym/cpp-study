#include <iostream>
#include <unordered_map>
#include <vector>

// Using a global vector of unordered_maps for memoization.
// The size of the vector will be determined by the input 'n' at runtime.
// memo[i] will store the results for states where the current stair is 'i'.
// The map within memo[i] maps a blockedMask (long long) to the number of ways (long long).
std::vector<std::unordered_map<long long, long long>> memo;

/**
 * @brief Recursive function with memoization to count the ways to climb the stairs.
 *
 * This function calculates the number of distinct ways to reach the n-th stair
 * starting from the 'current' stair, given a set of already 'blocked' stairs
 * represented by 'blockedMask'.
 *
 * @param current The current stair the person is on (starts at 0, representing the ground).
 * @param n The total number of stairs (the target stair to reach).
 * @param blockedMask A bitmask representing the set of blocked stairs.
 *                    The k-th bit (using 1-based indexing for stairs) is 1 if stair k is blocked,
 *                    and 0 otherwise. We use a long long to store the mask.
 * @return The number of distinct ways to reach stair n from the current state.
 */
long long countWaysRecursive(int current, int n, long long blockedMask) {
    // Base case: If we have successfully reached the target n-th stair.
    if (current == n) {
        return 1;  // This represents one valid way found.
    }

    // Check memoization table first.
    // The state is defined by the pair (current, blockedMask).
    // We look up memo[current] and check if blockedMask exists as a key.
    if (memo[current].count(blockedMask)) {
        // If the result for this state is already computed, return it directly.
        return memo[current][blockedMask];
    }

    // Initialize the total number of ways from this state to 0.
    long long totalWays = 0;

    // --- Explore possible moves ---

    // Option 1: Try to take 1 step
    int nextStair1 = current + 1;
    // Check if the next step is within the bounds of the staircase (up to n).
    if (nextStair1 <= n) {
        // Check if the next stair (nextStair1) is blocked.
        // We check if the nextStair1-th bit is set in the blockedMask.
        // `(1LL << nextStair1)` creates a mask with only the nextStair1-th bit set.
        // If the bitwise AND is non-zero, the stair is blocked.
        if (!(blockedMask & (1LL << nextStair1))) {
            // If the stair is not blocked, we can take this step.
            // Now, calculate the state for the recursive call.
            // Landing on nextStair1 blocks stair `stairToBlock1 = 2 * nextStair1`.
            long long nextBlockedMask = blockedMask;  // Start with the current mask.
            int stairToBlock1         = 2 * nextStair1;

            // We only need to update the mask if the stair to be blocked (stairToBlock1)
            // is within the range [1, n], because blocked stairs > n do not affect
            // the paths to reach n.
            if (stairToBlock1 <= n) {
                // Set the bit corresponding to stairToBlock1 in the next mask.
                nextBlockedMask |= (1LL << stairToBlock1);
            }

            // Make the recursive call for the state after taking 1 step and update total ways.
            totalWays += countWaysRecursive(nextStair1, n, nextBlockedMask);
        }
        // If nextStair1 is blocked, we cannot take this path, so we add 0 ways (do nothing).
    }

    // Option 2: Try to take 2 steps
    int nextStair2 = current + 2;
    // Check if the next step is within the bounds of the staircase (up to n).
    if (nextStair2 <= n) {
        // Check if the next stair (nextStair2) is blocked.
        if (!(blockedMask & (1LL << nextStair2))) {
            // If the stair is not blocked, we can take this step.
            // Calculate the state for the recursive call.
            // Landing on nextStair2 blocks stair `stairToBlock2 = 2 * nextStair2`.
            long long nextBlockedMask = blockedMask;  // Start with the current mask.
            int stairToBlock2         = 2 * nextStair2;

            // Only update the mask if the stair to be blocked is within the range [1, n].
            if (stairToBlock2 <= n) {
                // Set the bit corresponding to stairToBlock2 in the next mask.
                nextBlockedMask |= (1LL << stairToBlock2);
            }

            // Make the recursive call for the state after taking 2 steps and update total ways.
            totalWays += countWaysRecursive(nextStair2, n, nextBlockedMask);
        }
        // If nextStair2 is blocked, we cannot take this path, contribution is 0.
    }

    // --- Memoization ---
    // Store the computed result (totalWays) for the current state (current, blockedMask)
    // in the memoization table before returning.
    return memo[current][blockedMask] = totalWays;
}

int main() {
    int n;  // Input: Total number of stairs
    std::cin >> n;

    // Handle edge case: If n is 0 or negative, there are no ways to reach the top.
    if (n <= 0) {
        std::cout << 0 << std::endl;
        return 0;
    }

    // Practical limit check: The bitmask uses a 64-bit signed long long.
    // `(1LL << k)` works reliably for k up to 62. If n is 63 or larger,
    // using `1LL << n` might lead to issues (overflow or negative values depending on architecture).
    // We add a check here, assuming standard C++ environments.
    if (n >= 63) {
        std::cerr << "Error: Input n=" << n << " is too large for the 64-bit signed long long bitmask approach used."
                  << std::endl;
        // Consider using unsigned long long or arbitrary precision integers if n can be larger.
        return 1;  // Indicate error
    }

    // Complexity Note: The time complexity of this solution is O(n * 2^n) due to the state space size.
    // This is generally feasible only for small values of n (e.g., n <= 24).
    // For larger n, this program would likely exceed typical time limits (e.g., 1-2 seconds).
    // The problem constraints were not given, so this solution assumes n is small enough.

    // Initialize the memoization table.
    // 'memo' is a vector of size n+1 (for indices 0 to n). Each element is an unordered_map
    // that stores computed results for different blockedMasks at that 'current' stair level.
    memo.resize(n + 1);

    // Start the recursive calculation.
    // We begin at stair 0 (the ground) and initially, no stairs are blocked (blockedMask = 0LL).
    long long result = countWaysRecursive(0, n, 0LL);

    // Output the final result, which is the total number of ways to reach the n-th stair.
    std::cout << result << std::endl;

    return 0;
}