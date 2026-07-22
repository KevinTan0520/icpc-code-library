#include <bits/stdc++.h>
using namespace std;

void vector_example() {
    vector<int> v = {1, 2, 3};
    v.push_back(4);
    // emplace_back(args...) constructs an element directly at the end.
    v.emplace_back(5);
    v.pop_back();
    // insert(pos, value) inserts before pos and returns an iterator to the new element.
    v.insert(v.begin() + 1, 10);
    v.erase(v.begin());
    // resize(n, value) changes the size and uses value to fill new positions.
    v.resize(5, 0);
    // reserve(n) increases capacity when needed but does not change size.
    v.reserve(10);
    // at(pos) performs bounds checking, unlike operator[].
    int first = v.front(), last = v.back(), value = v.at(1);
    int n = v.size(), room = v.capacity();
    bool is_empty = v.empty();
    sort(v.begin(), v.end());
    v.clear();
    (void)first, (void)last, (void)value, (void)n, (void)room, (void)is_empty;
}

void deque_example() {
    deque<int> d = {2, 3};
    d.push_front(1);
    d.push_back(4);
    d.emplace_front(0);
    d.emplace_back(5);
    d.pop_front();
    d.pop_back();
    d.insert(d.begin() + 1, 10);
    d.erase(d.begin());
    int first = d.front(), last = d.back(), value = d.at(1);
    int n = d.size();
    bool is_empty = d.empty();
    d.clear();
    (void)first, (void)last, (void)value, (void)n, (void)is_empty;
}

void list_example() {
    list<int> a = {3, 1, 2, 2};
    a.push_front(0);
    a.push_back(4);
    a.pop_front();
    a.pop_back();
    // next(it, n) returns an iterator advanced by n positions.
    list<int>::iterator it = next(a.begin());
    a.insert(it, 5);
    a.erase(it);
    // list::sort sorts nodes without requiring random-access iterators.
    a.sort();
    // unique removes only consecutive duplicate elements.
    a.unique();
    a.reverse();
    list<int> b = {7, 8};
    // splice(pos, other) moves all nodes from other before pos in constant time.
    a.splice(a.end(), b);
    int first = a.front(), last = a.back(), n = a.size();
    bool is_empty = a.empty();
    a.clear();
    (void)first, (void)last, (void)n, (void)is_empty;
}

void string_example() {
    string s = "hello";
    s.push_back('!');
    s.pop_back();
    s.append(" world");
    // insert(pos, text) inserts text before the character at index pos.
    s.insert(5, ",");
    // erase(pos, count) removes count characters starting at pos.
    s.erase(5, 1);
    // replace(pos, count, text) replaces the selected range with text.
    s.replace(0, 5, "Hi");
    // substr(pos, count) returns up to count characters starting at pos.
    string part = s.substr(0, 2);
    // find returns string::npos when the target does not occur.
    size_t pos = s.find("world");
    char first = s.front(), last = s.back(), value = s.at(1);
    int n = s.size();
    bool is_empty = s.empty();
    s.clear();
    (void)part, (void)pos, (void)first, (void)last, (void)value, (void)n, (void)is_empty;
}

void set_example() {
    set<int> s = {1, 3, 5};
    // insert returns the element iterator and whether a new key was inserted.
    pair<set<int>::iterator, bool> inserted = s.insert(2);
    s.emplace(4);
    bool exists = s.find(3) != s.end();
    int occurrences = s.count(3);
    // lower_bound(x) returns the first element not less than x.
    set<int>::iterator lower = s.lower_bound(2);
    // upper_bound(x) returns the first element greater than x.
    set<int>::iterator upper = s.upper_bound(2);
    // equal_range(x) returns the half-open range [lower_bound(x), upper_bound(x)).
    pair<set<int>::iterator, set<int>::iterator> range = s.equal_range(2);
    s.erase(1);
    s.erase(s.begin());
    int n = s.size();
    bool is_empty = s.empty();
    s.clear();
    (void)inserted, (void)exists, (void)occurrences, (void)lower, (void)upper, (void)range, (void)n, (void)is_empty;
}

void map_example() {
    map<string, int> mp;
    // operator[] inserts a missing key with a value-initialized mapped value.
    mp["alice"] = 10;
    // insert returns the element iterator and whether a new key was inserted.
    pair<map<string, int>::iterator, bool> inserted = mp.insert(make_pair("bob", 20));
    mp.emplace("carol", 30);
    // at(key) checks that the key exists and throws out_of_range otherwise.
    int alice = mp.at("alice");
    bool exists = mp.find("bob") != mp.end();
    int occurrences = mp.count("carol");
    map<string, int>::iterator lower = mp.lower_bound("bob");
    map<string, int>::iterator upper = mp.upper_bound("bob");
    mp.erase("alice");
    int n = mp.size();
    bool is_empty = mp.empty();
    mp.clear();
    (void)inserted, (void)alice, (void)exists, (void)occurrences, (void)lower, (void)upper, (void)n, (void)is_empty;
}

