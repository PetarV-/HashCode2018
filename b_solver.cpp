struct car { int i, j, t; } ;
solution solve_b()
{
    std::sort(rides.begin(), rides.end(), [](ride a, ride b) { return a.start_t < b.start_t; });
    std::vector<car> cars;
    for(int i = 0; i < n_cars; i++) cars.push_back({0, 0, 0});

    solution sol;
    for(int i = 0; i < n_cars; i++)
        sol.sol.push_back({});
    
    for(ride r : rides)
    {
        bool ok = false;

        if(abs(r.start_i - r.end_i) + abs(r.start_j - r.end_j) > r.end_t - r.start_t)
        {
            std::cerr << "unsolvable ride: " << r.id << std::endl;
            continue;
        }

        std::vector<int> options;
        for(int i = 0; i < n_cars; i++)
        {
            int d = abs(cars[i].i - r.start_i) + abs(cars[i].j - r.start_j);
            if(d + cars[i].t <= r.start_t)
                options.push_back(i);
        }

        if(options.size() == 0)
        {
            std::cerr << "could not get bonus for ride " << r.id << "!" << std::endl;
            if(r.start_i + r.start_j > r.start_t) std::cerr << "(bonus not possible)" << std::endl;

            for(int i = 0; i < n_cars; i++)
            {
                int d = abs(cars[i].i - r.start_i) + abs(cars[i].j - r.start_j);
                if(d + cars[i].t + abs(r.start_i - r.end_i) + abs(r.start_j - r.end_j))
                    options.push_back(i);
            }
        }

        if(options.size() == 0)
        {
            std::cerr << "could not allocate ride " << r.id << std::endl;
        }
        else
        {
            std::random_shuffle(options.begin(), options.end());
            int i = options[0];
            int d = abs(r.end_i - r.start_i) + abs(r.end_j - r.start_j);
//            cars[i].t = r.start_t + d;
            cars[i].t = r.end_t;
            cars[i].i = r.end_i;
            cars[i].j = r.end_j;
            sol.sol[i].push_back(r.id);
        }
    }
    
    return sol;
}
