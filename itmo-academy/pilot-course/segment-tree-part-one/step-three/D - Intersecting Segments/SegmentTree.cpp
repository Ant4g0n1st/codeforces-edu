#include <type_traits>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <map>

namespace Types
{

    using Long = long long int;
    using Int = unsigned int;

}; // namespace Types

using namespace Types;

namespace DataStructures
{

    template <typename T>
    class SegmentTree
    {
        static_assert(std::is_integral<T>::value);

    public:
        SegmentTree(const Int &l, const Int &r) noexcept : l{l}, r{r} {}

        const T &Update(const Int &i, const T &u) noexcept
        {
            if (Disjoint(i, i))
            {
                return value;
            }
            if (Contained(i, i))
            {
                return value = u;
            }
            return value = left->Update(i, u)
                        + right->Update(i, u);
        }

        T Query(const Int &l, const Int &r) const noexcept
        {
            if (Disjoint(l, r))
            {
                return T{};
            }
            if (Contained(l, r))
            {
                return value;
            }
            return left->Query(l, r)
                + right->Query(l, r);
        }

        const T &Build() noexcept
        {
            auto h{(l + r) / 2};
            if (!this->IsLeaf())
            {
                left.reset(new SegmentTree(l, h));
                right.reset(new SegmentTree(++h, r));
                value = left->Build() + right->Build();
            }
            return value;
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
        T value{};
        Int l{};
        Int r{};
    };

}; // namespace DataStructures

namespace Solution
{

    template <typename T>
    using SegTree = DataStructures::SegmentTree<T>;

    using ValueType = Int;

    template <typename Tree, typename Sequence, typename Bucket>
    void Intersections(Tree &t, const Sequence &sequence, Bucket &b)
    {
        const auto &n{sequence.size()};
        std::map<Int, Int> previous{};
        for (Int i = 0; i < n; i++)
        {
            const auto &k{sequence[i]};
            const auto &&ep{previous.emplace(k, i)};
            if (!ep.second)
            {
                const auto &j{ep.first->second};
                b[k - 1] += t.Query(j + 1, i);
                t.Update(j, 0);
            }
            else
            {
                t.Update(i, 1);
            }
        }
    }

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        std::unique_ptr<SegTree<ValueType>> st{};
        Int m{}, n{}; input >> m, n = 2 * m - 1;
        st.reset(new SegTree<ValueType>{0, n++});
        std::vector<ValueType> intersections(m);
        std::vector<ValueType> sequence(n);
        for (auto &element : sequence)
        {
            input >> element;
        }
        st->Build();
        Intersections(*st, sequence, intersections);
        std::reverse(sequence.begin(), sequence.end());
        Intersections(*st, sequence, intersections);
        for (const auto &count : intersections)
        {
            output << count << ' ';
        }
        output << '\n';
    }

    void OptimizeIOStreams()
    {
        std::ios_base::sync_with_stdio(false);
        std::cout.tie(nullptr);
        std::cin.tie(nullptr);
    }

}; // namespace Solution

int main()
{
    Solution::OptimizeIOStreams();
    Solution::SolveProblem(std::cin, std::cout);
    return 0;
}