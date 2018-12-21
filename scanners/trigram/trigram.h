#pragma once

#include <cstdio>
#include <iterator>
#include <QFile>

class trigram {
public:
    static constexpr size_t size = 3;

    trigram();

    template<class ForwardIterator>
    explicit trigram(ForwardIterator iterator) {
        for (char &i : chars) {
            i = *(iterator);
            ++iterator;
        }
    }

    uint64_t hash() {
        uint64_t hash = 0;
        for (char i : chars) {
            hash = hash * 256 + (i - CHAR_MIN);
        }
        return hash;
    }

    char operator[](size_t pos);

    char const operator[](size_t pos) const;


private:
    char chars[size];
};

