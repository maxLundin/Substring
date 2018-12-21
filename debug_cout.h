//
// Created by max on 12/22/18.
//

#pragma once

#include <iostream>

//class debug_cout {
//public:
//    template<typename T>
//    friend debug_cout &operator<<(debug_cout &out, T &elem);
//};
//
//template<typename T>
//debug_cout &operator<<(debug_cout &out, T &elem) {
//#ifdef DEBUG
//    out << elem;
//#endif
//    return out;
//}


template<typename T>
void debug_cout(const T &elem1) {
#ifdef DEBUG
    std::cout << elem1;
#endif
}


template<typename T, typename ...Args>
void debug_cout(const T &elem1, const Args... elems) {
#ifdef DEBUG
    std::cout << elem1;
    debug_cout(elems...);
#endif
}
