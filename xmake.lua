package('log')
    set_homepage('https://github.com/jymk/log.git')
    set_description('cpp log output')

    set_urls('https://github.com/jymk/log/archive/refs/heads/main.zip')
    add_versions('v0.0.1', '217c58cbb30d668536faf755566b0031f7d9708f20691e8f2b27f47b42666df8')

    on_install(function (package) 
        io.writefile('xmake.lua', [[
            add_rules("mode.debug", "mode.release")
            target("log")
                set_kind("static")
                add_headerfiles("log.h")
                add_files("log.cpp")
        ]])

        import("package.tools.xmake").install(package)
    end)

    on_test(function (package) 
        assert(package:check_cxxsnippets({test = [[
            #include <log.h>
            void test() {
                DEBUG("log import suc");
            }
        ]]}, {configs = {languages = "c++20"}, includes = "log.h"}))
    end)
package_end()


target('log_test')
    set_kind('binary')
    set_languages('cxx20')
    
    if is_plat('windows') then
        -- 编译选项
        add_cxflags('/utf-8')
    end

    add_files('log.cpp', 'test/*.cpp')
