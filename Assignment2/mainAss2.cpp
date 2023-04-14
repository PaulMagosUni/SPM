#include <vector>
#include <iostream>
#include <thread>
#include <utimer.cpp>

using namespace std;

auto f = [] ( int x ){ return x*x; };

auto body(vector<double> *vec, int start, int end, int threadId)
{
    {
        // long usecs;
        // utimer t0(&usecs, threadId);
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
    int threads = (argc > 5 ? atoi(argv[5]) : -1); // threads to split
    
    vector<double> v(vec_len);
    vector<double> w(vec_len);
    const int max = 8; 
    srand(s); 
    for(int i=0; i<vec_len; i++){   
        v[i] = (double) (rand() % max);
        w[i] = v[i];
    }

    vector<thread*> tids;   
    long usecs; 
    {
        utimer t0("Computation",&usecs); 
        int start,end = 0;
        int bucket = (threads)? vec_len/threshold : threads;
        for (int i = 0;  i<((threads==-1)? bucket : threads); i++){
            start = i*bucket;
            end = (vec_len < start+bucket)? vec_len : start + bucket;
            tids.push_back(new thread(body, &v, start, end, i));
        }
        threads = tids.size();
        for(auto t : tids) t->join();
    }

    cout << "End (spent " << usecs << " usecs using " << threads << " threads for " << vec_len << " elements)" << endl;
    if(pf)                                    // print results (if needed)
        for(int i=0; i<vec_len; i++) cout <<i<<": "<< w[i]<<" -> "<< v[i] << endl;
}