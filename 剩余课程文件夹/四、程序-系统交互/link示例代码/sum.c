int sum(int *a,int n){
    int res;
    for(int i = 0;i<n;i++){
        res += a[i];
    }
    return res;
}