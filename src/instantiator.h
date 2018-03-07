#ifndef __INSTANTINATOR_H__
#define __INSTANTINATOR_H__

namespace dbproxy
{

/**
 * Simple product creator.
 */
template <typename Tinstance, typename ...Targs>
class instantiator
{
  public:
    virtual ~instantiator() = default;
    virtual std::shared_ptr<Tinstance> create(Targs...) = 0;
};

/**
 * Simple fabric.
 * @NOTE: Thread not safe.
 */
template <typename Tinstantiator>
class registry
{
  public:
    static void register_it(const std::string &name, std::shared_ptr<Tinstantiator> instantiator) throw()
    {
        if (is_exists(name))
        {
            throw std::invalid_argument("Already registered");
        }

        instantiators.insert(std::make_pair(name, instantiator));
    }

    static bool is_exists(const std::string &name)
    {
        return instantiators.find(name) != instantiators.cend();
    }

    static std::shared_ptr<Tinstantiator> get(const std::string &name) throw()
    {
        if (!is_exists(name))
        {
            throw std::invalid_argument("Not found");
        }

        return instantiators.find(name)->second;
    }

  protected:
    static std::map<const std::string, std::shared_ptr<Tinstantiator>> instantiators;
};

}

#endif
