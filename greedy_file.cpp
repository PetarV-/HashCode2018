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
    vector<int> ok_ride;
    vector<vector<int> > sol;
};

int rows, cols, n_cars, n_rides, start_bonus, steps;
std::vector<ride> rides;

inline int abs1(int x)
{
    if (x < 0) return -x;
    return x;
}

inline int max1(int a, int b)
{
    return (a > b) ? a : b;
}

solution greedy_solve(solution s)
{
    solution ret;
    ret.ok_ride = s.ok_ride;
    ret.sol = s.sol;

    for (int i=0;i<n_cars;i++)
    {
        if (!ret.sol[i].empty()) continue;
        int cur_t = 0;
        int cur_i = 0, cur_j = 0;
        while (true)
        {
            int best_r = -1;
            int best_t = -1;
            int best_f = -1;
            int best_st = 0;
            for (int j=0;j<n_rides;j++)
            {
                if (!ret.ok_ride[j])
                {
                    // check if we make it on time
                    int dt = abs1(cur_i - rides[j].start_i) + abs1(cur_j - rides[j].start_j);
                    int tt = abs1(rides[j].end_i - rides[j].start_i) + abs1(rides[j].end_j - rides[j].start_j);
                    int arr_t = max1(rides[j].start_t, cur_t + dt);
                    int ret_t = arr_t + tt;
                    int cur_st = (arr_t == rides[j].start_t);
                    if (ret_t <= rides[j].end_t)
                    {
                        if (best_r == -1 || cur_st > best_st || (cur_st == best_st && arr_t < best_t)) 
                        {
                            // can also try ret_t < best_t
                            best_r = j;
                            best_t = arr_t;
                            best_f = ret_t;
                            best_st = (arr_t == rides[j].start_t) ? 2 : 1;
                        }
                    }
                }
            }
            if (best_r == -1) break;
            cur_t = best_f;
            cur_i = rides[best_r].end_i;
            cur_j = rides[best_r].end_j;
            ret.ok_ride[best_r] = best_st;
            ret.sol[i].push_back(best_r);
        }
    }
    return ret;
}

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

    fclose(f);
    std::cerr << "Wrote output file " << filename << std::endl;
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

    solution gr_sol;
    gr_sol.ok_ride = vector<int>(n_rides);
    gr_sol.sol = vector<vector<int> >(n_cars, vector<int>());

    solution tst = greedy_solve(gr_sol);

    print_output(tst, argv[1]);

    std::cout << "Test " << argv[1] << std::endl;
    std::cout << "map is " << rows << "x" << cols << std::endl;
    std::cout << "num of cars = " << n_cars << std::endl;
    std::cout << "num of rides = " << n_rides << std::endl;
    std::cout << "bonus for starting on time = " << start_bonus << std::endl;
    std::cout << "num of steps = " << steps << std::endl;
    
    return 0;
}