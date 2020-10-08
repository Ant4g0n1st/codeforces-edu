#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace Strings
{

    class DeletableString
    {
    public:
        DeletableString(const std::string &s, const std::vector<int> &order)
            : modified{s}, order{order}, original{s} {}

        void Restore(const int &index)
        {
            for (int i = 0; i < index; ++i)
            {
                modified[order[i]] = original[order[i]];
            }
        }

        bool HasSubsequence(const std::string &s)
        {
            const int n = modified.size();
            const int m = s.size();
            int i = 0, j = 0;
            for (; i < n && j < m; ++i)
            {
                if (modified[i] != Sentinel)
                {
                    if (modified[i] == s[j])
                    {
                        j = j + 1;
                    }
                }
            }
            return j == m;
        }

        void DeleteUntil(const int &index)
        {
            for (int i = 0; i < index; ++i)
            {
                modified[order[i]] = Sentinel;
            }
        }

    private:
        static const char Sentinel{std::numeric_limits<char>::max()};

        std::vector<int> order{};
        std::string modified{};
        std::string original{};
    };

}; // namespace Strings

namespace Solution
{

    template <typename T, typename Function>
    T findLastSolution(const T &min, const T &max, Function &&f)
    {
        auto begin{min};
        auto end{max};
        T solution{};
        while (begin <= end)
        {
            auto middle{(begin + end) / 2};
            if (f(middle))
            {
                begin = middle + 1;
                solution = middle;
            }
            else
            {
                end = middle - 1;
            }
        }
        return solution;
    }

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        std::string t{}; input >> t;
        std::string p{}; input >> p;
        std::vector<int> order{};
        const int n = t.size();
        order.resize(n);
        for (auto &element : order)
        {
            input >> element;
            --element;
        }
        Strings::DeletableString text{t, order};
        auto &&f = [&p, &text](const int &index) {
            text.DeleteUntil(index);
            bool found{text.HasSubsequence(p)};
            text.Restore(index);
            return found;
        };
        auto x{findLastSolution(1, n, f)};
        output << x << '\n';
    }

    void OptimizeIOStreams()
    {
        std::ios::sync_with_stdio(false);
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