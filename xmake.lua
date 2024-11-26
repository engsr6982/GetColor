add_rules("mode.release", "mode.debug")

target("GetColor")
    add_rules("qt.widgetapp")
    add_cxflags(
        "/utf-8"
    )
    add_defines(
        "NOMINMAX",
        "UNICODE"
    )
    add_files(
        "**.cpp",
        "**.h",
        "**.ui",
        "**.qrc",
        "**.rc"
    )
    add_includedirs(".")
    add_headerfiles("*.h")

    if is_mode("debug") then
        set_symbols("debug")
    end

    after_build(function (target) 
        -- copy cursor.png to build target directory
        os.cp("cursor.png", path.join(target:targetdir(), "cursor.png"))

        if is_plat("windows") and is_mode("release") then
            -- 获取 Qt SDK windeployqt.exe 工具，执行打包项目
            local qt = target:data("qt")
            if qt == nil then
                return
            end
            
            local windeployqt = path.join(qt.bindir, "windeployqt.exe")
            if not os.isfile(windeployqt) then
                return
            end
            
            -- 执行 windeployqt
            os.exec(string.format("\"%s\" \"%s\"", windeployqt, target:targetfile()))
        end
    end)
