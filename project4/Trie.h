#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
        
    struct Node {
        Node() {
            m_label = 0;
        }
        
        char m_label;
        std::vector<ValueType> m_values;
        std::vector<Node*> m_children;
    };
    
    void freeTrie(Node* cur) {
        if (cur == NULL)
            return;
        for (int i = 0; i < cur->m_children.size(); i++) {
            freeTrie(cur->m_children[i]);
        }
        delete cur;
    }
    
    //traverse through every path and find matches
    //error if original key inputted is empty
    std::vector<ValueType> findMatches(Node* start, std::string& key, bool exactMatchOnly) const {
        std::vector<ValueType> result;
        if (key.size() == 0) {
            for (int i = 0; i < start->m_values.size(); i++) {
                result.push_back(start->m_values[i]);
            }
            return result;
        }
        std::string smallerKey = key.substr(1);
        for (int k = 0; k < start->m_children.size(); k++) {
            if (key[0] == start->m_children[k]->m_label) {
                std::vector<ValueType> matches = findMatches(start->m_children[k], smallerKey, exactMatchOnly);
                for (int j = 0; j < matches.size(); j++) {
                    result.push_back(matches[j]);
                }
            }
            else {
                if (exactMatchOnly == false) {
                    std::vector<ValueType> matches = findMatches(start->m_children[k], smallerKey, true);
                    for (int j = 0; j < matches.size(); j++) {
                        result.push_back(matches[j]);
                    }
                }
            }
        }
        return result;
    }
    
    Node* m_root;
};

template <typename ValueType>
Trie<ValueType>::Trie() {
    m_root = new Node();
    m_root->m_label = 0;
}

template <typename ValueType>
Trie<ValueType>::~Trie() {
    freeTrie(m_root);
}

template <typename ValueType>
void Trie<ValueType>::reset() {
    freeTrie(m_root);
    m_root = new Node();
}

template <typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value) {
    Node* cur = m_root;
    int i = 0;
    while (i < key.size()) {
        bool charFound = false;
        for (int k = 0; k < cur->m_children.size(); k++) {
            if (key[i] == cur->m_children[k]->m_label) {
                cur = cur->m_children[k];
                charFound = true;
                break;
            }
        }
        if (charFound == false) { //create a new node if path doesn't already exist
            Node* newNode = new Node();
            newNode->m_label = key[i];
            cur->m_children.push_back(newNode);
            cur = cur->m_children[cur->m_children.size() - 1];
        }
        i++;
    }
    //mark current node as leaf
    cur->m_values.push_back(value);
}

//run-time should be O(L*C)
//try to add recursion if have time
template <typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const {
    std::vector<ValueType> result;
    char begin = key[0];
    Node* start = nullptr;
    for (int i = 0; i < m_root->m_children.size(); i++) {
        if (begin == m_root->m_children[i]->m_label)
            start = m_root->m_children[i];
    }
    if (start != nullptr) {
        std::string restOfKey = key.substr(1);
        return findMatches(start, restOfKey, exactMatchOnly);
    }
    return result;
}


#endif // TRIE_INCLUDED