void multiset_example() {
    multiset<int> ms = {1, 2, 2, 3};
    ms.insert(2);
    ms.emplace(4);
    int occurrences = ms.count(2);
    multiset<int>::iterator it = ms.find(2);
    // equal_range(x) spans every occurrence of x.
    pair<multiset<int>::iterator, multiset<int>::iterator> range = ms.equal_range(2);
    // erase(iterator) removes one occurrence.
    ms.erase(it);
    // erase(value) removes every occurrence equal to value.
    ms.erase(2);
    int n = ms.size();
    bool is_empty = ms.empty();
    ms.clear();
    (void)occurrences, (void)range, (void)n, (void)is_empty;
}

void stack_example() {
    stack<int> st;
    st.push(1);
    st.emplace(2);
    int value = st.top();
    st.pop();
    int n = st.size();
    bool is_empty = st.empty();
    (void)value, (void)n, (void)is_empty;
}

void queue_example() {
    queue<int> q;
    q.push(1);
    q.emplace(2);
    int first = q.front(), last = q.back();
    q.pop();
    int n = q.size();
    bool is_empty = q.empty();
    (void)first, (void)last, (void)n, (void)is_empty;
}

struct Node {
    int vertex;
    int distance;

    Node(int vertex, int distance) : vertex(vertex), distance(distance) {}
};

struct NodeGreater {
    // Returning true gives lhs lower priority than rhs, so smaller distances and then smaller vertices come first.
    bool operator()(const Node &lhs, const Node &rhs) const {
        if (lhs.distance != rhs.distance) return lhs.distance > rhs.distance;
        return lhs.vertex > rhs.vertex;
    }
};

void priority_queue_example() {
    priority_queue<int> max_heap;
    max_heap.push(2);
    max_heap.emplace(5);
    max_heap.push(1);
    int largest = max_heap.top();
    max_heap.pop();

    // greater<int> makes top() return the smallest element.
    priority_queue<int, vector<int>, greater<int> > min_heap;
    min_heap.push(2);
    min_heap.push(5);
    min_heap.push(1);
    int smallest = min_heap.top();

    // The third template argument applies the custom comparator to Node.
    priority_queue<Node, vector<Node>, NodeGreater> nodes;
    nodes.push(Node{1, 10});
    // emplace(args...) constructs a Node directly inside the priority queue.
    nodes.emplace(2, 5);
    nodes.push(Node{3, 5});
    Node nearest = nodes.top();
    int n = nodes.size();
    bool is_empty = nodes.empty();
    (void)largest, (void)smallest, (void)nearest, (void)n, (void)is_empty;
}

void pair_example() {
    pair<string, int> p("alice", 10);
    pair<string, int> other("bob", 20);
    pair<int, int> point = make_pair(3, 4);
    string name = p.first;
    int score = p.second;
    // Relational operators compare first and then second lexicographically.
    bool ordered_before = point < make_pair(4, 0);
    p.swap(other);
    (void)name, (void)score, (void)ordered_before;
}

void tuple_example() {
    tuple<int, string, double> t = make_tuple(1, "alice", 9.5);
    // get<I>(tuple) accesses the element at compile-time index I.
    int id = get<0>(t);
    string name = get<1>(t);
    get<2>(t) = 10.0;
    double score;
    // tie binds existing variables so assignment can unpack a tuple.
    tie(id, name, score) = t;
    // tuple_size<T>::value gives the number of fields at compile time.
    int fields = tuple_size<decltype(t)>::value;
    (void)id, (void)name, (void)score, (void)fields;
}

void bitset_example() {
    bitset<8> bits(string("00101101"));
    bits.set(7);
    // set(pos, value) assigns one bit to the given Boolean value.
    bits.set(0, false);
    bits.reset(2);
    bits.flip(1);
    // test(pos) reads one bit with bounds checking.
    bool bit = bits.test(3);
    int ones = bits.count();
    bool has_one = bits.any(), all_one = bits.all(), no_one = bits.none();
    // to_ulong converts the bits to an unsigned integer and may throw on overflow.
    unsigned long value = bits.to_ulong();
    string text = bits.to_string();
    (void)bit, (void)ones, (void)has_one, (void)all_one, (void)no_one, (void)value, (void)text;
}

int main() {
    vector_example();
    deque_example();
    list_example();
    string_example();
    set_example();
    map_example();
    multiset_example();
    stack_example();
    queue_example();
    priority_queue_example();
    pair_example();
    tuple_example();
    bitset_example();
    return 0;
}
