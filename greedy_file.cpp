#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <ctime>
#include <string>

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

inline int abs1(int x)
{
    if (x < 0) return -x;
    return x;
}

inline int max1(int a, int b)
{
    return (a > b) ? a : b;
}

// mod: 0 --> arrange by arr_t
// mod: 1 --> arrange by ret_t
// mod: 2 --> arrange by dt
solution greedy_solve(solution s, int mod, bool bonuz)
{
    solution ret;
    ret.ok_ride = s.ok_ride;
    ret.sol = s.sol;

    vector<int> order;
    for(int i = 0; i < n_cars; i++) order.push_back(i);
    //random_shuffle(order.begin(), order.end());
    
    for (int _i=0;_i<n_cars;_i++)
    {
        int i = order[_i];
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
                    int cur_st = (arr_t == rides[j].start_t) ? 2 : 1;
                    if (ret_t <= rides[j].end_t)
                    {
                        if (mod == 0)
                        {
                            if (best_r == -1 || (bonuz && cur_st > best_st) || (bonuz && cur_st == best_st && arr_t < best_t) || (!bonuz && arr_t < best_t)) 
                            {
                                // can also try ret_t < best_t
                                best_r = j;
                                best_t = arr_t;
                                best_f = ret_t;
                                best_st = (arr_t == rides[j].start_t) ? 2 : 1;
                            }
                        }
                        else if (mod == 1)
                        {
                            if (best_r == -1 || (bonuz && cur_st > best_st) || (bonuz && cur_st == best_st && ret_t < best_t) || (!bonuz && ret_t < best_t)) 
                            {
                                // can also try ret_t < best_t
                                best_r = j;
                                best_t = ret_t;
                                best_f = ret_t;
                                best_st = (arr_t == rides[j].start_t) ? 2 : 1;
                            }
                        }
                        else if (mod == 2)
                        {
                            if (best_r == -1 || (bonuz && cur_st > best_st) || (bonuz && cur_st == best_st && dt < best_t) || (!bonuz && dt < best_t)) 
                            {
                                // can also try ret_t < best_t
                                best_r = j;
                                best_t = dt;
                                best_f = ret_t;
                                best_st = (arr_t == rides[j].start_t) ? 2 : 1;
                            }
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

int score(const solution &s) { return 0; }

void remove_car(solution &s, int c)
{
    for(int x : s.sol[c])
        s.ok_ride[x] = 0;
    s.sol[c].clear();
}

int calc_score(const solution &sol) {
    int score = 0;
    for(int i = 0; i < sol.ok_ride.size(); i++) {
        if (sol.ok_ride[i] == 0) continue;
        if (sol.ok_ride[i] == 2) score += start_bonus;
        if(sol.ok_ride[i] == 1 || sol.ok_ride[i] == 2) score += abs(rides[i].start_i-rides[i].end_i) + abs(rides[i].start_j-rides[i].end_j); 
    }
    return score;
}

bool equal(const solution &a, const solution &b)
{
    for(int i = 0; i < n_cars; i++)
    {
        if(a.sol[i].size() != b.sol[i].size())
        {
//            std::cerr << a.sol[i].size() << " " << b.sol[i].size() << " " << i << std::endl;
            return false;
        }
        for(int j = 0; j < a.sol[i].size(); j++)
            if(a.sol[i][j] != b.sol[i][j]) return false;
    }

    return true;
}

int best_known;
string prefix;

solution full_solve(solution init, int n_rem)
{
    init = greedy_solve(init, 1, false);
    int curr_score = score(init);

    auto last_save = clock();
    int save_id = 0;

    bool first = true;
    int TTL = (n_rem == 20) ? 12346789 : 200;
    int ttl = TTL;
    while(ttl--)
    {
        solution next = init;
        for(int rem = 0; rem < 15; rem++)
            remove_car(next, rand() % n_cars);

        if(!first)
            for(int &x : next.ok_ride)
                if(x == 0 && rand() % 50 == 0)
                    x = 3;
        first = false;
        
        next = greedy_solve(next, rand() % 3, rand() % 2 > 0);
//        if(n_rem == 50) next = full_solve(next, 20);
        if(n_rem > 5) next = full_solve(next, 5);

        for(int &x : next.ok_ride)
            if(x == 3) x = 0;
//        if(equal(next, init)) std::cerr << "nothing changed" << std::endl;
        int next_score = calc_score(next);
        if(next_score > curr_score)
        {
//            if(n_rem < 50) printf("    ");
            if(n_rem < 20) printf("    ");
            printf("Improved score to %d (+%d)\n", next_score, next_score - curr_score);
            curr_score = next_score;
            init = next;
            ttl = TTL;
        }

        if(clock() > last_save + 20 * CLOCKS_PER_SEC && curr_score > best_known)
        {
            best_known = curr_score;
            stringstream s;
            s << "checkpoints/" << prefix << "-" << save_id++;
            save_checkpoint(init, s.str().c_str());
            s << ".out";
            print_output(init, s.str().c_str());

            last_save = clock();
            std::cerr << "checkpoint " << save_id - 1 << ": " << calc_score(init) << std::endl;
        }

    }
    std::cerr << "TTL expired" << std::endl;

    return init;
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

    prefix = argv[1];

    solution gr_sol;
    gr_sol.ok_ride = vector<int>(n_rides);
    gr_sol.sol = vector<vector<int> >(n_cars, vector<int>());

    solution tst = full_solve(gr_sol, 20);

    print_output(tst, argv[1]);

    std::stringstream ss;
    ss << "out/" << argv[1] << ".sol";
    print_output(tst, ss.str().c_str());
    
    return 0;
}
