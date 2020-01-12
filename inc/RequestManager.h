#include "ProjectPCH.h"
#include "Stopper.h"
#include "Request.h"

std::string ANSI_BLUE = "\u001B[34m"; //NOLINT
std::string ANSI_RESET = "\u001B[0m"; //NOLINT

struct RequestManager
{
    static const int MIN_NUMBER_OF_CONSUMER_THREADS = 2;
private:
    static const int ACTIVE_TIME_SEC = 30;
    boost::lockfree::queue<Request *> m_queue{30};
    Stopper m_stopper;
    boost::thread_group m_producer_threads;
    boost::thread_group m_consumer_threads;

    const size_t m_number_of_producers = 2;
    size_t m_number_of_consumers;


public:
    explicit RequestManager(size_t m_number_of_consumers) : m_number_of_consumers(m_number_of_consumers)
    {}

    void simulate_activity()
    {
        for (size_t i = 0; i < m_number_of_producers; i++)
        {
            m_producer_threads.create_thread([this]()
                                             {
                                                 while (!m_stopper.is_suspended())
                                                 {
                                                     Request *request = get_request(m_stopper);
                                                     if (request != nullptr)
                                                     {
                                                         m_queue.push(request);
                                                     }
                                                     sleep(1);
                                                 }
                                             });
        }
        for (size_t i = 0; i < m_number_of_consumers; i++)
        {
            m_consumer_threads.create_thread([this]()
                                             {
                                                 while (!m_stopper.is_suspended())
                                                 {
                                                     Request *request;
                                                     if (m_queue.pop(request))
                                                     {
                                                         process_request(request, m_stopper);
                                                         delete request;
                                                     }
                                                 }
                                             });
        }
        sleep(ACTIVE_TIME_SEC);
        m_stopper.suspend();

#if defined BOOST_THREAD_PROVIDES_INTERRUPTIONS
        m_producer_threads.interrupt_all();
        m_consumer_threads.interrupt_all();
#endif
        m_producer_threads.join_all();
        m_consumer_threads.join_all();

        dump_queue();
    }

    void dump_queue()
    {
        while (!m_queue.empty())
        {
            Request *req;
            m_queue.pop(req);
            std::cout << "Request " << req->get_index()
                      << " is suspended and dumped" << std::endl;
            delete req;
        }
    }

};