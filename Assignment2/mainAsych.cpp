#include <vector>
#include <iostream>
#include <thread>
#include <future>
#include <utimer.cpp>

using namespace std;

auto f = [] ( int x ){ return x*x; };

auto body(vector<double> *vec, int start, int end)
{
    {
        long usecs;
        utimer t0("Asynch Thread ", &usecs);
        for (int i=start; i<end; i++)
        {
            (*vec)[i] = f((*vec)[i]);
        }
    }
}



int main(int argc, char* argv[]){

    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " vector_len seed threshold printflag threads" << endl; 
        return(0);
    }

    int vec_len = (argc > 1 ? atoi(argv[1]) : 100);  // len of the vector(s)
    int s = (argc > 2 ? atoi(argv[2]) : 123); // seed 
    int threshold = (argc > 3 ? atoi(argv[3]) : 10); // threshold to split
    bool pf=(argc > 4 ? (argv[4][0]=='t' ? true : false) : true);  
    
    vector<double> w(vec_len);
    vector<double> v(vec_len);
    const int max = 8; 
    srand(s); 
    for(int i=0; i<vec_len; i++){   
        v[i] = (double) (rand() % max);
        w[i] = v[i];
    }

    long usecs; 
    {
        utimer t0("Computation",&usecs); 
        int start,end = 0;
        int bucket = vec_len/threshold;
        for (int i = 0;  i<bucket; i++){
            start = i*bucket;
            end = (vec_len < start+bucket)? vec_len : start + bucket;
            auto threadsRes = async(launch::async, body, &v, start, end);
        }
    }

    cout << "End (spent " << usecs << " usecs for " << vec_len << " elements)" << endl;
    if(pf)                                    // print results (if needed)
        for(int i=0; i<vec_len; i++) cout <<i<<": "<< w[i]<<" -> "<< v[i] << endl;
}