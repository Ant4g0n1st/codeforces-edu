#include <type_traits>
#include <iostream>
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

    using ValueType = Long;

    enum class Operation : Int
    {
        ChangeElement = 1,
        QuerySum
    };

    std::istream &operator>>(std::istream &input, Operation &operation)
    {
        Int buffer{}; input >> buffer;
        operation = static_cast<Operation>(buffer);
        return input;
    }

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        std::unique_ptr<SegTree<ValueType>> st{};
        Int m{}, n{}; input >> n >> m;
        st.reset(new SegTree<ValueType>{0, --n});
        std::vector<ValueType> initial(++n);
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
            case Operation::ChangeElement:
            {
                Int index{}; input >> index;
                ValueType v{}; input >> v;
                st->Update(index, v);
            }
            break;
            case Operation::QuerySum:
            {
                Int l{}; input >> l;
                Int r{}; input >> r;
                output << st->Query(l, --r);
                output << '\n';
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