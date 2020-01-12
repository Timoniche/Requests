#ifndef TRANSAS_REQUEST_H
#define TRANSAS_REQUEST_H

#include "../inc/ProjectPCH.h"
#include "Stopper.h"

/** todo:
1) Организовать приём запросов, для этого класть в одну очередь задания возвращаемые
   функцией GetRequest.
2) Запустить несколько обрабатывающих запросы потоков (переменное число, но не менее двух),
   которые должны обрабатывать поступающие из очереди задания с помощью ProcessRequest.
3) Поработать в течение 30 секунд
4) Корректно остановить все потоки. Если остались не обработанные задания, не обрабатывать их
   и корректно удалить
5) Завершить программу
 */

namespace RandomUtils
{
    std::random_device rd; //NOLINT
    std::mt19937 gen{rd()}; //NOLINT
    std::uniform_real_distribution<double> dist{1, RAND_MAX}; //NOLINT

    int get_random_int()
    {
        return static_cast<int>(dist(gen));
    }
}

class Request
{
private:
    size_t _index;
public:
    std::mutex _mx;
    static std::mutex _cout_mx;
public:
    Request()
    {
        static std::atomic <size_t> request_counter = 0;
        _index = request_counter++;
    }

    inline size_t get_index() noexcept
    {
        return _index;
    }

    /**
    * возвращает nullptr, если объект stopSignal указывает на необходимость остановки
    * либо указатель на память, которую в дальнейшем требуется удалить
    */
    friend Request *get_request(const Stopper &stop_signal) noexcept;

    /**
    * обрабатывает запрос, но память не удаляет, завершает обработку досрочно, если
    * объект stopSignal указывает на необходимость остановки
     */
    friend void process_request(Request *request, const Stopper &stop_signal) noexcept;
};

std::mutex Request::_cout_mx;

Request *get_request(const Stopper &stop_signal) noexcept
{
    if (stop_signal.is_suspended())
    { return nullptr; }
    auto *ret = new Request();
    std::unique_lock <std::mutex> lock(Request::_cout_mx);
    std::cout << "init " << ret->_index << std::endl;
    return ret;
}

void process_request(Request *request, const Stopper &stop_signal) noexcept
{
    if (stop_signal.is_suspended()) return;
    int rand = RandomUtils::get_random_int();
    for (int spin_wait = 0; spin_wait < rand; ++spin_wait);
    std::unique_lock <std::mutex> lock1(request->_mx, std::defer_lock);
    std::unique_lock <std::mutex> lock2(Request::_cout_mx, std::defer_lock);
    std::lock(lock1, lock2);
    std::cout << "busy spin, request number: " << request->_index << std::endl;
}


#endif //TRANSAS_REQUEST_H
