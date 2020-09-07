#include <type_traits>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

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

        template <typename Sequence>
        const T &Build(const Sequence &initial) noexcept
        {
            auto h{(l + r) / 2};
            if (!this->IsLeaf())
            {
                left.reset(new SegmentTree(l, h++));
                right.reset(new SegmentTree(h, r));
                value = left->Build(initial)
                    + right->Build(initial);
            }
            else
            {
                value = initial[h];
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

    template <typename T>
    class MaximumSegment
    {
    public:
        MaximumSegment(const T &v) noexcept : max{std::max(v, T{})}, preffix{v}, suffix{v}, sum{v} {}

        MaximumSegment() = default;

        MaximumSegment operator+(const MaximumSegment &other) const noexcept
        {
            MaximumSegment result{};
            auto &m{result.max};
            result.suffix = std::max(other.suffix, other.sum + suffix);
            result.preffix = std::max(preffix, sum + other.preffix);
            m = std::max(m, suffix + other.preffix);
            m = std::max(m, result.preffix);
            m = std::max(m, result.suffix);
            result.sum = sum + other.sum;
            m = std::max(m, other.max);
            m = std::max(m, max);
            return result;
        }

        operator const T &() const noexcept
        {
            return max;
        }

    private:
        T preffix{};
        T suffix{};
        T max{};
        T sum{};
    };

}; // namespace DataStructures

namespace Solution
{

    template <typename T>
    using SegTree = DataStructures::SegmentTree<T>;

    using Integral = Long;

    using ValueType = DataStructures::MaximumSegment<Integral>;

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        std::unique_ptr<SegTree<ValueType>> st{};
        Int m{}, n{}; input >> n >> m;
        st.reset(new SegTree<ValueType>{0, --n});
        std::vector<Integral> initial(++n);
        for (auto &value : initial)
        {
            input >> value;
        }
        std::cout << st->Build(initial) << '\n';
        for (Int i = 0; i < m; i++)
        {
            Int index{}; input >> index;
            Integral v{}; input >> v;
            std::cout << st->Update(index, v);
            std::cout << '\n';
        }
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