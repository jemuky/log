target('log_test')
    set_kind('binary')
    set_languages('cxx11')
    
    if is_plat('windows') then
        -- 编译选项
        add_cxflags('/utf-8')
    end

    add_files('test/*.cpp')
    add_files('log.cpp')
