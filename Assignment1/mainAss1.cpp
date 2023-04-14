#include <vector>
#include <iostream>
#include <thread>
#include <cmath>
#include <utimer.cpp>

using namespace std;

auto f = [] ( int x ){ return x*x; };
auto g = [] (double x) { for(int i=0; i<1000; i++) x = x+1; return(x); };

auto body(vector<int> v, vector<int> *w, int start, int end, int threadId){
    long usecs; 
    {
        utimer t0(&usecs, threadId);
        for(int i=start; i<end; i++){
                (*w)[i] = f(v[i]);
        }
    }
}

int main(int argc, char* argv[]){

    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " vector_len seed threads printflag" << endl; 
        return(0);
    }

    int vec_len = argc > 1 ? atoi(argv[1]) : 99;  // len of the vector(s)
    int s = argc > 2 ? atoi(argv[2]) : 123; // seed
    int threads = argc > 3 ? atoi(argv[3]) : 10;   // par degree
    bool pf= argc > 4 && argv[4][0] == 't';

    vector<int> v(vec_len);
    vector<int> w(vec_len, 0);
    vector<thread*> tids;

    const int max = 8; 
    srand(s); 
    for(int i=0; i<vec_len; i++)                  
        v[i] = (double) (rand() % max);

    long usecs; 
    {
        utimer t0("Parallel computation",&usecs); 
        int start,end = 0;
        int bucket = vec_len/threads;
        for (int i = 0;  i<threads; i++){
            start = i*bucket;
            end = (i==threads-1)? vec_len : (i+1)*bucket;
            tids.push_back(new thread(body, v, &w, start, end, i));
        }

        for(auto t : tids) t->join();
    } 
    cout << "End (spent " << usecs << " usecs using " << threads << " threads for " << vec_len << " elements)" << endl;
    if(pf)                                    // print results (if needed)
        for(int i=0; i<vec_len; i++) cout <<i<<": "<< v[i]<<" -> "<< w[i]<< endl;

}