//
// Created by max on 14.12.18.
//

#pragma once

#include <cstdio>
#include <QFileInfo>

class trigram_counter {
public:

    static int64_t count_trigrams(QFileInfo &file, int64_t max_size = INT64_MAX);

    static bool is_binary(QFileInfo &file);
};
