/* Tested: https://judge.yosupo.jp/problem/inv_of_formal_power_series
Write by: std_abs*/
vector<int> polyInv(vector<int> a){
    int n = a.size();
    vector<int> ret(1, qp(a[0], MOD-2, MOD));

    for (int m=1 ; m<n ; m<<=1){
        if (n<2*m) a.resize(2*m);
        vector<int> v1(a.begin(), a.begin()+2*m), v2 = ret;
        v1.resize(4*m), v2.resize(4*m);
        NTT(v1), NTT(v2);
        for (int i=0 ; i<4*m ; i++) v1[i] = v1[i]*v2[i]%MOD*v2[i]%MOD;
        NTT(v1, true);
        ret.resize(2*m);
        for (int i=0 ; i<m ; i++) ret[i] = (ret[i]+ret[i])%MOD;
        for (int i=0 ; i<2*m ; i++) ret[i] = ((ret[i]-v1[i])%MOD+MOD)%MOD;
    }
    ret.resize(n);
    return ret;
}