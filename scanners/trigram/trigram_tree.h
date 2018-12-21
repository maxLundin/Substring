//
// Created by max on 14.12.18.
//

#pragma once

#include "trigram.h"
#include <bitset>

class trigram_tree {
public:
    trigram_tree();

    size_t size();

    template<typename ForwardIterator>
    void add_to_tree(ForwardIterator iterator);

private:
    size_t m_size;
    std::bitset<16777216> bitmap;
};


#include "trigram_tree.hpp"