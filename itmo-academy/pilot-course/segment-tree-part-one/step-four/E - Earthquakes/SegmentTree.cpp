#include <type_traits>
#include <functional>
#include <iostream>
#include <chrono>
#include <memory>
#include <random>

namespace Types
{

    using Long = long long int;
    using Int = unsigned int;

}; // namespace Types

using namespace Types;

namespace Random
{

    template <typename T>
    T GetRandom()
    {
        const static Int seed = std::chrono::system_clock::now().time_since_epoch().count();
        static std::uniform_int_distribution<T> distribution{};
        static std::default_random_engine generator{seed};
        static auto random{std::bind(distribution, generator)};
        return random();
    }

}; // namespace Random

namespace DataStructures
{

    template <typename K, typename V>
    struct Treap
    {
        static_assert(std::is_integral<K>::value);
        static_assert(std::is_integral<V>::value);
        static_assert(std::is_signed<V>::value);

    private:
        struct Node
        {
            using pNode = Node *;

            Node(const K &, const V &) noexcept;

            static void AddValue(pNode, const V &) noexcept;

            static void ApplyPending(pNode) noexcept;

            static void EraseSubtree(pNode) noexcept;

            static void Update(pNode) noexcept;

            static V Total(pNode) noexcept;

            ~Node() noexcept;

            Long priority{};
            bool erase{};
            pNode l{};
            pNode r{};
            V total{};
            V value{};
            K key{};
        };

        using pNode = Node *;

    private:
        static void Split(pNode, const K &, pNode &, pNode &) noexcept;

        static bool AddToKey(pNode, const K &, const V &) noexcept;

        static V EraseNotGreater(pNode &, const K &) noexcept;

        static void Merge(pNode &, pNode, pNode) noexcept;

        static void Insert(pNode &, pNode) noexcept;

        static K FindByOrder(pNode, int) noexcept;

    public:
        V EraseNotGreater(const K &) noexcept;

        K FindByOrder(const int &) noexcept;

        void Delete(const K &) noexcept;

        void Insert(const K &) noexcept;

        K FindFirst() noexcept;

        V Size() noexcept;

        ~Treap() noexcept;

    private:
        pNode root{};
    };

    template <typename K, typename V>
    class SegmentTree
    {
    public:
        SegmentTree(const Int &l, const Int &r) noexcept : l{l}, r{r} {}

        K Update(const Int &i, const K &u) noexcept
        {
            if (Disjoint(i, i))
            {
                return K{};
            }
            if (Contained(i, i))
            {
                K previous{treap.FindFirst()};
                treap.Delete(previous);
                treap.Insert(u);
                return previous;
            }
            K erase{right->Update(i, u)};
            erase |= left->Update(i, u);
            treap.Delete(erase);
            treap.Insert(u);
            return erase;
        }

        V Query(const Int &l, const Int &r, const K &q) noexcept
        {
            if (Disjoint(l, r))
            {
                return V{};
            }
            if (Contained(l, r))
            {
                return treap.EraseNotGreater(q);
            }
            return left->Query(l, r, q) + right->Query(l, r, q);
        }

        void Build() noexcept
        {
            auto h{(l + r) / 2};
            if (!this->IsLeaf())
            {
                left.reset(new SegmentTree(l, h++));
                right.reset(new SegmentTree(h, r));
                left->Build(), right->Build();
            }
        }

    private:
        bool Contained(const Int &l, const Int &r) const noexcept
        {
            return l <= this->l && this->r <= r;
        }

        bool Disjoint(const Int &l, const Int &r) const noexcept
        {
            return r < this->l || this->r < l;
        }

        bool IsLeaf() const noexcept
        {
            return l == r;
        }

    private:
        std::unique_ptr<SegmentTree> right{};
        std::unique_ptr<SegmentTree> left{};
        Treap<K, V> treap{};
        Int l{};
        Int r{};
    };

}; // namespace DataStructures

namespace Solution
{

    template <typename K, typename V>
    using SegTree = DataStructures::SegmentTree<K, V>;

    using Value = int;

    using Key = Int;

    enum class Operation : Int
    {
        ConstructBuilding = 1,
        Earthquake
    };

