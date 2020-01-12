#include "../inc/ProjectPCH.h"
#include "../inc/RequestManager.h"

int main()
{
    std::cerr << /**ANSI_BLUE +*/ "Nice day, isn't it?" /**+ ANSI_RESET*/ << std::endl;
    std::cout << "Please, enter the number of threads >= 2" << std::endl;

    size_t number_of_threads;
    std::cin >> number_of_threads;
    assert(number_of_threads >= RequestManager::MIN_NUMBER_OF_CONSUMER_THREADS && "need >= 2 threads");
    RequestManager request_manager(number_of_threads);
    request_manager.simulate_activity();
    return 0;
}
