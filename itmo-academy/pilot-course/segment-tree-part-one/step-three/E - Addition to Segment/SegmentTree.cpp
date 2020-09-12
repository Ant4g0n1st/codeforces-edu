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

        void Update(const Int &l, const Int &r, const T &u) noexcept
        {
            Update(l, u), Update(r + 1, -u);
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

        T Query(const Int &i) const noexcept
        {
            return Query(0, i);
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

        const T &Update(const Int &i, const T &u) noexcept
        {
            if (Disjoint(i, i))
            {
                return value;
            }
            if (Contained(i, i))
            {
                return value += u;
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
        AddToSegment = 1,
        QueryPosition
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
        st.reset(new SegTree<ValueType>{0, n});
        st->Build();
        for (Int i = 0; i < m; i++)
        {
            Operation operation{};
            input >> operation;
            switch (operation)
            {
            case Operation::QueryPosition:
            {
                Int index{}; input >> index;
                output << st->Query(index);
                output << '\n';
            }
            break;
            case Operation::AddToSegment:
            {
                Int l{}; input >> l;
                Int r{}; input >> r;
                ValueType v{}; input >> v;
                st->Update(l, --r, v);
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