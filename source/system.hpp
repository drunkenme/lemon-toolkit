// @date 2016/05/28
// @author Mao Jingkai(oammix@gmail.com)

#pragma once

#include "flow2d.hpp"
#include "entity.hpp"

NS_FLOW2D_BEGIN

struct System
{
    typedef size_t Type;

    virtual ~System() {}
    virtual void attach(SystemManager&) {}
    virtual void detach(SystemManager&) {}
    virtual void update(SystemManager&, float) {}

protected:
    friend class SystemManager;
    static Type s_type_counter;
};

template<typename T> struct SystemTrait : public System
{
    virtual ~SystemTrait() {}
    static Type type();
};

// an system for declaring component dependencies
template<typename C, typename ... Args>
struct RequireComponents : public SystemTrait<RequireComponents<C, Args...>>
{
    void attach(SystemManager&) override;
    void detach(SystemManager&) override;
    void receive(const EvtComponentAdded<C>&);

protected:
    template<typename D> void add_component(Entity);
    template<typename D, typename D1, typename ... Tails> void add_component(Entity);
};

struct SystemManager
{
    // non-copyable
    SystemManager(EntityManager&, EventManager&);
    SystemManager(const SystemManager&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;

    ~SystemManager();

    EventManager&   get_event_manager();
    EntityManager&  get_entity_manger();
    void            configure();
    void            update(float);

    template<typename S, typename ... Args> S* ensure(Args&& ... args);
    template<typename S, typename ... Args> S* add(Args&& ... args);
    template<typename S> void remove();
    template<typename S> S* get();

protected:
    bool                                        m_configured;
    EntityManager&                              m_entity_manager;
    EventManager&                               m_event_manager;
    std::unordered_map<System::Type, System*>   m_systems;
};

#include "system.inl"

NS_FLOW2D_END