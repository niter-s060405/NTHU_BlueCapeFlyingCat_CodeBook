/*
Tested : https://cses.fi/paste/7192deb5028acbcbe90d03/
Author : std_abs
*/
struct BCC_AP { // 0-based, remember to build
    int n, nbcc; // note for isolated point
    vector<vector<int>> E, F; // id >= n: 方點
    vector<int> pa, dep, low, stk, paf, depf;
    void dfs(int v, int p) {
        dep[v] = low[v] = ~p ? dep[p] + 1 : 0;
        stk.push_back(v), pa[v] = p;
        for (auto& u : E[v]) if (u != p) {
            if (low[u] == -1) {
                dfs(u, v), low[v] = min(low[v], low[u]);
                if (low[u] >= dep[v]) {
                    int id = nbcc++, x;
                    do {
                        x = stk.back(), stk.pop_back();
                        F[id + n].push_back(x), F[x].push_back(id + n);
                    } while (x != u);
                    F[id + n].push_back(v), F[v].push_back(id + n);
                }
            } else low[v] = min(low[v], dep[u]);
        }
    }
    bool is_cut(int x) { return F[x].size() != 1; }
    vector<int> bcc(int id) { return F[id + n]; }
    int bcc_id(int u, int v) { // starts from 0
        return paf[depf[u] < depf[v] ? v : u] - n; }
    void dfs2(int v, int p) {
        depf[v] = ~p ? depf[p] + 1 : 0, paf[v] = p;
        for (int u : F[v]) if (u != p) dfs2(u, v);
    }
    void build() {
        low.assign(n, -1);
        for (int i = 0; i < n; ++i) if (low[i] == -1)
            dfs(i, -1), dfs2(i, -1);
    }
    void add_edge(int u, int v) {
        E[u].push_back(v), E[v].push_back(u);
    }
    BCC_AP (int _n) : n(_n), nbcc(0), E(n), F(2 * n), pa(n), dep(n), low(n), stk(), paf(n * 2), depf(n * 2) {}
};
