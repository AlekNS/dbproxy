#ifndef __HANDLER_ALLOCATOR_H__
#define __HANDLER_ALLOCATOR_H__

#include <boost/cstdint.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/noncopyable.hpp>

namespace dbproxy
{

/**
 * Memory allocator for custom asio async handler.
 */
template <size_t alloc_size>
class handler_allocator : private boost::noncopyable
{
  public:
    handler_allocator() = default;
    ~handler_allocator() = default;

    void *allocate(size_t size)
    {
        if (!is_use && (size <= buffer.size))
        {
            is_use = true;
            return buffer.address();
        }
        return ::operator new(size);
    }

    void deallocate(void *pointer)
    {
        if (buffer.address() == pointer)
        {
            is_use = false;
            return;
        }
        ::operator delete(pointer);
    }

  private:
    bool is_use{false};
    boost::aligned_storage<alloc_size> buffer{};
};

}

#endif
