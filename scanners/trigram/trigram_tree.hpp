//
// Created by max on 12/18/18.
//

template<typename ForwardIterator>
void trigram_tree::add_to_tree(ForwardIterator iterator) {
    trigram tri(iterator);
    auto x = tri.hash();
    if (bitmap[x] == 0) {
        ++m_size;
        bitmap[x] = true;
    }
}
