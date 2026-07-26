#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/priority_queue.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/trie_policy.hpp>

using namespace std;
namespace pbds = __gnu_pbds;

template <class T>
using ordered_set = pbds::tree<
    T,
    pbds::null_type,
    less<T>,
    pbds::rb_tree_tag,
    pbds::tree_order_statistics_node_update>;

void ordered_tree_example() {
    ordered_set<int> s;
    s.insert(10);
    s.insert(30);
    s.insert(20);

    // find_by_order(k) returns the iterator to the zero-indexed k-th element.
    ordered_set<int>::iterator second = s.find_by_order(1);
    // order_of_key(x) counts elements strictly smaller than x.
    int smaller = s.order_of_key(25);
    bool exists = s.find(20) != s.end();
    s.erase(20);

    // Store a unique id with each value to support duplicates safely.
    ordered_set<pair<int, int> > ms;
    ms.insert(make_pair(5, 0));
    ms.insert(make_pair(5, 1));
    ms.insert(make_pair(8, 2));
    int count_less_than_8 = ms.order_of_key(make_pair(8, INT_MIN));
    int count_at_most_5 = ms.order_of_key(make_pair(5, INT_MAX));
    ms.erase(make_pair(5, 0));

    (void)second, (void)smaller, (void)exists;
    (void)count_less_than_8, (void)count_at_most_5;
}

void hash_table_example() {
    // gp_hash_table uses open addressing and is usually the faster choice.
    pbds::gp_hash_table<string, int> gp;
    gp["alice"] = 10;
    gp.insert(make_pair("bob", 20));
    bool exists = gp.find("alice") != gp.end();
    gp.erase("bob");

    // cc_hash_table resolves collisions with separate chaining.
    pbds::cc_hash_table<int, string> cc;
    cc[1] = "one";
    cc[2] = "two";
    string value = cc[1];
    cc.erase(2);

    (void)exists, (void)value;
}

void priority_queue_example() {
    using Heap = pbds::priority_queue<int, less<int>, pbds::pairing_heap_tag>;

    Heap heap;
    // push returns a stable point iterator used to modify or erase the node.
    Heap::point_iterator handle = heap.push(10);
    heap.push(20);
    heap.modify(handle, 30);
    int largest = heap.top();
    heap.erase(handle);

    Heap other;
    other.push(40);
    other.push(5);
    // join moves all nodes from other into heap and leaves other empty.
    heap.join(other);
    largest = heap.top();

    (void)largest;
}

using prefix_trie = pbds::trie<
    string,
    pbds::null_type,
    pbds::trie_string_access_traits<>,
    pbds::pat_trie_tag,
    pbds::trie_prefix_search_node_update>;

void trie_example() {
    prefix_trie words;
    words.insert("app");
    words.insert("apple");
    words.insert("apply");
    words.insert("banana");

    bool exists = words.find("apple") != words.end();
    // prefix_range(prefix) returns all keys beginning with prefix.
    pair<prefix_trie::iterator, prefix_trie::iterator> range =
        words.prefix_range("app");
    vector<string> matches(range.first, range.second);
    words.erase("app");

    (void)exists, (void)matches;
}

void list_update_example() {
    // Successful finds move frequently accessed keys toward the front.
    pbds::list_update<int, pbds::null_type> keys;
    keys.insert(1);
    keys.insert(2);
    keys.insert(3);
    bool exists = keys.find(2) != keys.end();
    keys.erase(1);
    int n = keys.size();
    keys.clear();

    (void)exists, (void)n;
}

int main() {
    ordered_tree_example();
    hash_table_example();
    priority_queue_example();
    trie_example();
    list_update_example();
    return 0;
}
