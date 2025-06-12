#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

/**
 * @brief Generates a vector with random integers.
 * 
 * @param size Number of elements in the vector.
 * @return vector<int> Vector filled with random integers.
 */
vector<int> generateRandomVector(size_t size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1000000);
    vector<int> vec(size);
    for (auto& x : vec) x = dis(gen);
    return vec;
}

/**
 * @brief Sorts a subrange of a vector.
 * 
 * @param vec The vector to sort.
 * @param start Starting index.
 * @param end Ending index (exclusive).
 */
void sortSubrange(vector<int>& vec, size_t start, size_t end) {
    sort(vec.begin() + start, vec.begin() + end);
}

/**
 * @brief Merges two sorted subranges into a single sorted range.
 * 
 * @param vec The original vector with sorted subranges.
 * @param leftStart Start of the first subrange.
 * @param rightStart Start of the second subrange.
 * @param rightEnd End of the second subrange.
 */
void mergeSortedHalves(vector<int>& vec, size_t leftStart, size_t rightStart, size_t rightEnd) {
    vector<int> result;
    size_t i = leftStart, j = rightStart;
    while (i < rightStart && j < rightEnd) {
        if (vec[i] < vec[j]) result.push_back(vec[i++]);
        else result.push_back(vec[j++]);
    }
    while (i < rightStart) result.push_back(vec[i++]);
    while (j < rightEnd) result.push_back(vec[j++]);
    copy(result.begin(), result.end(), vec.begin() + leftStart);
}

/**
 * @brief Sorts a vector using multithreading (2 threads).
 * 
 * @param vec The vector to sort.
 */
void parallelSort(vector<int>& vec) {
    size_t mid = vec.size() / 2;
    thread t1(sortSubrange, ref(vec), 0, mid);
    thread t2(sortSubrange, ref(vec), mid, vec.size());
    t1.join();
    t2.join();
    mergeSortedHalves(vec, 0, mid, vec.size());
}

/**
 * @brief Sorts a vector sequentially using std::sort.
 * 
 * @param vec The vector to sort.
 */
void sequentialSort(vector<int>& vec) {
    sort(vec.begin(), vec.end());
}

/**
 * @brief Measures and prints the time taken by a sorting function.
 * 
 * @param sortFunc Function pointer to the sorting function.
 * @param vec The vector to sort.
 * @param label Description of the method used.
 */
void measureSortTime(void (*sortFunc)(vector<int>&), vector<int> vec, const string& label) {
    auto start = chrono::high_resolution_clock::now();
    sortFunc(vec);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;
    cout << label << " time: " << elapsed.count() << " ms" << endl;
}

/**
 * @brief Main function: generates data, sorts using two methods and prints timing results.
 * 
 * @return int Exit code.
 */
int main() {
    size_t size;
    cout << "Enter the size of the array: ";
    cin >> size;

    vector<int> original = generateRandomVector(size);

    measureSortTime(sequentialSort, original, "Sequential sort");
    measureSortTime(parallelSort, original, "Parallel sort (2 threads)");

    return 0;
}
