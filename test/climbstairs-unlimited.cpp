#include <bitset>  // Required for std::bitset
#include <chrono>  // 添加 chrono 头文件用于计时
#include <iostream>
#include <unordered_map>
#include <vector>

// --- Configuration ---
// Define the maximum value of n this program is compiled to support.
// Needs to be large enough for the maximum expected input n.
// The size of the bitset will be MAX_N + 1 to represent stairs 1 to MAX_N.
// WARNING: Increasing MAX_N significantly increases memory usage for the memoization table
//          and does NOT fix the exponential time complexity O(n * 2^n).
//          This approach is likely feasible only for n up to around 25-30.
int const MAX_N = 1000;  // Example: Support n up to 64. Increase if needed and feasible.

// Type alias for the bitset representing blocked stairs
using BlockedMask = std::bitset<MAX_N + 1>;

// Use unsigned long long for counts to accommodate potentially very large results.
using CountType = unsigned long long;

// Memoization table:
// memo[i] stores results for states where the current stair is 'i'.
// The map maps a blockedMask (bitset) to the number of ways (CountType).
std::vector<std::unordered_map<BlockedMask, CountType>> memo;

/**
 * @brief Recursive function with memoization using std::bitset for blocked stairs.
 *
 * @param current The current stair (0 for ground).
 * @param n The target stair.
 * @param blockedMask A bitset representing blocked stairs. blockedMask[k] is true if stair k is blocked.
 * @return The number of distinct ways to reach stair n.
 */
CountType countWaysRecursive(int current, int n, BlockedMask blockedMask) {
    // Base case: Reached the target stair.
    if (current == n) {
        return 1;
    }

    // Memoization check: Look up state (current, blockedMask).
    // Check if the map at memo[current] contains the key blockedMask.
    // Note: Directly using operator[] would insert a default value if not found,
    //       so using find() or count() is preferred for checking existence.
    auto it = memo[current].find(blockedMask);
    if (it != memo[current].end()) {
        return it->second;  // Return stored result
    }

    CountType totalWays = 0;

    // --- Explore possible moves ---

    // Option 1: Try 1 step
    int nextStair1 = current + 1;
    if (nextStair1 <= n) {
        // Check if nextStair1 is blocked. bitset::test() checks the bit value.
        if (!blockedMask.test(nextStair1)) {
            // Not blocked, calculate next state's mask.
            BlockedMask nextBlockedMask = blockedMask;
            int stairToBlock1           = 2 * nextStair1;
            // Only block if the stair to block is within the range [1, n]
            // and also within the capacity of our bitset (<= MAX_N).
            // The second check is implicitly handled if n <= MAX_N.
            if (stairToBlock1 <= n) {
                nextBlockedMask.set(stairToBlock1);  // Set the bit for the newly blocked stair.
            }
            // Recursive call for the next state.
            totalWays += countWaysRecursive(nextStair1, n, nextBlockedMask);
        }
    }

    // Option 2: Try 2 steps
    int nextStair2 = current + 2;
    if (nextStair2 <= n) {
        // Check if nextStair2 is blocked.
        if (!blockedMask.test(nextStair2)) {
            // Not blocked, calculate next state's mask.
            BlockedMask nextBlockedMask = blockedMask;
            int stairToBlock2           = 2 * nextStair2;
            if (stairToBlock2 <= n) {
                nextBlockedMask.set(stairToBlock2);
            }
            // Recursive call for the next state.
            totalWays += countWaysRecursive(nextStair2, n, nextBlockedMask);
        }
    }

    // --- Memoization ---
    // Store the computed result for the current state before returning.
    return memo[current][blockedMask] = totalWays;
}

int main() {
    int n = 0;  // Input: Total number of stairs
    std::cout << "Enter the total number of stairs (n): ";
    std::cin >> n;

    // Basic input validation
    if (n <= 0) {
        std::cout << "Number of ways: 0" << '\n';
        return 0;
    }

    // Check if the input n exceeds the compiled maximum MAX_N.
    if (n > MAX_N) {
        std::cerr << "Error: Input n=" << n << " exceeds the maximum supported value MAX_N=" << MAX_N << "." << '\n';
        std::cerr << "Please increase MAX_N in the source code and recompile if necessary." << '\n';
        std::cerr << "Be aware of the exponential time complexity which limits practical values of n." << '\n';
        return 1;  // Indicate error
    }

    // 开始计时
    auto startTime = std::chrono::high_resolution_clock::now();

    // Resize the memoization table to accommodate indices 0 to n.
    memo.resize(n + 1);

    // Create the initial state: Start at stair 0, no stairs are blocked initially.
    BlockedMask initialMask;  // Creates a bitset with all bits set to 0.

    // Start the recursive calculation.
    CountType result = countWaysRecursive(0, n, initialMask);

    // 结束计时
    auto endTime = std::chrono::high_resolution_clock::now();

    // 计算耗时（毫秒）
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Output the final result.
    std::cout << "Number of ways to reach stair " << n << ": " << result << '\n';

    // 打印运行时间
    std::cout << "算法运行时间: " << duration << " 毫秒" << '\n';
    if (duration > 1000) {
        std::cout << "              " << (duration / 1000.0) << " 秒" << '\n';
    }

    return 0;
}