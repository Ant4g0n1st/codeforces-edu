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

        Int KthOne(const T &k) const noexcept
        {
            if (!this->IsLeaf())
            {
                if (right->value <= k)
                {
                    return left->KthOne(k - right->value);
                }
                return right->KthOne(k);
            }
            return l;
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

}; // namespace DataStructures

namespace Solution
{

    template <typename T>
    using SegTree = DataStructures::SegmentTree<T>;

    using Integral = Int;

    using ValueType = Integral;

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        std::unique_ptr<SegTree<ValueType>> st{};
        Int n{}; input >> n;
        st.reset(new SegTree<ValueType>{0, --n});
        std::vector<Integral> sequence(++n, 1);
        std::vector<Integral> inversions(n);
        for (int i = 0; i < n; i++)
        {
            input >> inversions[i];
        }
        st->Build(sequence);
        for (int i = n - 1; i >= 0; i--)
        {
            sequence[i] = st->KthOne(inversions[i]);
            st->Update(sequence[i]++, 0);
        }
        for (const auto &element : sequence)
        {
            output << element << ' ';
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