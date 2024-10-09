using namespace std;
bool f(string const& str){
    unordered_map<char,int> mapa;
    for (const auto& i:str) {
        if (mapa.count(i)) {
            mapa[i] += 1;
        } else {
            mapa[i] = 1;
        }
    }
    int impares = 0;
    for(const auto& c: mapa){
        if(c.second() % 2 != 0) impares ++;
        if (impares > 1) return false;
    }
    return true;
}