    std::istream &operator>>(std::istream &inputStream, Operation &op)
    {
        Int buffer{};
        inputStream >> buffer;
        op = static_cast<Operation>(buffer);
        return inputStream;
    }

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        std::unique_ptr<SegTree<Key, Value>> st{};
        Int m{}, n{};
        input >> n >> m;
        st.reset(new SegTree<Key, Value>{0, --n});
        st->Build();
        for (Int i = 0; i < m; ++i)
        {
            Operation operation{};
            input >> operation;
            switch (operation)
            {
            case Operation::Earthquake:
            {
                Int l{};
                input >> l;
                Int r{};
                input >> r;
                Key p{};
                input >> p;
                output << st->Query(l, --r, p);
                output << '\n';
            }
            break;
            case Operation::ConstructBuilding:
            {
                Int i{};
                input >> i;
                Key h{};
                input >> h;
                st->Update(i, h);
            }
            break;
            }
        }
    }

    void OptimizeIOStreams()
    {
        std::ios_base::sync_with_stdio(false);
        std::cout.tie(nullptr);
        std::cin.tie(nullptr);
    }

    // void TestTreap(std::istream &input, std::ostream &output)
    // {
    //     Treap<Key, Value> treap{};
    //     int n{};
    //     input >> n;
    //     for (int i = 0; i < n; ++i)
    //     {
    //         int option{};
    //         input >> option;
    //         Value v{};
    //         input >> v;
    //         switch (option)
    //         {
    //         case 1: // Insert
    //         {
    //             treap.Insert(v);
    //             output << "Size after inserting " << v << " : " << treap.Size() << std::endl;
    //         }
    //         break;
    //         case 2: // Delete
    //         {
    //             treap.Delete(v);
    //             output << "Size after deleting " << v << " : " << treap.Size() << std::endl;
    //         }
    //         break;
    //         case 3: // Erase Smaller
    //         {
    //             output << treap.EraseNotGreater(v) << " values not greater than " << v << std::endl;
    //             output << "\tSize after query : " << treap.Size() << std::endl;
    //         }
    //         break;
    //         case 4: // Kth Key
    //         {
    //             output << "The key in place " << v << " is : " << treap.FindByOrder(v) << std::endl;
    //         }
    //         break;
    //         default:
    //             output << "WHAT?\n";
    //             break;
    //         }
    //     }
    // }

}; // namespace Solution

#include <fstream>

int main()
{
    // Solution::OptimizeIOStreams();
    // Solution::SolveProblem(std::cin, std::cout);
    std::ifstream input{R"(itmo-academy\pilot-course\segment-tree-part-one\step-four\E - Earthquakes\input.in)"};
    std::ofstream output{R"(itmo-academy\pilot-course\segment-tree-part-one\step-four\E - Earthquakes\output.out)"};
    Solution::SolveProblem(input, output);
    // Solution::TestTreap(std::cin, std::cout);
    return 0;
}

namespace DataStructures
{

    /* Treap Node Implementation. */
    template <typename K, typename V>
    Treap<K, V>::Node::Node(const K &key, const V &value) noexcept
        : key{key}, priority{Random::GetRandom<Long>()}, total{value}, value{value} {}

    template <typename K, typename V>
    void Treap<K, V>::Node::AddValue(pNode node, const V &value) noexcept
    {
        if (node)
        {
            node->value += value;
        }
    }

    template <typename K, typename V>
    void Treap<K, V>::Node::ApplyPending(pNode node) noexcept
    {
        if (node && node->erase)
        {
            EraseSubtree(node->l);
            EraseSubtree(node->r);
            node->erase = false;
            node->value = V{};
            node->total = V{};
        }
    }

    template <typename K, typename V>
    void Treap<K, V>::Node::EraseSubtree(pNode node) noexcept
    {
        if (node)
        {
            node->erase = true;
        }
    }

    template <typename K, typename V>
    void Treap<K, V>::Node::Update(pNode node) noexcept
    {
        if (node)
        {
            auto &total{node->total};
            total = node->value;
            total += Total(node->l);
            total += Total(node->r);
        }
    }

    template <typename K, typename V>
    V Treap<K, V>::Node::Total(pNode node) noexcept
    {
        if (node)
        {
            return node->erase ? V{} : node->total;
        }
        return V{};
    }

