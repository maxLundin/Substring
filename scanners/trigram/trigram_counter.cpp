//
// Created by max on 14.12.18.
//

#include "debug_cout.h"
#include "trigram_counter.h"
#include "trigram_tree.h"
#include <fstream>
#include <set>
#include <iostream>
#include <QDebug>


const size_t buffer_length = 128000;
const int64_t magic_constant_is_binary = 20000;

inline void count(char *buffer, int64_t size, trigram_tree &tree) {
    for (int64_t i = 0; i < size; i++) {
        tree.add_to_tree(buffer + i);
    }
}

int64_t trigram_counter::count_trigrams(QFileInfo &file, int64_t max_size) {
    if (file.exists() && file.isReadable()) {
        char *char_buffer = new char[buffer_length + 2];
        trigram_tree tree;
        std::ifstream file_input(file.absoluteFilePath().toStdString(), std::ios::binary);
        if (file_input.is_open()) {
            file_input.read(char_buffer, buffer_length);
            if (file_input.gcount() > 2) {
                count(char_buffer, file_input.gcount() - 2, tree);
            } else {
                delete[] char_buffer;
                return 0;
            }
            while (!file_input.eof()) {
                char_buffer[0] = char_buffer[buffer_length];
                char_buffer[1] = char_buffer[buffer_length + 1];
                file_input.read(char_buffer + 2, buffer_length);
                if (file_input.gcount() == 0) {
                    break;
                }
                count(char_buffer, file_input.gcount(), tree);
                if (tree.size() > (size_t) max_size) {
                    delete[] char_buffer;
                    return tree.size();
                }
            }
            file_input.close();
        } else {
            delete[] char_buffer;
            return -1;
        }
        delete[] char_buffer;
        return tree.size();
    } else {
        return -1;
    }
}

bool trigram_counter::is_binary(QFileInfo &file) {
    auto trigam_count = count_trigrams(file, magic_constant_is_binary);
    debug_cout(file.absoluteFilePath().toStdString(), " ", trigam_count, '\n');
    if (trigam_count != -1)
        return trigam_count > magic_constant_is_binary;
    else
        return true;
}
