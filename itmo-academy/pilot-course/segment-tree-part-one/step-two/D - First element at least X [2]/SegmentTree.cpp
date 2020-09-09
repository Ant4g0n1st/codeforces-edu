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
        static constexpr Int NPos{std::numeric_limits<Int>::max()};

    public:
        SegmentTree(const Int &l, const Int &r) noexcept : l{l}, r{r} {}

        Int FirstAtLeast(const Int &i, const T &x) const noexcept
        {
            if (IsBefore(i) || value < x)
            {
                return SegmentTree::NPos;
            }
            if (!this->IsLeaf())
            {
                auto index{left->FirstAtLeast(i, x)};
                if (index == SegmentTree::NPos)
                {
                    return right->FirstAtLeast(i, x);
                }
                return index;
            }
            return l;
        }

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

        bool Contains(const Int &i) const noexcept
        {
            return this->l <= i && i <= this->r;
        }

        bool IsBefore(const Int &i) const noexcept
        {
            return !(Contains(i) || i < l);
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
    class Maximum
    {
        static_assert(std::is_integral<T>::value);

    public:
        Maximum(const T &value) noexcept : value{value} {}

        Maximum() = default;

        Maximum operator+(const Maximum &other) const noexcept
        {
            return {std::max(value, other.value)};
        }

        operator const T &() const noexcept
        {
            return value;
        }

    private:
        static constexpr T Neutral{std::numeric_limits<T>::min()};

        T value{Neutral};
    };

}; // namespace DataStructures

namespace Solution
{

    template <typename T>
    using SegTree = DataStructures::SegmentTree<T>;

    using Integral = Int;

    using ValueType = DataStructures::Maximum<Integral>;

    enum class Operation : Int
    {
        ChangeElement = 1,
        FindGreaterOrEqual
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
            Operation operation{};
            input >> operation;
            switch (operation)
            {
            case Operation::FindGreaterOrEqual:
            {
                Integral v{}; input >> v;
                Int l{}; input >> l;
                auto index{st->FirstAtLeast(l, v)};
                if (index != st->NPos)
                {
                    output << index << '\n';
                    continue;
                }
                output << "-1\n";
            }
            break;
            case Operation::ChangeElement:
            {
                Int index{}; input >> index;
                Integral v{}; input >> v;
                st->Update(index, v);
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