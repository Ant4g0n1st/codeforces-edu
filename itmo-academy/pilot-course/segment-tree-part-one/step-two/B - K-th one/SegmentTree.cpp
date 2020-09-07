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

        Int KthOne(const T &k) const noexcept
        {
            if (!this->IsLeaf())
            {
                if (left->value <= k)
                {
                    return right->KthOne(k - left->value);
                }
                return left->KthOne(k);
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

        const T &Flip(const Int &i) noexcept
        {
            if (Disjoint(i, i))
            {
                return value;
            }
            if (Contained(i, i))
            {
                return value = !value;
            }
            return value = left->Flip(i)
                        + right->Flip(i);
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

    enum class Operation : Int
    {
        FlipPosition = 1,
        FindKthOne
    };

    std::istream &operator>>(std::istream &inputStream, Operation &op)
    {
        Int buffer{}; inputStream >> buffer;
        op = static_cast<Operation>(buffer);
        return inputStream;
    }

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
        st->Build(initial);
        for (Int i = 0; i < m; i++)
        {
            Operation operation;
            input >> operation;
            switch (operation)
            {
            case Operation::FlipPosition:
            {
                Int index{}; input >> index;
                st->Flip(index);
            }
            break;
            case Operation::FindKthOne:
            {
                ValueType k{}; input >> k;
                std::cout << st->KthOne(k);
                std::cout << '\n';
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

}; // namespace Solution

int main()
{
    Solution::OptimizeIOStreams();
    Solution::SolveProblem(std::cin, std::cout);
    return 0;
}