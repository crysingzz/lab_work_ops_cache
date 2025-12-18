#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>
#include <filesystem>
#include <queue>
#include <algorithm>

template<typename T>
std::vector<T> KwayMergeSort(const std::vector<T>& array
    , std::size_t k = 4
    , std::size_t block_size = 8)
{
    if (array.empty() || k == 0) {
        return std::vector<T>();
    }

    std::vector<std::vector<T>> chunks(k);
    std::size_t n = array.size();

    for (std::size_t i = 0; i < n; i++) {
        std::size_t chunk_idx = i / block_size;
        if (chunk_idx >= k) {
            chunk_idx = k - 1;
        }
        
        chunks[chunk_idx].push_back(array[i]);
    }
    for (auto& chunk : chunks) {
        std::sort(chunk.begin(), chunk.end());
    }
    struct Node {
        T* ptr;
        std::size_t chunk_id;
        std::size_t index_in_chunk;

        bool operator>(const Node& other) const {
            return *ptr > *other.ptr;
        }
    };
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> min_heap;

    for (std::size_t i = 0; i < k; i++) {
        if (!chunks[i].empty()) {
            min_heap.push({&chunks[i][0], i, 0});
        }
    }

    std::vector<T> result;
    result.reserve(n);

    while (!min_heap.empty()) {
        auto top = min_heap.top();
        min_heap.pop();

        result.push_back(*top.ptr);
        if (top.index_in_chunk + 1 < chunks[top.chunk_id].size()) {
            top.ptr++;
            top.index_in_chunk++;
            min_heap.push(top);
        }
    }

    return result;
}

