#include <chrono>
#include <iostream>
#include <algorithm>

#include "logger.h"

using namespace dbproxy;

template <>
std::map<const std::string, std::shared_ptr<logger_instantiator>> logger_registry::registry::instantiators{};

logger_instantiator_simple_out_file::logger_instantiator_simple_out_file(const std::string &logfile) : outfile(logfile)
{
}

std::shared_ptr<logger> logger_instantiator_simple_out_file::create()
{
    if (instance == nullptr)
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr)
        {
            auto ptr = new logger_file(outfile);
            instance = std::shared_ptr<logger_file>(ptr);
            ptr->start();
        }
    }
    return instance;
}

std::mutex logger_instantiator_simple_out_file::mtx{};
std::shared_ptr<logger> logger_instantiator_simple_out_file::instance = nullptr;

logger_file::logger_file(const std::string &file) : outfile(file),
                                                    done(false)
{
}

void logger_file::out(const std::string &str)
{
    std::lock_guard<std::mutex> lock(mtx);

    if (buffer.size() < max_buffer_size)
    {
        buffer.push_back(str);
    }
    else
    {
        std::cerr << "Log buffer overflow, this is backpressure problem" << std::endl;
    }
}

void logger_file::start()
{
    outstream.open(outfile, std::ios::app);
    worker_thread = std::thread([this] {
        write_worker();
    });
}

void logger_file::stop()
{
    done = true;
    worker_thread.join();
}

void logger_file::write_worker()
{
    try
    {
        std::string str;
        std::list<std::string> local_buffer{};
        while (!done)
        {
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (!buffer.empty())
                {
                    std::swap(local_buffer, buffer);
                }
            }

            if (!local_buffer.empty())
            {
                std::for_each(local_buffer.cbegin(), local_buffer.cend(), [this](const std::string &str) {
                    outstream << str << "\n";
#ifdef NDEBUG
                    std::cout << str << "\n";
#endif
                });
                outstream.flush();
                local_buffer.clear();
                continue;
            }
            else
            {
                // ugly way
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error was occurred when async logger work" << e.what() << std::endl;
    }
}
