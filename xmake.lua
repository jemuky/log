target('log_test')
    set_kind('binary')
    set_languages('cxx11')
    
    if is_plat('windows') then
        -- 编译选项
        add_cxflags('/utf-8')
    end

    add_files('test/*.cpp')
    add_files('log.cpp')

package('log')
    set_homepage('https://github.com/jymk/log.git')
    set_description('cpp log output')

    set_urls('https://github.com/jymk/log/archive/refs/heads/main.zip')

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
            void test(int argc, char** argv) {
                DEBUG("log import suc");
            }
        ]]}, {configs = {languages = "c++11"}, includes = "log.h"}))
    end)
