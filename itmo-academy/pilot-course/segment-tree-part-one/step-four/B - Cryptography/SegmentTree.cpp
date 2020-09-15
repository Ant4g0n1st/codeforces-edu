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
    public:
        SegmentTree(const Int &l, const Int &r) noexcept : l{l}, r{r} {}

        T Query(const Int &l, const Int &r) const noexcept
        {
            if (Disjoint(l, r))
            {
                return T::Identity();
            }
            if (Contained(l, r))
            {
                return value;
            }
            return left->Query(l, r)
                * right->Query(l, r);
        }

        template <typename Sequence>
        T Build(const Sequence &initial) noexcept
        {
            auto h{(l + r) / 2};
            if (!this->IsLeaf())
            {
                left.reset(new SegmentTree(l, h++));
                right.reset(new SegmentTree(h, r));
                value = left->Build(initial)
                    * right->Build(initial);
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

namespace Math
{

    template <typename T, Int N>
    class SquareMatrix
    {
    public:
        SquareMatrix() = default;

        SquareMatrix operator*(const SquareMatrix &other) noexcept
        {
            SquareMatrix result{};
            for (Int i = 0; i < N; ++i)
            {
                for (Int j = 0; j < N; ++j)
                {
                    for (Int k = 0; k < N; ++k)
                    {
                        result[i][j] += matrix[i][k] * other.matrix[k][j];
                    }
                }
            }
            return result;
        }

        static SquareMatrix Identity() noexcept
        {
            SquareMatrix matrix{};
            for (Int i = 0; i < N; ++i)
            {
                matrix[i][i] = 1;
            }
            return matrix;
        }

        T *operator[](const Int &i) noexcept
        {
            return matrix[i];
        }

    private:
        T matrix[N][N]{};
    };

    template <typename T>
    class ModInt
    {
        static_assert(std::is_integral<T>::value);

    public:
        ModInt(const T &value) : value{value % mod} {}

        ModInt() = default;

        template <typename X>
        friend std::istream &operator>>(std::istream &, ModInt &);

        ModInt operator*(const ModInt &other) const noexcept
        {
            return {value * other.value};
        }

        ModInt &operator+=(const ModInt &other) noexcept
        {
            return *this = {value + other.value};
        }

        static void SetModulo(const T &mod) noexcept
        {
            ModInt::mod = mod;
        }

        operator const T &() const noexcept
        {
            return value;
        }

    private:
        static T mod;

        T value{};
    };

    template <typename T>
    std::istream &operator>>(std::istream &stream, ModInt<T> &mi)
    {
        T buffer{}; stream >> buffer;
        mi = ModInt<T>(buffer);
        return stream;
    }

    template <typename T>
    T ModInt<T>::mod{};

}; // namespace Math

namespace Solution
{

    namespace Constants
    {

        constexpr Int D = 2;

    };

    using Matrix = Math::SquareMatrix<Math::ModInt<Int>, Constants::D>;

    template <typename T>
    using SegTree = DataStructures::SegmentTree<T>;

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        Int m{}, n{}, r{}; input >> r >> n >> m;
        Math::ModInt<Int>::SetModulo(r);
        std::unique_ptr<SegTree<Matrix>> st{};
        st.reset(new SegTree<Matrix>{0, --n});
        std::vector<Matrix> initial(++n);
        for (auto &matrix : initial)
        {
            for (Int i = 0; i < Constants::D; ++i)
            {
                for (Int j = 0; j < Constants::D; ++j)
                {
                    input >> matrix[i][j];
                }
            }
        }
        st->Build(initial);
        for (Int i = 0; i < m; ++i)
        {
            Int l{}; input >> l, --l;
            Int r{}; input >> r, --r;
            auto &&matrix{st->Query(l, r)};
            for (Int i = 0; i < Constants::D; ++i)
            {
                for (Int j = 0; j < Constants::D; ++j)
                {
                    output << matrix[i][j] << ' ';
                }
                output << '\n';
            }
            output << '\n';
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