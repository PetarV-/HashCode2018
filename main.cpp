#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>

using namespace std;

struct ride
{
    int start_i, start_j, end_i, end_j;
    int start_t, end_t;
} ;

struct solution
{
    vector<int> ok_ride; // 0 = not solved, 1 = no bonus, 2 = bonus
    vector<vector<int> > sol;
};

int rows, cols, n_cars, n_rides, start_bonus, steps;
std::vector<ride> rides;

void load_test(const char *test_name)
{
    std::stringstream s;
    s << "tests/" << test_name << ".in";
    FILE *f = fopen(s.str().c_str(), "r");

    fscanf(f, "%d %d %d %d %d %d", &rows, &cols, &n_cars, &n_rides, &start_bonus, &steps);
    for(int i = 0; i < n_rides; i++)
    {
        ride x;
        fscanf(f, "%d %d %d %d %d %d", &x.start_i, &x.start_j, &x.end_i, &x.end_j, &x.start_t, &x.end_t);
        rides.push_back(x);
    }

    fclose(f);
}

void clean_rides()
{
    vector <ride> nwrides;
    for (int i=0; i<rides.size(); i++)
    {
        int startlen=rides[i].start_i+rides[i].start_j;
        int ridelen=abs(rides[i].end_i-rides[i].start_i)+abs(rides[i].end_j-rides[i].start_j);
        int timewindow=rides[i].end_t-rides[i].start_t;
        if (startlen+ridelen<=rides[i].end_t && timewindow>=ridelen)
        {
            nwrides.push_back(rides[i]);
        }
    }
    rides=nwrides;
}

int calc_score(const solution &sol) {
    int score = 0;
    for(int i = 0; i < sol.ok_ride.size(); i++) {
        if (sol.ok_ride[i] == 0) continue;
        if (sol.ok_ride[i] == 2) score += start_bonus;
        score += abs(rides[i].start_i-rides[i].end_i) + abs(rides[i].start_j-rides[i].end_j); 
    }
    return score;
}

void print_output(const solution &sol, const char *filename)
{
    if(sol.sol.size() != n_cars)
    {
        std::cerr << "warning: sol.size() != n_cars!" << std::endl;
        std::cerr << "sol.size() == " << sol.sol.size() << ", n_cars = " << n_cars << std::endl;
    }
    
    FILE *f = fopen(filename, "w");
    for(int i = 0; i < sol.sol.size(); i++)
    {
        fprintf(f, "%d", sol.sol[i].size());
        for(int x : sol.sol[i])
            fprintf(f, " %d", x);
        fprintf(f, "\n");
    }
    std::cerr << "Output score: " << calc_score(sol) << std::endl;

    fclose(f);
    std::cerr << "Wrote output file " << filename << std::endl;
}

void save_checkpoint(const solution &sol, const char *filename)
{
    FILE *f = fopen(filename, "w");
    if(!f)
    {
        std::cerr << "Could not write to checkpoint file: " << filename << std::endl;
        return;
    }

    for(int i = 0; i < sol.sol.size(); i++)
    {
        for(int x : sol.sol[i]) fprintf(f, "%d ", x);
        fprintf(f, "-1\n");
    }

    for(int x : sol.ok_ride)
        fprintf(f, "%c", '0' + x);
    fprintf(f, "\n");

    fclose(f);
}

solution load_checkpoint(const char *filename)
{
    FILE *f = fopen(filename, "r");
    solution res;
    
    if(!f)
    {
        std::cerr << "Could not read checkpoint file: " << filename << std::endl;
        return res;
    }
    
    for(int i = 0; i < n_cars; i++)
    {
        std::vector<int> tmp;
        while(1)
        {
            int x;
            fscanf(f, "%d", &x);
            if(x == -1) break;
            tmp.push_back(x);
        }
        res.sol.push_back(tmp);
    }

    for(int i = 0; i < n_rides; i++)
    {
        char c;
        fscanf(f, " %c", &c);
        res.ok_ride.push_back(c - '0');
    }

    fclose(f);
    return res;
}

int mx_score()
{
  int ret=0;
  for (int i=0; i<rides.size(); i++)
    ret+=abs(rides[i].start_i-rides[i].end_i)+abs(rides[i].start_j-rides[i].end_j)+start_bonus;
  return ret;
}

vector <int> end_to_start(vector <int> sat)
{
  vector <int> rt;
  for (int i=0; i<rides.size(); i++)
  {
    int mn=1<<25;
    for (int j=0; j<rides.size(); j++)
      if (j!=i && sat[j]==0)
      {
        int nw=abs(rides[i].end_i-rides[j].start_i)+abs(rides[i].end_j-rides[j].start_j);
        nw=min(nw,mn);
      }
    rt.push_back(mn);
  }
  return rt;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <test name>" << std::endl;
        return 1;
    }

    if(argv[1][1] == '\0')
    {
        if(argv[1][0] == 'a') load_test("a_example");
        else if(argv[1][0] == 'b') load_test("b_should_be_easy");
        else if(argv[1][0] == 'c') load_test("c_no_hurry");
        else if(argv[1][0] == 'd') load_test("d_metropolis");
        else if(argv[1][0] == 'e') load_test("e_high_bonus");
        else load_test(argv[1]);
    }
    else load_test(argv[1]);
    
    return 0;
}
