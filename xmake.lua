target('params')
    set_kind('binary')
    set_languages('cxx11')
    
    if is_plat('windows') then
        -- 编译选项
        add_cxflags('/utf-8')
        -- 静态链接选项
        add_ldflags('/NODEFAULTLIB:MSVCRT')
    end

    add_files('*.cpp')
