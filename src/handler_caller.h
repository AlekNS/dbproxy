#ifndef __HANDLER_CALLER_H__
#define __HANDLER_CALLER_H__

#include <boost/cstdint.hpp>
#include <boost/asio.hpp>

namespace dbproxy
{

/**
 * Custom asio async handler.
 */
template <typename Allocator, typename F>
class handler_caller
{
  public:
    using handler_type = handler_caller<Allocator, F>;

    handler_caller(Allocator &allocator, const F &h) : allocator(allocator),
                                                       handler(h)
    {
    }

    handler_caller(const handler_type &o) : allocator(o.allocator),
                                            handler(o.handler)
    {
    }

    ~handler_caller() = default;

    friend void *asio_handler_allocate(std::size_t size, handler_type *ctx)
    {
        return ctx->allocator.allocate(size);
    }

    friend void asio_handler_deallocate(void *ptr, std::size_t, handler_type *ctx)
    {
        ctx->allocator.deallocate(ptr);
    }

    template <typename H>
    friend void asio_handler_invoke(const H function, handler_type *context)
    {
        using boost::asio::asio_handler_invoke;
        asio_handler_invoke(function, boost::addressof(context->handler));
    }

    template <typename... Args>
    void operator()(const Args &... args)
    {
        handler(args...);
    }

  private:
    Allocator &allocator;
    F handler;
};

template <typename Allocator, typename F>
inline handler_caller<Allocator, F>
make_custom_handler(Allocator &allocator, const F &f)
{
    return handler_caller<Allocator, F>(allocator, f);
}

}

#endif
