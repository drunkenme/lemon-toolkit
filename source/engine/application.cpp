// @date 2016/08/17
// @author Mao Jingkai(oammix@gmail.com)

#include <engine/application.hpp>
#include <engine/engine.hpp>
#include <engine/arguments.hpp>
#include <core/core.hpp>

NS_LEMON_BEGIN

Application::Application() : _exitcode(0)
{}

void Application::parse(const char* path)
{
    ENSURE(core::get_subsystem<Arguments>()->parse(path));
}

int Application::run()
{
    std::srand(time(0));
    core::details::initialize();
    core::add_subsystem<Arguments>();

    setup();
    if( _exitcode != 0 )
        return _exitcode;

    auto engine = core::add_subsystem<Engine>();
    start();
    if( _exitcode != 0 )
        return _exitcode;

    while( engine->is_running() )
        engine->run_one_frame();

    stop();

    core::details::dispose();
    return _exitcode;
}

void Application::terminate_with_error(const std::string& message)
{
//    LOGE(message.c_str());
    _exitcode = -1;
}

NS_LEMON_END