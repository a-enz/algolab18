#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

int edf(const std::vector<std::pair<uint64_t, uint64_t> >& j, uint64_t m, int i)
{
    int c = 1;
    uint64_t endp = j[i].first;
    uint64_t startp = j[i].second + m;
    for (; j[i].first < startp && i < j.size(); i++) {
        if (j[i].second > endp) {
            endp = j[i].first;
            c++;
        }
    }
    return c;
}

int main(int argc, char* args[])
{
    int t;
    std::cin >> t;
    while (t--) {
        int n, m;
        std::cin >> n >> m;
        std::vector<std::pair<uint64_t, uint64_t> > j;
        std::vector<std::pair<uint64_t, uint64_t> > s(2*n);
        int c = 0;
        for (int i = 0; i < n; i++) {
            uint64_t a, b;
            std::cin >> a >> b;
            if (a-- > b--) {
                c++;
                j.push_back(std::make_pair(b + m, a));
                j.push_back(std::make_pair(b + 2*m, a + m));
            }
            else {
                j.push_back(std::make_pair(b, a));
                j.push_back(std::make_pair(b + m, a + m));
                j.push_back(std::make_pair(b + 2*m, a + 2*m));
            }
            s[2*i] = std::make_pair(a, 0);
            s[2*i+1] = std::make_pair(b, 1);
        }
        std::sort(j.begin(), j.end());
        std::sort(s.begin(), s.end());
        uint64_t p = 0;
        int cmin = c;
        for (auto& pr : s) {
            if (pr.second == 0) {
                c++;
            }
            else {
                c--;
                if (c < cmin) {
                    p = pr.first + 1;
                    cmin = c;
                }
            }
        }
        p += m;
        int mj;
        for (int i = 0; i < j.size(); i++) {
            if (j[i].second >= p) {
                mj = edf(j, m, i);
                break;
            }
        }
        for (int i = 0; i < j.size(); i++) {
            if (j[i].first >= p && j[i].second < p) {
                mj = std::max(mj, edf(j, m, i));
            }
        }
        std::cout << mj << std::endl;
    }
}