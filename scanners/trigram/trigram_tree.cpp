//
// Created by max on 12/18/18.
//

#include "trigram_tree.h"


trigram_tree::trigram_tree() : m_size(0), bitmap() {}


size_t trigram_tree::size() {
    return m_size;
}