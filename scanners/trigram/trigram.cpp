#include <fstream>
#include <set>
#include "trigram.h"

trigram::trigram() = default;

char trigram::operator[](size_t pos) {
    return chars[pos];
}

char const trigram::operator[](size_t pos) const {
    return chars[pos];
}