    template <typename K, typename V>
    Treap<K, V>::Node::~Node() noexcept
    {
        if (l)
        {
            delete l;
        }
        if (r)
        {
            delete r;
        }
    }

    /* Treap Implementation. */
    template <typename K, typename V>
    bool Treap<K, V>::AddToKey(pNode node, const K &key, const V &value) noexcept
    {
        Node::ApplyPending(node);
        bool keyFound{false};
        if (node)
        {
            if (key < node->key)
            {
                keyFound = AddToKey(node->l, key, value);
            }
            else if (key > node->key)
            {
                keyFound = AddToKey(node->r, key, value);
            }
            else
            {
                Node::AddValue(node, value);
                keyFound = true;
            }
        }
        Node::Update(node);
        return keyFound;
    }

    template <typename K, typename V>
    void Treap<K, V>::Split(pNode node, const K &key, pNode &l, pNode &r) noexcept
    {
        Node::ApplyPending(node);
        if (node)
        {
            if (key < node->key)
            {
                Split(node->l, key, l, node->l);
                r = node;
            }
            else
            {
                Split(node->r, key, node->r, r);
                l = node;
            }
        }
        else
        {
            l = r = nullptr;
        }
        Node::Update(node);
    }

    template <typename K, typename V>
    void Treap<K, V>::Merge(pNode &node, pNode x, pNode y) noexcept
    {
        Node::ApplyPending(x);
        Node::ApplyPending(y);
        if (!x || !y)
        {
            node = x ? x : y;
        }
        else if (x->priority > y->priority)
        {
            Merge(x->r, x->r, y);
            node = x;
        }
        else
        {
            Merge(y->l, x, y->l);
            node = y;
        }
        Node::Update(node);
    }

    template <typename K, typename V>
    V Treap<K, V>::EraseNotGreater(pNode &node, const K &key) noexcept
    {
        if (node)
        {
            pNode left{}, right{};
            Split(node, key, left, right);
            auto total{Node::Total(left)};
            Node::EraseSubtree(left);
            Merge(node, left, right);
            return total;
        }
        return V{};
    }

    template <typename K, typename V>
    void Treap<K, V>::Insert(pNode &node, pNode n) noexcept
    {
        Node::ApplyPending(node);
        if (node)
        {
            if (n->priority > node->priority)
            {
                Split(node, n->key, n->l, n->r);
                node = n;
            }
            else if (n->key < node->key)
            {
                Insert(node->l, n);
            }
            else
            {
                Insert(node->r, n);
            }
        }
        else
        {
            node = n;
        }
        Node::Update(node);
    }

    template <typename K, typename V>
    K Treap<K, V>::FindByOrder(pNode node, int k) noexcept
    {
        Node::ApplyPending(node);
        if (node)
        {
            const auto &&total{Node::Total(node->l)};
            if (k <= total)
            {
                return FindByOrder(node->l, k);
            }
            k = k - total;
            if (k <= node->value)
            {
                return node->key;
            }
            k = k - node->value;
            return FindByOrder(node->r, k);
        }
        return K{};
    }

    template <typename K, typename V>
    void Treap<K, V>::Insert(const K &key) noexcept
    {
        if (!Treap::AddToKey(root, key, 1))
        {
            Treap::Insert(root, new Node{key, 1});
        }
    }

    template <typename K, typename V>
    V Treap<K, V>::EraseNotGreater(const K &key) noexcept
    {
        return Treap::EraseNotGreater(root, key);
    }

    template <typename K, typename V>
    void Treap<K, V>::Delete(const K &key) noexcept
    {
        Treap::AddToKey(root, key, -1);
    }

    template <typename K, typename V>
    K Treap<K, V>::FindByOrder(const int &k) noexcept
    {
        return Treap::FindByOrder(root, k);
    }

    template <typename K, typename V>
    K Treap<K, V>::FindFirst() noexcept
    {
        return Treap::FindByOrder(1);
    }

    template <typename K, typename V>
    V Treap<K, V>::Size() noexcept
    {
        return Node::Total(root);
    }

    template <typename K, typename V>
    Treap<K, V>::~Treap() noexcept
    {
        if (root)
        {
            delete root;
        }
    }

}; // namespace DataStructures