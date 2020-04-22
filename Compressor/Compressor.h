#pragma once
#include <cstddef>
#include <functional>
#include <string>

struct Key
{
    char* array;
    int size;
    bool operator==(const Key& other) const
    {
        for (int i = 0; i < size; i++) {
            if (array[i] != other.array[i])
            {
                return 0;
            }
        }
        return 1;
    }
};

template<typename T>
struct hash
{

};

namespace std
{
    template<>
    struct hash<Key>
    {
        size_t operator()(const Key& k) const
        {
            size_t result = 0;
            const size_t prime = 31;
            for (size_t i = 0; i < k.size; i++)
            {
                result = k.array[i] + (result * prime);
            }
            return result;
        }
    };
}